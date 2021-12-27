rtsetparams(44100, 2)
rtoutput("test.wav")
load("./libSGRAN2.so")

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
                p20: wavetable*
                p21: grainEnv* 
		
		* p20(wavetable) and p21(grainEnv) must be passed as pfield make tables.  
		p2(amp) may receive a table or some other pfield source
        */

wave = maketable("wave", 1000, "saw")
env = src_env = maketable("window", 1000, "hanning")
amp = maketable("line", 1000, 0, 0, 1, 1, 2, 0)

SGRAN2(0, 10, 150000 * amp, 
//grain rate
0.01, 0.004, 0.008, 0.04, 4, 
//grain dur
0.1, 0.5, 0.8, 0.1, 
//freq
400, 430, 440, 6, 
//pan
0, 0.8, 1, 0.4,
//wavetables
wave, env)


