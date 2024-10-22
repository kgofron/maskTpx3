#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1) // Ensure proper alignment for BMP header

typedef struct {
    uint16_t bfType;      // BMP file type
    uint32_t bfSize;      // Size of the BMP file
    uint16_t bfReserved1; // Reserved
    uint16_t bfReserved2; // Reserved
    uint32_t bfOffBits;   // Offset to the pixel data
} BMPFileHeader;

typedef struct {
    uint32_t biSize;          // Size of this header
    int32_t  biWidth;         // Width of the bitmap in pixels
    int32_t  biHeight;        // Height of the bitmap in pixels
    uint16_t biPlanes;        // Number of color planes (must be 1)
    uint16_t biBitCount;      // Bits per pixel
    uint32_t biCompression;    // Compression method
    uint32_t biSizeImage;     // Size of the image data
    int32_t  biXPelsPerMeter;  // Horizontal resolution
    int32_t  biYPelsPerMeter;  // Vertical resolution
    uint32_t biClrUsed;       // Number of colors in the palette
    uint32_t biClrImportant;   // Important colors
} BMPInfoHeader;

#pragma pack(pop)

void create_binary_bmp(const char *filename, int width, int height) {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // Set up the file header
    fileHeader.bfType = 0x4D42; // 'BM'
    fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + width * height;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    // Set up the info header
    infoHeader.biSize = sizeof(BMPInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 1; // 1 bit per pixel for binary image
    infoHeader.biCompression = 0; // No compression
    infoHeader.biSizeImage = (width + 31) / 32 * 4 * height; // Row size padded to 32 bits
    infoHeader.biXPelsPerMeter = 0; // Resolution (not important)
    infoHeader.biYPelsPerMeter = 0; // Resolution (not important)
    infoHeader.biClrUsed = 2; // 2 colors for binary image
    infoHeader.biClrImportant = 0;

    // Open file for writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error creating file");
        return;
    }

    // Write headers
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Write the color palette (black and white)
    fputc(0, file); // Blue for black
    fputc(0, file); // Green for black
    fputc(0, file); // Red for black
    fputc(0, file); // Reserved for black

    fputc(255, file); // Blue for white
    fputc(255, file); // Green for white
    fputc(255, file); // Red for white
    fputc(0, file); // Reserved for white

    // Write pixel data (checkerboard pattern)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Alternate between black and white
            uint8_t pixelValue = ((x / 8 + y / 8) % 2) == 0 ? 1 : 0; // Checkerboard pattern
            fputc(pixelValue, file);
        }
        // Pad each row to 4 bytes
        for (int padding = 0; padding < (4 - (width % 4)) % 4; padding++) {
            fputc(0, file);
        }
    }

    // Close the file
    fclose(file);
    printf("Binary BMP image created: %s\n", filename);
}

int main() {
    int width = 100;
    int height = 100;
    create_binary_bmp("binary_mask.bmp", width, height);
    return 0;
}
