#include <epicsTypes.h>
#include <epicsExport.h>
#include <dbAccess.h>
#include <dbScan.h>
#include <recGbl.h>
#include <dbRecordType.h>
#include <waveformRecord.h>

#include <stdio.h>
#include <stdlib.h>

#define N_ROWS 512
#define N_COLS 512

void initializeWaveform(void) {
    waveformRecord *pwf;
    int status;

    for (int i = 0; i < N_ROWS; i++) {
        // Get the waveform record for the current row
        char recordName[20];
        snprintf(recordName, sizeof(recordName), "myWaveform_%d", i);
        status = dbGetRecord(pwf, recordName);
        
        if (status != 0) {
            printf("Error accessing record %s\n", recordName);
            continue;
        }

        // Set all elements to 1
        for (int j = 0; j < N_COLS; j++) {
            pwf->bptr[j] = 1.0; // Assuming double type
        }
        
        pwf->nord = N_COLS; // Set the number of elements in use
        pwf->val = 1;       // Ensure a non-zero value for the record
        pwf->stat = 0;      // Clear the status
        pwf->sevr = 0;      // Clear the severity

        // Process the record to apply changes
        dbScanLock((dbCommon *)pwf);
        process(pwf);
        dbScanUnlock((dbCommon *)pwf);
    }

    printf("Waveform records initialized to 1.\n");
}

int main() {
    // Call the initialization function
    initializeWaveform();
    return 0;
}
