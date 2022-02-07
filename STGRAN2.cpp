#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include <math.h>
#include <algorithm>
#include <PField.h>
#include <Instrument.h>
#include "STGRAN2.h"			  // declarations for this instrument class
#include <rt.h>
#include <rtdefs.h>
#include <iostream>
#include <vector>

// Construct an instance of this instrument and initialize some variables.
// Using an underbar as the first character of a data member is a nice
// convention to follow, but it's not necessary, of course.  It helps you
// to see at a glance whether you're looking at a local variable or a
// data member.

AUDIOBUFFER::AUDIOBUFFER(int maxSize): _head(0), _full(false)
{
    _buffer = new std::vector<double>(maxSize);
}

AUDIOBUFFER::~AUDIOBUFFER()
{
    delete _buffer;
}

void AUDIOBUFFER::Append(double sample)
{
    (*_buffer)[_head] = sample;
    if (_head == (int) _buffer->size() -1)
	_full = true;
    _head = (_head + 1) % _buffer->size();
}

double AUDIOBUFFER::Get(float index) // maybe add interpolation at some point
{
	while (index < 0)
		index += (float) _buffer->size();
	while (index > _buffer->size())
		index -= (float) _buffer->size();
    return (*_buffer)[(int) index];
}

int AUDIOBUFFER::GetHead()
{
    return _head;
}

int AUDIOBUFFER::GetSize()
{
    return (int) _buffer->size();
}

bool AUDIOBUFFER::GetFull()
{
    return _full;
}

void AUDIOBUFFER::Print()
{
    for (size_t i = 0; i < _buffer->size(); i++)
    {
        std::cout << (*_buffer)[i] << " ,";
    }
    std::cout << "\n";
}

STGRAN2::STGRAN2() : branch(0)
{
}


// Destruct an instance of this instrument, freeing any memory you've allocated.

STGRAN2::~STGRAN2()
{
	//std::cout << " grains used " << grainsUsed << "\n";

	for (size_t i = 0; i < grains->size(); i++)
	{
		delete (*grains)[i];
	}
	delete grains;
	delete buffer;
}


// Called by the scheduler to initialize the instrument. Things done here:
//   - read, store and check pfields
//   - set output file (or bus) pointer
//   - init instrument-specific things
// If there's an error here (like invalid pfields), call and return die() to
// report the error.  If you just want to warn the user and keep going,
// call warn() or rterror() with a message.

int STGRAN2::init(double p[], int n_args)
{

	/* Args:
		p0: inskip
		p1: dur
		p2: amp
		p3: grainRate
		p4: grainRateVarLow
		p5: grainRateVarMid
		p6: grainRateVarHigh
		p7: grainRateVarTigh
		p8: grainDurLow
		p9: grainDurMid
		p10: grainDurHigh
		p11: grainDurTight
		p12: transLow (cents)
		p13: transMid
		p14: transHigh
		p15: transTight
		p16: panLow
		p17: panMid
		p18: panHigh
		p19: panTight
		p20: grainEnv
	*/
	if (rtsetinput(p[0], this) == -1)
      		return DONT_SCHEDULE; // no input

	if (rtsetoutput(p[0], p[1], this) == -1)
		return DONT_SCHEDULE;

	if (outputChannels() > 2)
	      return die("STGRAN2", "Output must be mono or stereo.");

	if (n_args < 21)
		return die("STGRAN2", "all arguments are required");

	if (inputChannels() > 1)
		return die("STGRAN2", "Currently only accepting mono input");

	buffer = new AUDIOBUFFER(44100); // figure out different buffer sizes

	grainEnvLen = 0;
	amp = p[2];

	grainRate = p[3];
	newGrainCounter = 0;
	grainRateSamps = round(grainRate * SR);

	// init tables
	grainEnv = (double *) getPFieldTable(20, &grainEnvLen);

	// make the needed grains, which have no values yet as they need to be set dynamically
	grains = new std::vector<Grain*>();
	// maybe make the maximum grain value a non-pfield enabled parameter
	for (int i = 0; i < 1500; i++)
	{
		addgrain();
	}

	return nSamps();
}



