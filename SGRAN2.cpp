#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include <math.h>
#include <algorithm>
#include <PField.h>
#include <Instrument.h>
#include "SGRAN2.h"
#include <rt.h>
#include <rtdefs.h>
#include <iostream>
#include <vector>

#define MAXGRAINS 1500

SGRAN2::SGRAN2() : branch(0)
{
}



SGRAN2::~SGRAN2()
{
	if (!configured)
		return;
	for (size_t i = 0; i < grains->size(); i++)
	{
		delete (*grains)[i];
	}
	delete grains;
}


int SGRAN2::init(double p[], int n_args)
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
		p12: freqLow
		p13: freqMid
		p14: freqHigh
		p15: freqTight
		p16: panLow
		p17: panMid
		p18: panHigh
		p19: panTight
		p20: wavetable
		p21: grainEnv
		p22: grainLimit=1500
	*/
	if (rtsetoutput(p[0], p[1], this) == -1)
		return DONT_SCHEDULE;

	if (outputChannels() > 2)
	      return die("SGRAN2", "Output must be mono or stereo.");

	if (n_args < 22)
		return die("SGRAN2", "22 arguments are required");
	else if (n_args > 23)
		return die("SGRAN2", "too many arguments");
	grainEnvLen = 0;
	wavetableLen = 0;
	amp = p[2];

	grainRate = p[3];
	newGrainCounter = 0;
	grainRateSamps = round(grainRate * SR);

	// init tables
	wavetable = (double *) getPFieldTable(20, &wavetableLen);
	grainEnv = (double *) getPFieldTable(21, &grainEnvLen);

	if (n_args > 22)
	{
		grainLimit = p[22];
		if (grainLimit > MAXGRAINS)
		{
			rtcmix_advise("STGRAN2", "user provided max grains exceeds limit, lowering to 1500");
			grainLimit = MAXGRAINS;
		}
			
	}
	else
		grainLimit = MAXGRAINS;

	return nSamps();
}



int SGRAN2::configure()
{
	std::cout << "running config with grainLimit " << grainLimit << "\n";
	// make the needed grains, which have no values yet as they need to be set dynamically
	grains = new std::vector<Grain*>();
	// maybe make the maximum grain value a non-pfield enabled parameter

	for (int i = 0; i < grainLimit; i++)
	{
		std::cout << "pushing grain \n";
		grains->push_back(new Grain());
	}

	configured = true;

	return 0;	// IMPORTANT: Return 0 on success, and -1 on failure.
}

double SGRAN2::prob(double low,double mid,double high,double tight)
        // Returns a value within a range close to a preferred value
                    // tightness: 0 max away from mid
                     //               1 even distribution
                      //              2+amount closeness to mid
                      //              no negative allowed
{
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

// set new parameters and turn on an idle grain
void SGRAN2::resetgrain(Grain* grain)
{
	float freq = cpsmidi((float)prob(midicps(freqLow), midicps(freqMid), midicps(freqHigh), freqTight));
	float grainDurSamps = (float) prob(grainDurLow, grainDurMid, grainDurHigh, grainDurTight) * SR;
	float panR = (float) prob(panLow, panMid, panHigh, panTight);
	grain->waveSampInc = wavetableLen * freq / SR;
	grain->ampSampInc = ((float)grainEnvLen) / grainDurSamps;
	grain->currTime = 0;
	grain->isplaying = true;
	grain->wavePhase = 0;
	grain->ampPhase = 0;
	grain->panR = panR;
	grain->panL = 1 - panR; // separating these in RAM means fewer sample rate calculations
	(*grain).dur = (int)round(grainDurSamps);
	//std::cout<<"sending grain with freq : " << freq << " dur : " << grain->dur << " panR " << panR << "\n";

}

void SGRAN2::resetgraincounter()
{
	newGrainCounter = (int)round(grainRateSamps * prob(grainRateVarLow, grainRateVarMid, grainRateVarHigh, grainRateVarTight));
}

// determine the maximum grains we need total.  Needs to be redone using ZE CALCULUS
int SGRAN2::calcgrainsrequired()
{
	return ceil(grainDurMid / (grainRateVarMid * grainRate)) + 1;
}


// update pfields
void SGRAN2::doupdate()
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

	freqLow = (double)p[12];
	freqMid = (double)p[13]; if (freqMid < freqLow) freqMid = freqLow;
	freqHigh = (double)p[14]; if (freqHigh < freqMid) freqHigh = freqMid;
	freqTight = (double)p[15];


	panLow = (double)p[16];
	panMid = (double)p[17]; if (panMid < panLow) panMid = panLow;
	panHigh = (double)p[18]; if (panHigh < panMid) panHigh = panMid;
	panTight = (double)p[19];

	// Ouput amplitude will eventually be adjusted here
	// grainsRequired = calcgrainsrequired();
	// amp /= grainsRequired;

}


int SGRAN2::run()
{
	float out[2];
	for (int i = 0; i < framesToRun(); i++) {
		if (--branch <= 0)
		{
		doupdate();
		branch = getSkip();
		}

		out[0] = 0;
		out[1] = 0;
		for (size_t j = 0; j < grains->size(); j++)
		{
			Grain* currGrain = (*grains)[j];
			if (currGrain->isplaying)
			{
				if (++(*currGrain).currTime > currGrain->dur)
				{
					currGrain->isplaying = false;
				}
				else
				{
					// should include an interpolation option at some point
					float grainAmp = oscili(1, currGrain->ampSampInc, grainEnv, grainEnvLen, &((*currGrain).ampPhase));
					float grainOut = oscili(grainAmp,currGrain->waveSampInc, wavetable, wavetableLen, &((*currGrain).wavePhase));
					out[0] += grainOut * currGrain->panL;
					out[1] += grainOut * currGrain->panR;
				}
			}
			// this is not an else statement so a grain can be potentially stopped and restarted on the same frame

			if ((newGrainCounter <= 0) && !currGrain->isplaying)
			{
				resetgraincounter();
				if (newGrainCounter > 0) // we don't allow two grains to be create o
					{resetgrain(currGrain);}
				else
					{newGrainCounter = 1;}

			}
		}

		// if all current grains are occupied, we skip this request for a new grain
		if (newGrainCounter <= 0)
		{
			resetgraincounter();
		}

		out[0] *= amp;
		out[1] *= amp;
		rtaddout(out);
		newGrainCounter--;
		increment();
	}

	// Return the number of frames we processed.

	return framesToRun();
}


Instrument *makeSGRAN2()
{
	SGRAN2 *inst = new SGRAN2();
	inst->set_bus_config("SGRAN2");

	return inst;
}

#ifndef EMBEDDED
void rtprofile()
{
	RT_INTRO("SGRAN2", makeSGRAN2);
}
#endif

