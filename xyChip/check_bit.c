#include <stdio.h>
#include <stdlib.h>

#define CHIP_SIZE 65536    // 256 * 256 bytes per chip
#define DIMENSION 256      // Both X and Y are 256 elements

void calculate_coordinates(long position, int *chip, int *x, int *y) {
    // Calculate chip number (0-3)
    *chip = position / CHIP_SIZE;
    
    // Calculate position within the chip
    long pos_in_chip = position % CHIP_SIZE;
    
    // Calculate X (column) and Y (row)
    // Since each row has 256 elements, Y is pos_in_chip / 256
    // and X is the remainder
    *y = (pos_in_chip / DIMENSION);
    *x = (pos_in_chip % DIMENSION);
}

/*
 * Calculate global image coordinates (X,Y) from local chip coordinates (x,y)
 * This function is for LEFT orientation only.
 * 
 * Chip layout for LEFT orientation:
 *    [Chip 1]    [Chip 0]    (upper-left)  (upper-right)
 *    [Chip 2]    [Chip 3]    (lower-left)  (lower-right)
 * 
 * For reference, UP orientation would be:
 *    [Chip 2]    [Chip 1]    (left-top)    (upper-right)
 *    [Chip 3]    [Chip 0]    (left-bottom) (lower-right)
 * 
 * Transformations for LEFT orientation:
 * Chip 0: X=511-y, Y=255-x  (upper-right)
 * Chip 1: X=y, Y=x          (upper-left)
 * Chip 2: X=y, Y=256+x      (lower-left)
 * Chip 3: X=511-y, Y=511-x  (lower-right)
 */
void calculate_image_coordinates(int chip, int x, int y, int *X, int *Y) {
    switch(chip) {
        case 0:
            *X = 511 - y;    // Flip and invert Y coordinate
            *Y = 255 - x;    // Flip and invert X coordinate, max 255
            break;
        case 1:
            *X = y;          // Y coordinate becomes X
            *Y = x;          // X coordinate becomes Y
            break;
        case 2:
            *X = y;          // Y coordinate becomes X
            *Y = 256 + x;    // X coordinate offset by 256
            break;
        case 3:
            *X = 511 - y;    // Flip and invert Y coordinate
            *Y = 511 - x;    // Flip and invert X coordinate, max 511
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Print file name, orientation and table header
    printf("File: %s\n", argv[1]);
    printf("Orientation=LEFT\n");
    printf("------------------------------------------------------------------------\n");
    printf("#\tPosition\tValue\tChip\tx\ty\tX\tY\n");
    printf("-\t--------\t-----\t----\t-\t-\t-\t-\n");

    unsigned char byte;
    long position = 0;
    int chip, x, y, X, Y;
    int counter = 1;  // Initialize counter

    while (fread(&byte, 1, 1, file) == 1) {
        // Check if bit 0 is 1
        if (byte & 1) {
            calculate_coordinates(position, &chip, &x, &y);
            calculate_image_coordinates(chip, x, y, &X, &Y);
            printf("%d\t%ld\t\t0x%02X\t%d\t%d\t%d\t%d\t%d\n", 
                   counter++, position, byte, chip, x, y, X, Y);
        }
        position++;
    }

    fclose(file);
    return 0;
}
