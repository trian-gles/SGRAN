rtsetparams(44100, 2)
rtinput("clar.aiff")
load("./libSTGRAN2.so")

        /* Args:
                p0: outskip
                p1: inskip
                p2: dur
                p3: amp*
                p4: rateLow (seconds before new grain)*
                p5: rateMid*
                p6: rateHigh*
                p7: rateTight*
                p8: durLow (length of grain in seconds)*
                p9: durMid*
                p10: durHigh*
                p11: durTight*
                p12: transLow (oct.pc)*
                p13: transMid (oct.pc)*
                p14: transHigh (oct.pc)*
                p15: transTight*
		p16: panLow (0 - 1.0)*
		p17: panMid*
		p18: panHigh*
		p19: panTight*
                p20: grainEnv**
                p21: bufferSize=1 (size of the buffer used to choose new grains)*
		p22: maxGrains=1500
		p23: inchan=0
		
		* may receive a table or some other pfield source
                ** must be passed as a pfield maketable.  
        */

outskip = 0
inskip = 60
dur = 80
amp = maketable("line", 1000, 0, 0, 1, 1, 20, 1, 21, 0)

ratelo = 0.00004
ratemid = 0.0001
ratehi = .004
rateti = 0.6 

durlo = 0.02
durmid =0.08
durhi = 0.3
durti = 1

translo = -1.00
transmid = 0
transhi = 1.00
transti = 2

panlo = 0
panmid = 0.5
panhi = 1
panti = 0.6

env = maketable("window", 1000, "hanning")

buffer_size = makeLFO("square", 0.5, 0.02, 1)

STGRAN2(outskip, inskip, dur, 0.2 * amp, ratelo, ratemid, ratehi, rateti, durlo, durmid, durhi, durti, 
translo, transmid, transhi, transti, panlo, panmid, panhi, panti, env, buffer_size)


