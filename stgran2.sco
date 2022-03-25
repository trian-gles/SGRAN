rtsetparams(44100, 2)
rtinput("clar.aiff")

load("./libSTGRAN2.so")

        /* Args:
                p0: inskip
                p1: dur
                p2: amp*
                p3: grainRate (seconds before new grain)
                p4: grainRateVarLow (fraction of grainRate actually used)*
                p5: grainRateVarMid*
                p6: grainRateVarHigh*
                p7: grainRateVarTight*
                p8: grainDurLow (length of grain in seconds)*
                p9: grainDurMid*
                p10: grainDurHigh*
                p11: grainDurTight*
                p12: transLow (semitones)*
                p13: transMid (semitones)*
                p14: transHigh (semitones)*
                p15: transTight*
		p16: panLow (0 - 1.0)*
		p17: panMid*
		p18: panHigh*
		p19: panTight*
                p20: grainEnv**
                p21: bufferSize=1 (size of the buffer used to choose new grains)*
		
		
		* may receive a table or some other pfield source
                ** must be passed as a pfield maketable.  
        */

inskip = 0
dur = 20
amp = maketable("line", 1000, 0, 0, 1, 1, 20, 1, 21, 0)

rate = .001
ratelo = 0.4
ratemid = 0.1
ratehi = 4
rateti = 3 

durlo = maketable("line", "nonorm", 1000, 0, 0.02, 1, 0.08)
durmid = maketable("line", "nonorm", 1000, 0, 0.08, 1, 0.4)
durhi = maketable("line", "nonorm", 1000, 0, 0.1, 1, 0.8)
durti = 0.6

translo = maketable("line", "nonorm", 1000, 0, -2, 1, -14)
transmid = 0
transhi = maketable("line", "nonorm", 1000, 0, 2, 1, 14)
transtight = maketable("line", "nonorm", 1000, 0, 2, 1, 0.1)

panlo = 0
panmid = 0.5
panhi = 1
panti = 0.6

env = maketable("window", 1000, "hanning")

buffer_size = makeLFO("square", 0.5, 0.02, 1)

STGRAN2(inskip, dur, 0.025 * amp, rate, ratelo, ratemid, ratehi, rateti, durlo, durmid, durhi, durti, 
translo, transmid, transhi, transhi, panlo, panmid, panhi, panti, env, buffer_size)


