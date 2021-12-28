Stochastic granular synthesizer based on Dr. Mara Helmuth's SGRANR instrument.
Currently allows a limit of 1500 overlapping grains.  Don't worry about this as the instrument will simply slow down the rate without any audible artifacts if you exceed this.


Args:
    - p0: inskip
    - p1: dur
    - p2: amp*
    - p3: grainRate (seconds before new grain)
    - p4: grainRateVarLow (fraction of grainRate actually used)*
    - p5: grainRateVarMid*
    - p6: grainRateVarHigh*
    - p7: grainRateVarTight*
    - p8: grainDurLow (length of grain in seconds)*
    - p9: grainDurMid*
    - p10: grainDurHigh*
    - p11: grainDurTight*
    - p12: freqLow (Hz)*
    - p13: freqMid*
    - p14: freqHigh*
    - p15: freqTight*
    - p16: panLow (0 - 1.0)*
    - p17: panMid*
    - p18: panHigh*
    - p19: panTight*
    - p20: wavetable**
    - p21: grainEnv**
		
\* may receive a reference to a pfield handle
\*\* must receive a reference to a pfield maketable handle