// For non-interactive (script-driven) sessions, the constructor and init()
// for every instrument in the script are called before any of them runs.
// By contrast, configure() is called right before the instrument begins
// playing.  If we were to allocate memory at init time, then all notes in
// the score would allocate memory then, resulting in a potentially excessive
// memory footprint.  So this is the place to allocate any substantial amounts
// of memory you might be using.

int STGRAN2::configure()
{
	return 0;	// IMPORTANT: Return 0 on success, and -1 on failure.
}
// void addgrain(float sampInc; float trans; float dur; float pan; bool isplaying;);
//        void resetgrsain(Grain* grain);
//        int calcgrainsrequired();

double STGRAN2::prob(double low,double mid,double high,double tight)
        // Returns a value within a range close to a preferred value
                    // tightness: 0 max away from mid
                     //               1 even distribution
                      //              2+amount closeness to mid
                      //              no negative allowed
{
	int repeat;
	double range, num, sign;

	range = (high-mid) > (mid-low) ? high-mid : mid-low;
	do {
	  	if (rrand() > 0.)
			sign = 1.;
		else  sign = -1.;
	  	num = mid + sign*(pow((rrand()+1.)*.5,tight)*range);
	} while(num < low || num > high);
	return(num);
}


void STGRAN2::addgrain()
{
	// typedef struct {float waveSampInc; float ampSampInc; float wavePhase; float ampPhase; float dur; float panR; float panL float currTime; bool isplaying;} Grain;


	Grain* newgrain = new Grain();
	newgrain-> waveSampInc = 0;
	newgrain-> ampSampInc = 0;
	newgrain-> ampPhase = 0;
	newgrain-> endTime = 0;
	newgrain-> panR = 0;
	newgrain-> panL = 0;
	newgrain-> currTime = 0;
	newgrain-> isplaying = false;

	grains->push_back(newgrain);
}

// set new parameters and turn on an idle grain
void STGRAN2::resetgrain(Grain* grain)
{

	if (!buffer->GetFull())
		return;

	float trans = (float)prob(transLow, transMid, transHigh, transTight);
	float rate = pow(2, trans / 12);
	float offset = rate - 1;
	float grainDurSamps = (float) prob(grainDurLow, grainDurMid, grainDurHigh, grainDurTight) * SR;
	float sampOffset = grainDurSamps * offset; // how many total samples the grain will deviate from the normal buffer movement

	grain->currTime = buffer->GetHead() - (int) floor(buffer->GetSize() / 2);


	if (abs(sampOffset) > buffer->GetSize()) // this grain cannot exist with size of the buffer
	{
		std::cout << "Grain offset too high!" <<"\n";
		return;
	}
	else if (abs(sampOffset) > buffer->GetSize() / 2)  // we can make this grain fit by starting it earlier or later
	{
		grain->currTime -= sampOffset;
	}

	float panR = (float) prob(panLow, panMid, panHigh, panTight);
	grain->waveSampInc = rate;
	grain->ampSampInc = ((float)grainEnvLen) / grainDurSamps;

	grain->isplaying = true;
	grain->ampPhase = 0;
	grain->panR = panR;
	grain->panL = 1 - panR; // separating these in RAM means fewer sample rate calculations
	grain->endTime = grainDurSamps + grain->currTime;
	//std::cout<<"sending grain with start time : "<< grain->currTime << " first sample : " << buffer->Get(grain->currTime) << "\n";
}

void STGRAN2::resetgraincounter()
{
	newGrainCounter = (int)round(grainRateSamps * prob(grainRateVarLow, grainRateVarMid, grainRateVarHigh, grainRateVarTight));
}

// determine the maximum grains we need total
int STGRAN2::calcgrainsrequired()
{
	return ceil(grainDurMid / (grainRateVarMid * grainRate)) + 1;
}


