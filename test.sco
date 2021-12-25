set_option("play = 0")

rtsetparams(44100, 2)
rtoutput("synth.wav")

load("./libSGRAN2.so")

        /* Args:
                p0: inskip
                p1: dur
                p2: amp
                p3: grainRate
                p4: grainDurLow
                p5: grainDurMid
                p6: grainDurHigh
                p7: grainDurTight
                p8: freqLow
                p9: freqMid
                p10: freqHigh
                p11: freqTight
                p12: wavetable
                p13: grainEnv
        */
wave = maketable("wave", 1000, "sine")
env = src_env = maketable("window", 1000, "hanning")
SGRAN2(0, 5.5, 10000, 0.05, 0.01, 0.1, 0.6, 1, 220, 440, 880, 1, wave, env)


