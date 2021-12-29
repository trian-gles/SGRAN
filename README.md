## about
Stochastic granular synthesizer for RTcmix version 4.5 based on Dr. Mara Helmuth's SGRANR instrument.
Currently allows a limit of 1500 overlapping grains.  Upon exceeding this, the instrument will simply slow down the rate without any audible artifacts if you exceed this.

## examples

https://user-images.githubusercontent.com/69212477/147691785-44a433a8-5641-47cd-8736-3a59bc73df5a.mp4


https://user-images.githubusercontent.com/69212477/147691891-53d72308-b080-4f00-8393-49e684ce733b.mp4


## usage

Make sure the package.conf points to the appropriate makefile.conf before building.

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
