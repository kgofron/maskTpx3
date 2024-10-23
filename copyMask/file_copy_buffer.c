#include <stdio.h>
#include <stdlib.h>

#define XCHIPS 2  // Set the number of horizontal chips
#define YCHIPS 2  // Set the number of vertical chips
#define X_PEL  256  // Set the number of rows in chip
#define Y_PEL  256  // Set the number of columns in chip
#define ROWS (YCHIPS * Y_PEL)  // Set the number of horizontal pixels
#define COLS (XCHIPS * X_PEL)  // Set the number of vertical pixels


int find_masked_positions(char *buffer, long fileSize, int N) {
    int i;
    int nMaskedPel = 0;
    for (int i = 0; i < fileSize; i++) {
        // Check if the bit at position N is set to 1
        if (buffer[i] & (1 << N)) {
            nMaskedPel += 1;
    //        printf("Bit i=%d, nMaskedPel=%d\n", i, nMaskedPel);
        }
    }
    return nMaskedPel;
}

// Mask reset to 0 (active pel) or 1 (disabled pel)
void maskReset(int *buf, int OnOff) {
//    printf("OnOff=%d\n", OnOff);
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            buf[i*ROWS + j] = OnOff;
        }
    }
}

// Mask a nXsize x nYsize rectangle, or pint (nXsize=nYsize=1), or line respectivly
void maskRectangle(int *buf, int nX,int nXsize, int nY, int nYsize, int OnOff) {
    for (int i = nX; i < nX+nXsize; ++i) {
        if ( i < ROWS ) {
            for (int j = nY; j < nY + nYsize; ++j) {
                if (j < COLS) {
    //                buf[i*ROWS + j] |= (1 << 0);
                    if (OnOff) {
                        buf[i*ROWS + j] |= (1 << 0);    // set mask bit to 1
                    }
                    else {
                        buf[i*ROWS + j] &= ~(1 << 0);   // set mask bit 0 0
                    }
                }
            }
        }
    }
}

// Mask a circle: OnOff=1 set bit to 1, OnOff=0, set bit to 0
void maskCircle(int *buf, int nX,int nY, int nRadius, int OnOff) {
    for (int i = nX - nRadius; i <= nX+nRadius; ++i) {
        for (int j = nY - nRadius; j <= nY + nRadius; ++j) {
            if ((i >= 0) && (i < ROWS) && (j >= 0) && (j < COLS) && (((i - nX)*(i - nX) + (j - nY)*(j - nY)) <= nRadius*nRadius)) {
                if (OnOff) {
                    buf[i*ROWS + j] |= (1 << 0);    // set mask bit to 1
                }
                else {
                    buf[i*ROWS + j] &= ~(1 << 0);   // set mask bit 0 0
                }
            }
        }
    }
}

// Transcode the 2D mask into the Binary Pixel Configuration 1D mask. Quad 2x2 TimePix3 only
void mask2DtoBPC(int *buf, char *bufBPC) {
    int index = 0;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {

            // Image coordinate
            if (i < 256) { // chip 2, 3
            //    printf("Chip2,3\n");
                if (j < 256) { 
                    index = 3*256*256 + i + j*256;  // chip 3, CORRECT
                    if ((index > 4*256*256) || (index < 3*256*256)) {
                        printf("chip3, i=%d, j=%d, index=%d\n", i, j, index);
                    }
                    if (buf[i*ROWS + j] & (1 << 0)) {
                        bufBPC[index] |= (1 << 0);
                    }
                }
                else if ((j >= 256) && (j < 512)) {     // chip 2
                    index = 2*256*256 + (255 - i) + 256 * (511 - j); 
                    if ((index > 3*256*256) || (index < 2*256*256)) {
                        printf("chip2, i=%d, j=%d, index=%d\n", i, j, index);
                    }
                    if (buf[i*ROWS + j] & (1 << 0)) {
                        bufBPC[index] |= (1 << 0);
                    }
                }
                else {
                    printf("image larger than 256 x 512\n");
                }
            }
            else if ((i >= 256) && (i < 512)) {
                if (j < 256) {  
                    index = (i - 256) + j*256; // chip 0, CORRECT
                    if ((index > 256*256) || (index < 0)) {
                        printf("chip0, i=%d, j=%d, index=%d\n", i, j, index);
                    }
                    if (buf[i*ROWS + j] & (1 << 0)) {
                        bufBPC[index] |= (1 << 0);
                    }
                }
                else if ((j >= 256) && (j < 512)) {     // chip 1
                    index = 2*256*256 + (255 - i) - 256 *(j-256); // chip 1, CORRECT
                    if ((index > 2*256*256) || (index < 256*256)) {
                        printf("chip1, i=%d, j=%d, index=%d\n", i, j, index);
                    }
                    if (buf[i*ROWS + j] & (1 << 0)) {
                        bufBPC[index] |= (1 << 0);
                    }
                }
                else {
                    printf("image larger than 512 x 512\n");
                }
            }
            else {
                printf("image size larger than 512 x 512\n");
            }
                

        }
    }    
}


int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        return 1;
    }

    FILE *sourceFile, *destFile;
    long fileSize;
    char *buffer;
    long nMaskedPixels;
    int OnOffPel=0;

    // Declare and initialize the 2D mask array
    int binaryArray[ROWS][COLS] = {0};

    OnOffPel = 1;
    maskReset((int*) binaryArray,OnOffPel);
    maskRectangle( (int*) binaryArray, 3, 2, 15, 3, 0);
    maskCircle((int*) binaryArray, 5, 5, 4, 0);
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 30; ++j) {
            printf(",%d", binaryArray[i][j]);
        }
        printf("\n");
    }
//    OnOffPel = 1;
//    maskReset((int*) binaryArray,OnOffPel);
//    maskRectangle( (int*) binaryArray, 150, 200, 150, 200, 0);

    // Declare TimePix3 Binary Pixel Configuration .bpc vector index
    int index = 0;

    // Open the source file for reading
    sourceFile = fopen(argv[1], "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return 1;
    }

    // Determine the size of the source file
    fseek(sourceFile, 0, SEEK_END); // Move to the end of the file
    fileSize = ftell(sourceFile);    // Get the current position (file size)
    rewind(sourceFile);              // Go back to the beginning of the file

    // Allocate memory for the buffer
    buffer = (char *)malloc(fileSize);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(sourceFile);
        return 1;
    }

    // Read the entire file into the buffer
    fread(buffer, 1, fileSize, sourceFile);
    fclose(sourceFile);

    nMaskedPixels = find_masked_positions(buffer, fileSize, 0);
    printf("Number of masked pixels=%ld\n", nMaskedPixels);

    mask2DtoBPC( (int*) binaryArray, buffer);

    // After mask applied
    nMaskedPixels = find_masked_positions(buffer, fileSize, 0);
    printf("Number of masked pixels=%ld\n", nMaskedPixels);

//    // Print the array to verify initialization
//    for (int i = 0; i < ROWS; i++) {
//        for (int j = 0; j < COLS; j++) {
//            printf("%d ", binaryArray[i][j]);
//        }
//        printf("\n");
//    }

    // Open the destination file for writing
    destFile = fopen(argv[2], "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        free(buffer);
        return 1;
    }

    // Write the buffer to the destination file
    fwrite(buffer, 1, fileSize, destFile);

    // Clean up
    free(buffer);
    fclose(destFile);

    printf("File copied successfully.\n");
    return 0;
}
