## Long Range Radio Communications
Team Members: Daniel, Sparsh

We are currently working on interacting with the radio board via arduino and pyserial.

### Instructions for working with SPI chip

#### Checking functionality

Run `ls /dev/tty*`. You should see one result called `/dev/ttyUSB0`. That is the ftdi ft232h. Run the command `screen /dev/ttyUSB0`. If you type, nothing should appear. Now connect D0 and D1. Whatever you type should be printed to the screen; this means that connecting the input and output is mirroring the output back to you. To exit the screen shell, hit `CTRL-a` and then `\` before finally hitting `y` to exit.

#### Debugging lang-id error
 Run `cd ~/etc/udev/rules.d` to move into the correct directory.
 Run `sudo touch 11-ftdi.rules` to create a new udev rules file
 Run `sudo vim 11-ftdi.rules` to enter your new rules file in vim
 Press `i` to enter edit mode in vim.

 Add the following to the file:

```
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6001", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6011", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6010", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6014", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6015", GROUP="plugdev", MODE="0666"
```

 When finished, hit `esc`, then enter `:wq` to write and quit the file. Unplug and replug in the device and things should work.
