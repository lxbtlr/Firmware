import serial
import signal

#
# Note 1: This python script was designed to run with Python 3.
#
# Note 2: The script uses "pyserial" which must be installed.  If you have
#         previously installed the "serial" package, it must be uninstalled
#         first.
#
# Note 3: While this script is running you can not re-program the Arduino.
#         Before downloading a new Arduino sketch, you must exit this
#         script first.
#


#
# Set the name of the serial port.  Determine the name as follows:
#	1) From Arduino's "Tools" menu, select "Port"
#	2) It will show you which Port is used to connect to the Arduino
#
# For Windows computers, the name is formatted like: "COM6"
# For Apple computers, the name is formatted like: "/dev/tty.usbmodemfa141"
#
arduinoComPort = "COM8"


#
# Set the baud rate
# NOTE1: The baudRate for the sending and receiving programs must be the same!
# NOTE2: For faster communication, set the baudRate to 115200 below
#        and check that the arduino sketch you are using is updated as well.
#
baudRate = 9600


#
# open the serial port
#
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

data = []
reading = []
sensor1 = []
sensor2 = []
i = 0
n = 0

print("The available commands are: SD, NEO, DRS")
#
# main loop to read data from the Arduino, then display it
#
while True:
  #
  # ask for a line of data from the serial port, the ".decode()" converts the
  # data from an "array of bytes", to a string
  #
  lineOfData = serialPort.readline().decode()
  #print(lineOfData)
  #
  # check if data was received
  #
  data.append(lineOfData)
  # reading.append(lineOfData[0:6])
  # sensor1.append(lineOfData[7:11])
  # print(reading[i])
  # print(sensor1[i])
  # i += 1

  CMD_Interrupt = input()

  if CMD_Interrupt == "SD":
      print("Initializing Self Destruct Mode")
      serialPort.write(0x1A)
      data.append("SD")
  
  if CMD_Interrupt == "NEO":
      print("Initializing the neopixel sequence")
      serialPort.write(0x1B)
      data.append("NEO")

  if CMD_Interrupt == "DRS":
      print("Deploying the Recovery System")
      serialPort.write(0x1C)
      data.append("DRS")

  #print(data)

  if len(lineOfData) > 0:
    #
    # data was received, convert it into 4 integers
    #
    #
    # print the results
    #
    
    data.append(lineOfData)


    # print(lineOfData)
    
    with open('listfile.txt', 'w') as filehandle:
        for listitem in data:
            filehandle.write('%s\n' % listitem)

    #print(", c = " + str(c), end="")
    #print(", d = " + str(d))