// update pfields
void STGRAN2::doupdate()
{
	double p[20];
	update(p, 20);
	amp =(float) p[2];

	grainDurLow = (double)p[8];
	grainDurMid = (double)p[9]; if (grainDurMid < grainDurLow) grainDurMid = grainDurLow;
	grainDurHigh = (double)p[10]; if (grainDurHigh < grainDurMid) grainDurHigh = grainDurMid;
	grainDurTight = (double)p[11];


	grainRateVarLow = (double)p[4];
	grainRateVarMid = (double)p[5]; if (grainRateVarMid < grainRateVarLow) grainRateVarMid = grainRateVarLow;
	grainRateVarHigh = (double)p[6]; if (grainRateVarHigh < grainRateVarMid) grainRateVarHigh = grainRateVarMid;
	grainRateVarTight = (double)p[7];

	transLow = (double)p[12];
	transMid = (double)p[13]; if (transMid < transLow) transMid = transLow;
	transHigh = (double)p[14]; if (transHigh < transMid) transHigh = transMid;
	transTight = (double)p[15];


	panLow = (double)p[16];
	panMid = (double)p[17]; if (panMid < panLow) panMid = panLow;
	panHigh = (double)p[18]; if (panHigh < panMid) panHigh = panMid;
	panTight = (double)p[19];

	// This needs to be redone at some point maybe
	// grainsRequired = calcgrainsrequired();
	// amp /= grainsRequired;

}

// Called by the scheduler for every time slice in which this instrument
// should run.  This is where the real work of the instrument is done.
int STGRAN2::run()
{
	//std::cout<<"new control block"<<"\n";
	float out[2];
	int samps = framesToRun();

	if (in == NULL) // first time, we need to allocate the buffer memory
		in = new float[RTBUFSAMPS*inputChannels()];

	rtgetin(in, this, samps);
	//int grainsCurrUsed = 0;
	for (int i = 0; i < samps; i++) {
		buffer->Append(in[i]);
		if (--branch <= 0) {doupdate();}

		out[0] = 0;
		out[1] = 0;
		

		for (size_t j = 0; j < grains->size(); j++)
		{
			Grain* currGrain = (*grains)[j];
			if (currGrain->isplaying)
			{
				if ((*currGrain).currTime > currGrain->endTime)
				{
					currGrain->isplaying = false;
				}
				else
				{
					// at some point, make your own interpolation
					float grainAmp = oscil(1, currGrain->ampSampInc, grainEnv, grainEnvLen, &((*currGrain).ampPhase));
					float grainOut = grainAmp * buffer->Get(currGrain->currTime);
					currGrain->currTime += currGrain->waveSampInc;
					// std::cout<<" outputing grain " << grainAmp << "\n";
					out[0] += grainOut * currGrain->panL;
					out[1] += grainOut * currGrain->panR;
				}
			}
			// this is not an else statement so a grain can be potentially stopped and restarted on the same frame

			if ((newGrainCounter == 0) && !currGrain->isplaying)
			{
				resetgraincounter();
				if (newGrainCounter > 0) // we don't allow two grains to be created on the same frame
					{resetgrain(currGrain);}
				else
					{newGrainCounter = 1;}

			}
		}

		// if all current grains are occupied, we skip this request for a new grain
		if (newGrainCounter == 0)
		{
			resetgraincounter();
			//std::cout << "skipping grain request" << "\n";
		}

		out[0] *= amp;
		out[1] *= amp;
		rtaddout(out);
		newGrainCounter--;
		increment();
	}
	return framesToRun();
}


// The scheduler calls this to create an instance of this instrument
// and to set up the bus-routing fields in the base Instrument class.
// This happens for every "note" in a score.

Instrument *makeSTGRAN2()
{
	STGRAN2 *inst = new STGRAN2();
	inst->set_bus_config("STGRAN2");

	return inst;
}


// The rtprofile introduces this instrument to the RTcmix core, and
// associates a script function name (in quotes below) with the instrument.
// This is the name the instrument goes by in a script.

#ifndef EMBEDDED
void rtprofile()
{
	RT_INTRO("STGRAN2", makeSTGRAN2);
}
#endif

