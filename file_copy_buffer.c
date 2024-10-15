#include <stdio.h>
#include <stdlib.h>

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
    int rows = 512;
    int cols = 512;
    long binaryArray[rows][cols] = {0}; // All elements initialized to 0
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

    nMaskedPixels = find_masked_positions(buffer, fileSize, 0);
    printf("Number of masked pixels=%ld\n", nMaskedPixels);

    // Set bit 0 to 1: pixels are not processed
    //for (int i = 0; i < 55; ++i) { 
    //    buffer[i] |= (1 << 0); // Set bit 0 to 1 
    //}

    // Rectangular image mask (X=20, Y=30, SizeX=30, SizeY=50)
    //for (int i = 20; i < 50; ++i) {
    //    for (int j = 30; i < 80; ++i) { 
    //        binaryArray[i][j] = 1;
    //    }
    //}

//    // Binary Pixel mask 
//    for (int i = 0; i < fileSize; ++i) {
//        if (binaryArray[i][i] == 1) {
//            buffer[i] |= (1 << 0); // Set bit 0 to 1 
//        }
//    }

    // Find Chip starting pixel; 0 < i,j <256
    // chip0: buffer[i+j*256] -> (256+i,j) in image
    // chip1: buffer[i+j*256 + 256*256] -> (512-i,512-j) in image
    // chip2: buffer[i+j*256 + 2*256*256] -> (256-i,256-j) in image
    // chip0: buffer[i+j*256 + 3*256*256] -> (i,j) in image
    for (int i = 150; i < 350; ++i) {
        for (int j = 150; j < 350; ++j) {
        //    buffer[i+j*256] |= (1 << 0); // ORIGIN Chip0 (down-left corner): Set bit 0 to 1
        //    buffer[i+j*256 + 256*256] |= (1 << 0); // ORIGIN Chip1 (upper-right corner): Set bit 0 to 1
        //    buffer[i+j*256 + 2*256*256] |= (1 << 0); // ORIGIN Chip2 (upper-right corner): Set bit 0 to 1
        //    buffer[i+j*256 + 3*256*256] |= (1 << 0); // ORIGIN Chip3 (down-left corner): Set bit 0 to 1

            // Image coordinate
            if (i < 256) { // chip 2, 3
            //    printf("Chip2,3\n");
                if (j < 256) { 
                    index = 3*256*256 + i + j*256;  // chip 3, CORRECT
                    if ((index > 4*256*256) || (index < 3*256*256)) {
                        printf("chip3, i=%d, j=%d, index=%d\n", i, j, index);
                    }                    
                    buffer[index] |= (1 << 0);
                }
                else if ((j >= 256) && (j < 512)) {     // chip 2
                    index = 2*256*256 + (255 - i) + 256 * (511 - j); 
                    if ((index > 3*256*256) || (index < 2*256*256)) {
                        printf("chip2, i=%d, j=%d, index=%d\n", i, j, index);
                    }
                    buffer[index] |= (1 << 0);
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
                    buffer[index] |= (1 << 0);
                }
                else if ((j >= 256) && (j < 512)) {     // chip 1
                    index = 2*256*256 + (255 - i) - 256 *(j-256); // chip 1, CORRECT
                    if ((index > 2*256*256) || (index < 256*256)) {
                        printf("chip1, i=%d, j=%d, index=%d\n", i, j, index);
                    }
                    buffer[index] |= (1 << 0);
                }
                else {
                    printf("image larger than 512 x 512\n");
                }
            }
            else {
                printf("image size larger than 512 x 512\n");
            }
                

        }
        //    buffer[256+i+j*256] |= (1 << 0); // ORIGIN Chip0 (down-left corner): Set bit 0 to 1
        //    buffer[i+j*256 + 256*256] |= (1 << 0); // ORIGIN Chip1 (upper-right corner): Set bit 0 to 1
        //    buffer[i+j*256 + 2*256*256] |= (1 << 0); // ORIGIN Chip2 (upper-right corner): Set bit 0 to 1
        //    buffer[i+j*256 + 3*256*256] |= (1 << 0); // ORIGIN Chip3 (down-left corner): Set bit 0 to 1
    }    

    // Open the destination file for writing
    destFile = fopen(argv[2], "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        free(buffer);
        fclose(sourceFile);
        return 1;
    }

    // Write the buffer to the destination file
    fwrite(buffer, 1, fileSize, destFile);

    // Clean up
    free(buffer);
    fclose(sourceFile);
    fclose(destFile);

    printf("File copied successfully.\n");
    return 0;
}
