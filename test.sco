set_option("play = 0")

rtsetparams(44100, 2)
rtoutput("synth.wav")

load("./libSIMPLEOSC.so")

wave = maketable("wave", 1000, "saw")
SIMPLEOSC(0, 3.5, 20000, 387.14, wave)


