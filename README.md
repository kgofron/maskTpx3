# maskTpx3
Detector mask for TimePix3 ROI

Testing with the detector to identify masking bit
* compile 
	* g++ writeMask.cpp -o writeMask
	* cp ~/Documents/Detector/TimePix3/Venus/maskPixel/writeBinary mask.bpc
	* ./writeMask

## Identify chips: 
* BL10, and Bl7 chips are rotated left.
* UP
```
2 | 1
3 | 0
```
The mask starts from lower right corner of chip0.

## LEFT-> BL10:
```
1 | 0
2 | 3
```

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

* Chip index
Width=W=256 pel
ChipNo	Mask[k] index
0	i - W + W*j
1	W*W + (2*W-1 - i) + (2*W-1 - j)*W
2	2*W*W + (W-1 - i) + (2*W-1 - j)*W
3	3*W*W + i + W*j

Image (i,j)
0,  256<=i<512; 0<=j<256
1,  256<=i<512; 256<=j<512
2,  0<=i<256; 256<=j<512
3,  0<=i<256; 0<=j<256

Mask(k)->Image(i,j)
chip=k/Width
i=(k-chip*W)/W
j=(k-chip*W)%W

For chips 1,2, need to reverse indexes in the global coordinates. These chips are mask-index counted from upper-right corner in the mask.
Chip internal (i,j) for chip 1,2: i->W-i, j->W-j
Global index
0: W+i,j
1: 2W-i,2W-j
2: W-i,2W-j
3: i,j


## LEFT

```
"Layout" : {
  "DetectorOrientation" : "LEFT",
  "Original" : {
    "Width" : 512,
    "Height" : 512,
    "Chips" : [ {
      "Chip" : 0,
      "X" : 256,
      "Y" : 0,
      "Orientation" : "LtRBtT"
    }, {
      "Chip" : 1,
      "X" : 256,
      "Y" : 256,
      "Orientation" : "RtLTtB"
    }, {
      "Chip" : 2,
      "X" : 0,
      "Y" : 256,
      "Orientation" : "RtLTtB"
    }, {
      "Chip" : 3,
      "X" : 0,
      "Y" : 0,
      "Orientation" : "LtRBtT"
    } ]
  },
  "Rotated" : {
    "Chips" : [ {
      "Chip" : 0,
      "X" : 256,
      "Y" : 256,
      "Orientation" : "BtTRtL"
    }, {
      "Chip" : 1,
      "X" : 0,
      "Y" : 256,
      "Orientation" : "TtBLtR"
    }, {
      "Chip" : 2,
      "X" : 0,
      "Y" : 0,
      "Orientation" : "TtBLtR"
    }, {
      "Chip" : 3,
      "X" : 256,
      "Y" : 0,
      "Orientation" : "BtTRtL"
    } ],
    "Width" : 512,
    "Height" : 512
  }
```