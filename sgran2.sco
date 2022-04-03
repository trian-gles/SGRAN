rtsetparams(44100, 2)
load("./libSGRAN2.so")

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
                p12: freqLow (semitones)*
                p13: freqMid (semitones)*
                p14: freqHigh (semitones)*
                p15: freqTight*
		p16: panLow (0 - 1.0)*
		p17: panMid*
		p18: panHigh*
		p19: panTight*
                p20: wavetable*
                p21: grainEnv* 
                p22: cores
		
		* p20(wavetable) and p21(grainEnv) must be passed as pfield make tables.  
		p2(amp) may receive a table or some other pfield source
        */
inskip = 0
dur = 25

amp = maketable("line", 1000, 0, 0, 8, 0.8, 16, 1, 17, 0)

rate = 1
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

cores = 1

SGRAN2(inskip, dur, 300 * amp, rate, ratelo, ratemid, ratehi, rateti, durlo, durmid, durhi, durti, 
freqlo, freqmid, freqhi, freqti, panlo, panmid, panhi, panti, wave, env, cores)


