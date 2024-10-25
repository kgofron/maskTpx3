# Initialize EPICS waveform
* Initializes a 512x512 waveform record in EPICS, setting all values to 1

## Compile
Make sure to link against the necessary EPICS libraries

* Makefile
```
PROG = initialize_waveform
SRC = initialize_waveform.c

include $(EPICS_BASE)/bin/linux-x86_64/Makefile
```


## Running the Program
* Load database
	* dbLoadDatabase("path/to/waveform.db")
* Compile and run the C program
	* make
	* ./initialize_waveform


## Notes

- Make sure to adapt the record names and types as needed for your specific EPICS setup.
- This program initializes each row of the 2D waveform separately, assuming that you have separate records for each row. Adjust the logic if you are using a single 1D waveform with 512x512 data.
- Error handling in this example is minimal; consider adding more robust checks as necessary.
