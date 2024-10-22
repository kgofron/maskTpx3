#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>

void create_binary_tiff(const char *filename, int width, int height) {
    TIFF *tif = TIFFOpen(filename, "w");
    if (!tif) {
        perror("Could not open TIFF file for writing");
        return;
    }

    // Set TIFF properties
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK); // Black and white

    // Allocate memory for a single row of pixels
    uint8_t *row = (uint8_t *)malloc(width * sizeof(uint8_t));
    if (!row) {
        perror("Could not allocate memory for row");
        TIFFClose(tif);
        return;
    }

    // Write pixel data (checkerboard pattern)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Alternate between black and white
            row[x] = ((x / 8 + y / 8) % 2) == 0 ? 0 : 1; // 0 for black, 1 for white
        }
        // Write the row to the TIFF file
        TIFFWriteScanline(tif, row, y, 0);
    }

    // Clean up
    free(row);
    TIFFClose(tif);
    printf("Binary TIFF image created: %s\n", filename);
}

int main() {
    int width = 100;
    int height = 100;
    create_binary_tiff("binary_mask.tiff", width, height);
    return 0;
}
