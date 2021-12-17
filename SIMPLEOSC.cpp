#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include <math.h>
#include <PField.h>
#include <Instrument.h>
#include "SIMPLEOSC.h"			  // declarations for this instrument class
#include <rt.h>
#include <rtdefs.h>
#include <iostream>

// Construct an instance of this instrument and initialize some variables.
// Using an underbar as the first character of a data member is a nice
// convention to follow, but it's not necessary, of course.  It helps you
// to see at a glance whether you're looking at a local variable or a
// data member.

SIMPLEOSC::SIMPLEOSC() : theOscil(NULL)
{
}


// Destruct an instance of this instrument, freeing any memory you've allocated.

SIMPLEOSC::~SIMPLEOSC()
{
	delete theOscil;
}


// Called by the scheduler to initialize the instrument. Things done here:
//   - read, store and check pfields
//   - set output file (or bus) pointer
//   - init instrument-specific things
// If there's an error here (like invalid pfields), call and return die() to
// report the error.  If you just want to warn the user and keep going,
// call warn() or rterror() with a message.

int SIMPLEOSC::init(double p[], int n_args)
{		// store this for use in doupdate()

	/* Args:
		p0: inskip
		p1: dur
		p2: amp
		p3: freq
		p4: wavetable */
	int idk = rtsetoutput(p[0], p[1], this);
	int amptablelen = 0;
	int wavetablelen = 0;

	// init wavetable
	std::cout << "initializing wavetable\n";
	double* wavetable = (double *) getPFieldTable(4, &wavetablelen);
	std::cout << "wavetable length:" << wavetablelen << "\n";
	theOscil = new Ooscili(SR, p[3], wavetable, wavetablelen);

	return nSamps();
}


// For non-interactive (script-driven) sessions, the constructor and init()
// for every instrument in the script are called before any of them runs.
// By contrast, configure() is called right before the instrument begins
// playing.  If we were to allocate memory at init time, then all notes in
// the score would allocate memory then, resulting in a potentially excessive
// memory footprint.  So this is the place to allocate any substantial amounts
// of memory you might be using.

int SIMPLEOSC::configure()
{
	return 0;	// IMPORTANT: Return 0 on success, and -1 on failure.
}



// Called by the scheduler for every time slice in which this instrument
// should run.  This is where the real work of the instrument is done.

int SIMPLEOSC::run()
{
	float out[2];

	for (int i = 0; i < framesToRun(); i++) {
		double p[4];
		float out[2];
		int cf = currentFrame();
		update(p, 4, 1 << 2);
		float amp = p[2];
		out[0] = theOscil->next() * amp;
		out[1] = out[0];
		rtaddout(out);
		increment();
	}

	// Return the number of frames we processed.

	return framesToRun();
}


// The scheduler calls this to create an instance of this instrument
// and to set up the bus-routing fields in the base Instrument class.
// This happens for every "note" in a score.

Instrument *makeSIMPLEOSC()
{
	SIMPLEOSC *inst = new SIMPLEOSC();
	inst->set_bus_config("SIMPLEOSC");

	return inst;
}


// The rtprofile introduces this instrument to the RTcmix core, and
// associates a script function name (in quotes below) with the instrument.
// This is the name the instrument goes by in a script.

void rtprofile()
{
	RT_INTRO("SIMPLEOSC", makeSIMPLEOSC);
}


