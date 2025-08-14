#!/bin/bash

# Computes x,y chip coordinates of masked pixels in chip coordinates
gcc -o check_bit check_bit.c
./check_bit  Eq_neg_cfg1.bpc

# OUTPUT

# File: ~/Documents/Detector/TimePix3/41000039Venus/usb/high-power/eq-accos-02.bpc
# Orientation=LEFT
# ------------------------------------------------------------------------
# #	Position	Value	Chip	x	y	X	Y
# -	--------	-----	----	-	-	-	-
# 1	964		0x1F	0	196	3	508	59
# 2	3569		0x1F	0	241	13	498	14
# 3	3570		0x1F	0	242	13	498	13
# 4	3968		0x01	0	128	15	496	127
