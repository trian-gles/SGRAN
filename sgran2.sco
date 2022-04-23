rtsetparams(44100, 2)
load("./libSGRAN2.so")

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
                p11: freqLow (semitones)*
                p12: freqMid (semitones)*
                p13: freqHigh (semitones)*
                p14: freqTight*
		p15: panLow (0 - 1.0)*
		p16: panMid*
		p17: panHigh*
		p18: panTight*
                p19: wavetable*
                p20: grainEnv* 
		
		* p20(wavetable) and p21(grainEnv) must be passed as pfield make tables.  
		p2(amp) may receive a table or some other pfield source
        */
inskip = 0
dur = 25

amp = maketable("line", 1000, 0, 0, 8, 0.8, 16, 1, 17, 0)

ratelo = 0.00004
ratemid = maketable("line", "nonorm", 200, 0, 0.0008, 1, 0.00008)
ratehi = maketable("line", "nonorm", 200, 0, 0.004, 1, 0.0004)
rateti = maketable("line", "nonorm", 200, 0, 8, 1, 0.2)

durlo = 0.1
durmid = 0.5
durhi = 0.8
durti = 0.1

freqlo = maketable("line", "nonorm", 200, 0, 400, 1, 200)
freqmid = maketable("line", "nonorm", 200, 0, 430, 1, 350, 2, 600)
freqhi = maketable("line", "nonorm", 200, 0, 440, 1, 460, 2, 800)
freqti = maketable("line", "nonorm", 200, 0, 6, 1, 0.2)

panlo = 0
panmid = maketable("line", "nonorm", 200, 0, 0.1, 1, 0.1, 2, 0.5)
panhi = maketable("line", "nonorm", 200, 0, 0.2, 1, 0.5, 2, 1)
panti = 0.4

wave = maketable("wave", 1000, "square")
env = src_env = maketable("window", 1000, "hanning")

SGRAN2(inskip, dur, 800 * amp, ratelo, ratemid, ratehi, rateti, durlo, durmid, durhi, durti, 
freqlo, freqmid, freqhi, freqti, panlo, panmid, panhi, panti, wave, env)


