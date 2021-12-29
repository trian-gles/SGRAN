rtsetparams(44100, 2)
rtoutput("ex2.wav")
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

wave = maketable("wave", 1000, "square")
env = src_env = maketable("window", 1000, "hanning")
amp = maketable("line", 1000, 0, 0, 8, 0.8, 16, 1, 17, 0)


rate_low = 0.00004
rate_mid = maketable("line", "nonorm", 200, 0, 0.0008, 1, 0.00008)
rate_high = maketable("line", "nonorm", 200, 0, 0.004, 1, 0.0004)
rate_tight = maketable("line", "nonorm", 200, 0, 8, 1, 0.2)




freq_low = maketable("line", "nonorm", 200, 0, 400, 1, 200)
freq_mid = maketable("line", "nonorm", 200, 0, 430, 1, 350, 2, 600)
freq_high = maketable("line", "nonorm", 200, 0, 440, 1, 460, 2, 800)
freq_tight = maketable("line", "nonorm", 200, 0, 6, 1, 0.2)



pan_low = 0
pan_mid = maketable("line", "nonorm", 200, 0, 0.1, 1, 0.1, 2, 0.5)
pan_high = maketable("line", "nonorm", 200, 0, 0.2, 1, 0.5, 2, 1)

pan_tight = 0.4

SGRAN2(0, 25, 200000 * amp, 
//grain rate
1, rate_low, rate_mid, rate_high, rate_tight, 
//grain dur
0.1, 0.5, 0.8, 0.1, 
//freq
freq_low, freq_mid, freq_high, freq_tight, 
//pan
pan_low, pan_mid, pan_high, pan_tight,
//wavetables
wave, env)


