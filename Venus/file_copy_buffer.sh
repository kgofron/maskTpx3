#!/bin/bash
g++ -o file_copy_buffer file_copy_buffer.c
# Venus
../copyMask/file_copy_buffer Sophy_PixelConfig_after_Equalization.bpc maskV.bpc

# Venus ASI
../copyMask/file_copy_buffer Eq_neg_cfg.bpc maskASI.bpc

