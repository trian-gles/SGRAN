rtsetparams(44100, 2)
rtinput("clar.aiff")
set_option("clobber_on")
rtoutput("stex2.wav")


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
                p12: transLow (semitones)
                p13: transMid (semitones)
                p14: transHigh (semitones)
                p15: transTight
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

durlo = maketable("line", "nonorm", 1000, 0, 0.02, 1, 0.08)
durmid = maketable("line", "nonorm", 1000, 0, 0.08, 1, 0.4)
durhigh = maketable("line", "nonorm", 1000, 0, 0.1, 1, 0.8)


translo = maketable("line", "nonorm", 1000, 0, -2, 1, -14)
transhi = maketable("line", "nonorm", 1000, 0, 2, 1, 14)
transtight = maketable("line", "nonorm", 1000, 0, 2, 1, 0.1)

STGRAN2(0, 20, 6 * amp, 
//grain rate
.0005, 0.1, 3, 10, 0.1, 
//grain dur
durlo, durmid, durhigh, 0.1, 
//trans
translo, 0, transhi, 0.1, 
//pan
0, 0.5, 1, 0.4,
//wavetables
env)


