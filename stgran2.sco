rtsetparams(44100, 2)
rtinput("clar.aiff")

load("./libSTGRAN2.so")

        /* Args:
                p0: outskip
                p1: inskip
                p2: dur
                p3: amp*
                p4: grainRateVarLow (seconds before new grain)*
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

outskip = 0
inskip = 60
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
transti = maketable("line", "nonorm", 1000, 0, 2, 1, 0.1)

panlo = 0
panmid = 0.5
panhi = 1
panti = 0.6

env = maketable("window", 1000, "hanning")

buffer_size = makeLFO("square", 0.5, 0.02, 1)

STGRAN2(outskip, inskip, dur, 0.2 * amp, ratelo, ratemid, ratehi, rateti, durlo, durmid, durhi, durti, 
translo, transmid, transhi, transti, panlo, panmid, panhi, panti, env, buffer_size)


