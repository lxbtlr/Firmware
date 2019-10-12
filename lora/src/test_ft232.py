import usb.core
from ftdi_ft232.spi import SPI
import ftdi_ft232.board as board
import adafruit_rfm9x
import busio
from digitalio import DigitalInOut, Direction, Pull
# import time
# import serial
# dev=usb.core.find(idVendor=0x0403,idProduct=0x6014)
# dev.set_configuration()
# print(dev)

# test = SPI()
# slave = test._spi.get_port(cs=0, freq=12E6, mode=0)
# slave.write(b'hello world!')
# test.write(0x03)
# print(test.readinto(0x03))
# from pyftdi.usbtools import UsbTools

# usb = UsbTools.find_all()
# print(usb)

spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
CS = DigitalInOut(board.CS)
RESET = DigitalInOut(board.RESET)

while True:
    try:
        rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 915.0)
        print('RFM9x: Detected')
    except RuntimeError:
        print('RFM9x: ERROR')
