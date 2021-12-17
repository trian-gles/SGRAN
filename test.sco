set_option("play = 0")

rtsetparams(44100, 2)
rtoutput("synth.wav")

load("./libSIMPLEOSC.so")
load("WAVETABLE")

wave = maketable("wave", 1000, "saw")
amp = maketable("line", 1000, 0, 1, 1, 0)
SIMPLEOSC(0, 3.5, amp * 20000, 387.14, wave)


