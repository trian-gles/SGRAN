rtsetparams(44100, 2)
rtinput("clar.aiff")

load("./libSTGRAN2.so")

        /* Args:
                p0: inskip
                p1: dur
                p2: amp*
                p3: grainRateVarLow (seconds before new grain)*
                p4: grainRateVarMid*
                p5: grainRateVarHigh*
                p6: grainRateVarTight*
                p7: grainDurLow (length of grain in seconds)*
                p8: grainDurMid*
                p9: grainDurHigh*
                p10: grainDurTight*
                p11: transLow (semitones)*
                p12: transMid (semitones)*
                p13: transHigh (semitones)*
                p14: transTight*
		p15: panLow (0 - 1.0)*
		p16: panMid*
		p17: panHigh*
		p18: panTight*
                p19: grainEnv**
                p20: bufferSize=1 (size of the buffer used to choose new grains)*
		
		
		* may receive a table or some other pfield source
                ** must be passed as a pfield maketable.  
        */

inskip = 0
dur = 20
amp = maketable("line", 1000, 0, 0, 1, 1, 20, 1, 21, 0)

ratelo = 0.0004
ratemid = 0.001
ratehi = .004
rateti = 3 

durlo = maketable("line", "nonorm", 1000, 0, 0.02, 1, 0.08)
durmid = maketable("line", "nonorm", 1000, 0, 0.08, 1, 0.4)
durhi = maketable("line", "nonorm", 1000, 0, 0.1, 1, 0.8)
durti = 0.6

translo = -1.00
transmid = 0
transhi = 1.00
transtight = maketable("line", "nonorm", 1000, 0, 2, 1, 0.1)

panlo = 0
panmid = 0.5
panhi = 1
panti = 0.6

env = maketable("window", 1000, "hanning")

buffer_size = makeLFO("square", 0.5, 0.02, 1)

STGRAN2(inskip, dur, 0.2 * amp, ratelo, ratemid, ratehi, rateti, durlo, durmid, durhi, durti, 
translo, transmid, transhi, transhi, panlo, panmid, panhi, panti, env, buffer_size)


