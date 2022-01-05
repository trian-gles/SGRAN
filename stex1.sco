rtsetparams(44100, 2)
rtinput("clar.aiff")
set_option("clobber_on")
rtoutput("stex1.wav")


load("./libSTGRAN2.so")

        /* Args:
                p0: inskip
                p1: dur
                p2: amp*
                p3: grainRate (seconds before new grain)
                p4: grainRateVarLow (fraction of grainRate actually used)
                p5: grainRateVarMid
                p6: grainRateVarHigh
                p7: grainRateVarTight
                p8: grainDurLow (length of grain in seconds)
                p9: grainDurMid
                p10: grainDurHigh
                p11: grainDurTight
                p12: freqLow (Hz)
                p13: freqMid
                p14: freqHigh
                p15: freqTight
		p16: panLow (0 - 1.0)
		p17: panMid
		p18: panHigh
		p19: panTight
                p20: grainEnv* 
		
		* p20(grainEnv) must be passed as a pfield maketable.  
		p2(amp) may receive a table or some other pfield source
        */

env = maketable("window", 1000, "hanning")
amp = maketable("line", 1000, 0, 0, 1, 1, 20, 1, 21, 0)

STGRAN2(0, 20, 6 * amp, 
//grain rate
.001, 0.1, 3, 10, 4, 
//grain dur
0.2, 0.5, 0.8, 0.1, 
//trans
.5, 1, 1.5, 0.2, 
//pan
0, 0.5, 1, 0.4,
//wavetables
env)


