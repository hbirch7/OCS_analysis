--- Firmware testing analysis tool ---

Before starting an analysis ensure the board data is moved to the SURF_datastore/ directory.

Append the varible fname with the new data file name in tree.h

Clean the make file using the command: $ make clean

Compile the analysis code using the command $ make

To run an analysis use the following command $ python2.7 runMe.py <filename>

