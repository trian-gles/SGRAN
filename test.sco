set_option("play = 0")

rtsetparams(44100, 2)
rtoutput("synth.wav")

load("./libSGRAN2.so")

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
wave = maketable("wave", 1000, "sine")
env = src_env = maketable("window", 1000, "hanning")
SGRAN2(0, 3.5, 10000, 0.001, 0.04, 220, 440, 880, 0.3, wave, env)


