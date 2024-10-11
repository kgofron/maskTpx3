#include <stdio.h>

int main() {
    const char *filename = "mask.bpc";
    
    // Open the binary file for reading and writing
    FILE *file = fopen(filename, "r+b");
    int nBytes = 0;
    
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Create a character with bit 3 and bit 7 set to 1
    char ch = 0x00;  // Start with 0x00
    // char ch = 0xC;  // Start with 0xC
    // ch |= (1 << 3); // Set bit 3 (8 in decimal)
    // ch |= (1 << 5); // Set bit 5 (32 in decimal)

    // Replace the first 15 bytes with the modified character
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 0; i < 32768; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    ch = 0x00;  // Start with 0x00
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 32768; i < 65536; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    ch = 0x00;  // Start with 0x00
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 65536; i < 98304; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    ch = 0x00;  // Start with 0x00
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 98304; i < 131072; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    ch = 0x00;  // Start with 0x00
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 131072; i < 163840; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    ch = 0x00;  // Start with 0x00
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 163840; i < 196608; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    ch = 0x00;  // Start with 0x00
    ch |= (1 << 0); // Set bit 1 (8 in decimal)
    for (int i = 196608; i < 229376; ++i) {   // 1/2 of one chip
        fputc(ch, file);  // Write the modified character
    }

    // ch = 0x00;  // Start with 0x00
    // ch |= (1 << 0); // Set bit 1 (8 in decimal)
    // for (int i = 229376; i < 262144; ++i) {   // 1/2 of one chip
    //     fputc(ch, file);  // Write the modified character
    // }

    // Move the cursor back to the beginning of the file for reading
    rewind(file);

    printf("Data from file (after modification): ");

    // Read and echo the file content one byte at a time
    int byte;
    while ((byte = fgetc(file)) != EOF) {
    //    printf("%02X ", byte);  // Print each byte as a hex value
        nBytes += 1;
    }

    printf("\n### nByte=%d\n", nBytes);

    printf("\n");
    fclose(file);

    return 0;
}
