Work in progress!

Based on Dr. Mara Helmuth's SGRANR instrument.

Args:
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
    p12: freqLow (Hz)
    p13: freqMid
    p14: freqHigh
    p15: freqTight
		p16: panLow (0 - 1.0)
		p17: panMid
		p18: panHigh
		p19: panTight
    p20: wavetable*
    p21: grainEnv* 
		
		* p20(wavetable) and p21(grainEnv) must be passed as pfield make tables.  
		p2(amp) may receive a table or some other pfield source
