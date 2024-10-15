# Compile .cpp
* g++ write_byte.cpp -o write_byte
* g++ writeMask.cpp -o writeMask
* gcc writeMask.c -o writeMask

## copy file to buffer
* gcc -o file_copy_buffer file_copy_buffer.c
* ./file_copy_buffer source.txt destination.txt


## Chip DAC adjustments
* medium power -> high power
```
[Chip0]
Ibias_CP_PLL:128
Ibias_DiscS1_OFF:8
Ibias_DiscS1_ON:64 -> 128
Ibias_DiscS2_OFF:8
Ibias_DiscS2_ON:64 -> 128
Ibias_Ikrum:10
Ibias_PixelDAC:81 -> 149
Ibias_Preamp_OFF:8
Ibias_Preamp_ON:64 -> 128
Ibias_TPbufferIn:128
Ibias_TPbufferOut:128
PLL_Vcntrl:128
VPreamp_NCAS:128
VTP_coarse:128
VTP_fine:256
Vfbk:128
Vthreshold_coarse:6
Vthreshold_fine:379 -> 396
```

