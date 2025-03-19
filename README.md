EDID/DisplayID Writer is a tool for writing a new EDID or DisplayID to the display.

Warning: This program modifies the EEPROM data in the display. If something goes wrong, it can make the display unusable until the data is corrected. NVIDIA in particular will not recognize a display with invalid data, so you would need to use an AMD GPU or some other method to fix the data. Use at your own risk.

    A message from ToastyX:

    Over the years, I have created various monitor-related software and provided support for free. I would like to continue providing updates and work on new ideas, but I need your support. If you find my software useful, please consider supporting me through Patreon:

    [Image: patreon.png]


Requirements:

    Windows Vista or later
    AMD/ATI or NVIDIA GPU with appropriate driver installed (Intel is currently not supported)

EDID usage:

    To save the current EDID to a file, click the "Read EDID" button, and then click the "Save File..." button.
    To write a new EDID to the display, click the "Load File..." button to choose a file, and then click the "Write EDID" button.

If the EDID has a corrupted header, invalid extension block, or invalid checksum, it will offer to fix the data. No changes are made to the display until clicking the "Write EDID" button.

DisplayID usage:

    To save the current DisplayID to a file, click the "Read DisplayID" button, and then click the "Save File..." button.
    To write a new DisplayID to the display, click the "Load File..." button to choose a file, and then click the "Write DisplayID" button.

Notes:

Many monitors have the EEPROM write-protected. Some monitors allow writing by accessing the service menu and enabling burn-in mode or some other setting. The service menu can often be accessed by holding one or two buttons down while powering on the monitor. Another method is to physically disconnect the write-protect pin on the EEPROM chip as described in this guide: https://blurbusters.com/zero-motion-blur/hardware-mod/

Due to driver limitations, this program can only read and write up to 256 bytes. That is enough for an EDID with one extension block or one DisplayID block. Any data beyond 256 bytes will be preserved.

Changes in Beta 2:

    Fixed failure to start on older versions of Windows 10
