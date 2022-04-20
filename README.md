## About
Stochastic granular synthesizers for RTcmix version 4.5 based on Dr. Mara Helmuth's SGRANR instrument.  SGRAN2 works with periodic waveforms while SGRANR works with a provided soundfile or live input signal.
Both currently allow a limit of 1500 overlapping grains.  Upon exceeding this, the instrument will simply slow down the grain rate without any audible artifacts.

## Examples
### SGRAN2

https://user-images.githubusercontent.com/69212477/147691785-44a433a8-5641-47cd-8736-3a59bc73df5a.mp4

https://user-images.githubusercontent.com/69212477/147691891-53d72308-b080-4f00-8393-49e684ce733b.mp4

### STGRAN2

https://user-images.githubusercontent.com/69212477/148407993-227b08c9-a545-46cc-b253-875c567a8963.mp4

https://user-images.githubusercontent.com/69212477/148408034-002d62c7-b3ef-4b4c-9067-4aa3a63c321d.mp4



## Usage

Make sure the package.conf points to the appropriate RTcmix makefile.conf before building, then `make`

See the included scorefiles for example usage

### SGRAN2

Args:  
    - p0: inskip  
    - p1: dur  
    - p2: amp*  
    - p3: grainRateVarLow (seconds before new grain)*  
    - p4: grainRateVarMid*  
    - p5: grainRateVarHigh*  
    - p6: grainRateVarTight*  
    - p7: grainDurLow (length of grain in seconds)*  
    - p8: grainDurMid*  
    - p9: grainDurHigh*  
    - p10: grainDurTight*  
    - p11: freqLow (Hz)*  
    - p12: freqMid*  
    - p13: freqHigh*  
    - p14: freqTight*  
    - p15: panLow (0 - 1.0)*  
    - p16: panMid*  
    - p17: panHigh*  
    - p18: panTight*  
    - p19: wavetable**  
    - p20: grainEnv**  
    
\* may receive a reference to a pfield handle  
\*\* must receive a reference to a pfield maketable handle  


### STGRAN2

Args:  
    - p0: inskip  
    - p1: dur  
    - p2: amp*    
    - p3: grainRateVarLow (seconds before new grain)*  
    - p4: grainRateVarMid*  
    - p5: grainRateVarHigh*  
    - p6: grainRateVarTight*  
    - p7: grainDurLow (length of grain in seconds)*  
    - p8: grainDurMid*  
    - p9: grainDurHigh*  
    - p10: grainDurTight*  
    - p11: freqLow (Hz)*  
    - p12: freqMid*  
    - p13: freqHigh*  
    - p14: freqTight*  
    - p15: panLow (0 - 1.0)*  
    - p16: panMid*  
    - p17: panHigh*  
    - p18: panTight*  
    - p19: grainEnv**
    - p20: (optional) preferred size of the input buffer used in seconds, higher sizes will cause more smear of impulses or sudden changes.  1 by default*
    
\* may receive a reference to a pfield handle  
\*\* must receive a reference to a pfield maketable handle