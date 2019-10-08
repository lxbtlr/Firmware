from ftdi_ft232.spi import SPI
# import time
# import serial
import usb.core
dev=usb.core.find(idVendor=0x0403,idProduct=0x6014)
# dev.set_configuration()
print(dev)

test = SPI()
# test.write(0x03)
# print(test.readinto(0x03))
# from pyftdi.usbtools import UsbTools

# usb = UsbTools.find_all()
# print(usb)

