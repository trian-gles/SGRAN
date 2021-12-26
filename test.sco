set_option("play = 0")

rtsetparams(44100, 2)
rtoutput("synth.wav")

load("./libSGRAN2.so")

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
        */

wave = maketable("wave", 1000, "sine")
env = src_env = maketable("window", 1000, "hanning")

SGRAN2(0, 5.5, 20000, 
//grain rate
0.02, 0.05, 0.04, 0.6, 4, 
//grain dur
0.01, 0.1, 0.6, 0.1, 
//freq
220, 440, 880, 0.07, 
//pan
0, 0.5, 1, 0.4,
//wavetables
wave, env)


