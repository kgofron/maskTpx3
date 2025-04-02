#!/bin/bash

# Computes x,y chip coordinates of masked pixels in chip coordinates
gcc -o check_bit check_bit.c
./check_bit  Eq_neg_cfg1.bpc

# OUTPUT
#
#Position	Value	Chip	X	Y
#--------	-----	----	-	-
#18291		0x01	0	115	71
#18292		0x01	0	116	71
#24812		0x01	0	236	96

