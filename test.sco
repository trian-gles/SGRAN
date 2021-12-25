set_option("play = 0")

rtsetparams(44100, 2)
rtoutput("synth.wav")

load("./libSGRAN2.so")

/*		p0: inskip
                p1: dur
                p2: amp
                p3: freq
                p4: grainDur
                p5: grainRate
                p6: wavetable
                p7: grainEnv */

wave = maketable("wave", 1000, "sine")
env = src_env = maketable("window", 1000, "hanning")
SGRAN2(0, 3, 1000, 387.14, 0.4, .02, wave, env)


