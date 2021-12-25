#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include <math.h>
#include <PField.h>
#include <Instrument.h>
#include "SGRAN2.h"			  // declarations for this instrument class
#include <rt.h>
#include <rtdefs.h>
#include <iostream>
#include <vector>

// Construct an instance of this instrument and initialize some variables.
// Using an underbar as the first character of a data member is a nice
// convention to follow, but it's not necessary, of course.  It helps you
// to see at a glance whether you're looking at a local variable or a
// data member.

SGRAN2::SGRAN2()
{
}


// Destruct an instance of this instrument, freeing any memory you've allocated.

SGRAN2::~SGRAN2()
{
	for (size_t i = 0; i < grains->size(); i++)
	{
		delete (*grains)[i];
	}
	delete grains;
}


// Called by the scheduler to initialize the instrument. Things done here:
//   - read, store and check pfields
//   - set output file (or bus) pointer
//   - init instrument-specific things
// If there's an error here (like invalid pfields), call and return die() to
// report the error.  If you just want to warn the user and keep going,
// call warn() or rterror() with a message.

int SGRAN2::init(double p[], int n_args)
{		// store this for use in doupdate()

	/* Args:
		p0: inskip
		p1: dur
		p2: amp
		p3: grainRate
		p4: grainDur
		p5: freqLow
		p6: freqMid
		p7: freqHigh
		p8: freqTight
		p9: wavetable
		p10: grainEnv*/
	std::cout<<"setting basic params"<<"\n";
	int idk = rtsetoutput(p[0], p[1], this);
	grainEnvLen = 0;
	wavetableLen = 0;
	amp = p[2];
	grainDur = p[4];
	grainRate = p[3];

	freqLow = (double)p[5];
	freqMid = (double)p[6]; if (freqMid < freqLow) freqMid = freqLow;
	freqHigh = (double)p[7]; if (freqHigh < freqMid) freqHigh = freqMid;
	freqTight = (double)p[8];

	newGrainCounter = 0;
	grainRateSamps = round(grainRate * SR );

	// init tables
	std::cout<<"making tables"<<"\n";
	wavetable = (double *) getPFieldTable(9, &wavetableLen);
	grainEnv = (double *) getPFieldTable(10, &grainEnvLen);

	// make the needed grains, which have no values yet as they need to be created dynamically
	std::cout<<"setting up some basic grains"<<"\n";
	grains = new std::vector<Grain*>();
	for (int i = 0; i < calcgrainsrequired(); i++)
	{
		addgrain(0, 0, 0, 0, false);
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

int SGRAN2::configure()
{
	return 0;	// IMPORTANT: Return 0 on success, and -1 on failure.
}
// void addgrain(float sampInc; float freq; float dur; float pan; bool isplaying;);
//        void resetgrsain(Grain* grain);
//        int calcgrainsrequired();

double SGRAN2::prob(double low,double mid,double high,double tight)
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


//add a new grain to the grain list
void SGRAN2::addgrain(float waveSampInc,float ampSampInc, int dur, float pan, bool isplaying)
{
	// typedef struct {float waveSampInc; float ampSampInc; float wavePhase; float ampPhase; float dur; float pan; float currTime; bool isplaying;} Grain;


	Grain* newgrain = new Grain{waveSampInc,ampSampInc, 0, 0, dur, pan, 0, isplaying};
	grains->push_back(newgrain);
}

// set new parameters and turn on an idle grain
void SGRAN2::resetgrain(Grain* grain)
{
	float freq = (float)prob(freqLow, freqMid, freqHigh, freqTight);
	(*grain).waveSampInc = wavetableLen * freq / SR;
	(*grain).ampSampInc = grainEnvLen * (1 / grainDur) / SR;
	(*grain).currTime = 0;
	(*grain).isplaying = true;
	(*grain).wavePhase = 0;
	(*grain).ampPhase = 0;
	(*grain).dur = (int)round(grainDur * SR);
	std::cout<<"sending grain with freq : " << freq << " dur : " << grain->dur << "\n";

}

// determine the maximum grains we need total
int SGRAN2::calcgrainsrequired()
{
	return ceil(grainDur / grainRate) + 1;
}

// Called by the scheduler for every time slice in which this instrument
// should run.  This is where the real work of the instrument is done.

int SGRAN2::run()
{
	std::cout<<"new control block"<<"\n";
	float out[2];
	int totalCurrGrains;
	for (int i = 0; i < framesToRun(); i++) {
		std::cout<<"running frame "<< currentFrame() << "\n";
		float monoout = 0;
		totalCurrGrains = 0;
		if ((newGrainCounter == 0))
		{
			std::cout<<"we need a new grain!\n";
		}
		for (size_t j = 0; j < grains->size(); j++)
		{
			Grain* currGrain = (*grains)[j];
			if ((*currGrain).isplaying)
			{
				if (++(*currGrain).currTime > (*currGrain).dur)
				{
					(*currGrain).isplaying = false;
					std::cout<<"turning off grain \n";
				}
				else
				{
					// at some point, make your own interpolation
					float grainAmp = oscil(1, currGrain->ampSampInc, grainEnv, grainEnvLen, &((*currGrain).ampPhase));
					monoout += oscil(grainAmp,(*currGrain).waveSampInc, wavetable, wavetableLen, &((*currGrain).wavePhase));
					totalCurrGrains++; //there are smarter ways to do this!!!
				}
			}
			// this is not an else statement so a grain can be potentially stopped and restarted on the same frame

			if ((newGrainCounter == 0) && !(*currGrain).isplaying)
			{
				std::cout<<"resetting grain \n";
				resetgrain(currGrain);
				newGrainCounter = grainRateSamps;
			}
		}
		std::cout<<"total curr grains : "<<totalCurrGrains<<"\n";
		out[0] = monoout * amp / totalCurrGrains;
		out[1] = out[0];
		rtaddout(out);
		newGrainCounter--;
		increment();
	}

	// Return the number of frames we processed.

	return framesToRun();
}


// The scheduler calls this to create an instance of this instrument
// and to set up the bus-routing fields in the base Instrument class.
// This happens for every "note" in a score.

Instrument *makeSGRAN2()
{
	SGRAN2 *inst = new SGRAN2();
	inst->set_bus_config("SGRAN2");

	return inst;
}


// The rtprofile introduces this instrument to the RTcmix core, and
// associates a script function name (in quotes below) with the instrument.
// This is the name the instrument goes by in a script.

void rtprofile()
{
	RT_INTRO("SGRAN2", makeSGRAN2);
}


