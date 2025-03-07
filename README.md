FIC compares a MASTER filetree with a SLAVE filetree.
A MASTER or SLAVE can be a directory, a ZIP file or a checksum list file generated earlier with FIC.
MASTER serves as the source for the files to be compared. Any MASTER files that are not available in SLAVE
are logged as not available. Any SLAVE files that are not available in MASTER are ignored.

Usage: FIC /COMMAND PARAMETER -SWITCH   ...etc...

COMMANDS:
  /md directory   - MASTER is the given directory.
  /mz filename    - MASTER is the given ZIP file.
  /mc filename    - MASTER is the given checksum list file.
  /sd directory   - SLAVE is the given directory.
  /sz filename    - SLAVE is the given ZIP file.
  /sc filename    - SLAVE is the given checksum list file.
  /c filename     - Saves a checksum list file generated from MASTER.
  /l filename     - Saves a log file.

SWITCHES:
  -s  - Silent mode, file operations won't be shown on screen.
  -fs - Errors while searching for files are fatal, otherwise they are logged but search continues.
  -fo - File operation errors are fatal, otherwise they are logged but operations continue on other files.

RETURN CODES:
Bit 0 (LSB):  - Command line error.
Bit 1:        - Log error.
Bit 2:        - One or more file comparison(s) failed.
Bit 3:        - Error building master.
Bit 4:        - Error building slave.
Bit 5:        - Checksum list error.
Bit 6:        - One or more file(s) were unavailable on the master side.
Bit 7:        - One or more file(s) were unavailable on the slave side.
Bit 8:        - One or more read error(s) on the master side.
Bit 9:        - One or more read error(s) on the slave side.
Bit 10 (MSB): - Platform specific error.
