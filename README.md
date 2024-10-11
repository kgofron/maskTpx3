# maskTpx3
Detector mask for TimePix3 ROI

Testing with the detector to identify masking bit
* compile 
	* g++ writeMask.cpp -o writeMask
	* cp ~/Documents/Detector/TimePix3/Venus/maskPixel/writeBinary mask.bpc
	* ./writeMask

## Identify chips
```
2 | 1
3 | 0
```
The mask starts from lower right corner of chip0.
 
## Chip orientation
* Chip0 - bottom right-corner
* Chip1 - top right-corner
* chip2 - top left-corner
* chip3 - bottom left-corner

* Masking orientation in .bpc file
```
5 | 3
6 | 4
8 | 2
7 | 1
```
