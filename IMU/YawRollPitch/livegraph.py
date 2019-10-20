
#ORIGINAL SOURCE CODE: https://github.com/engineersportal/pylive

import matplotlib.pyplot as plt
import numpy as np
import time
import serial
from pathlib import Path
# from kbhit import KBHit
import sys


# use ggplot style for more sophisticated visuals
plt.style.use('ggplot')


#initialize pyspift objects
#initialize keyboard support
# kb = KBHit()

#initialize data limits
global maxLim, minLim
maxLim = 30000
minLim = 0

max_vals = [0,0,0,0,0,0]
min_vals = [0,0,0,0,0,0]

#initialize data set
size = 5000
x_vec = np.linspace(0,1,size+1)[0:-1]
y_val = [[0 for x in range(len(x_vec))] for y in range(3)]

#initialize average data readout
avg_vals = [0,0,0,0,0,0]
avgLen = 100 #max = 100


#intialize plot
plt.ion()
fig = plt.figure(figsize=(13,6))
ax = fig.add_subplot(111)
plt.ylabel('Raw Value')
plt.title('YPR IMU'.format(''))

roll, = ax.plot(x_vec,y_val[0],'-',alpha=0.8)
pitch, = ax.plot(x_vec,y_val[1],'-',alpha=0.8)
yaw, = ax.plot(x_vec,y_val[2],'-',alpha=0.8)

lines = [roll, pitch, yaw]
ax.legend(lines,('roll', 'pitch', 'yaw'),loc='lower left')
plt.show()

ser = serial.Serial(port='COM3', baudrate=115200, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=0)
update_var = ""
data = {"r": 0.00, "p": 0.00, "y": 0.00}
temp = {"r": "", "p": "", "y": ""}
#main loop
counter =0
while True: # while not kb.kbhit():

    #get adc reading
    #This is the update portion of the code
    val = [data['r'], data['p'], data['y']]


    bits = ser.read()
    # if len(decoded_bytes) >=20 and decoded_bytes.count("y")==1 and decoded_bytes.count("p") ==1 and decoded_bytes.count("r")==1 and decoded_bytes.count("X")==1:
    if bits:
        tempy = str(ser.readline())
        tempy = tempy.replace(' ', '')

        #
        if len(tempy)<8:
            continue
        # print(tempy)

        tempy = tempy[2:-5]
        # print(tempy)
        if tempy[0].isalpha():
            update_var = tempy[0]
        else:
            continue
        for i in range(1, len(tempy)):
            temp[update_var] += tempy[i]
        if len(temp[update_var])<2:
            temp[update_var] =""
            continue
        data[update_var] = float(temp[update_var])
        temp[update_var] = ""
        print(data)

    #print average values for each pin


    # plt.pause(.011)
    if counter >100:
        print(time.time())
        fig.canvas.draw()
        fig.canvas.flush_events()
        counter = 0

    counter+=1
    for i in range(len(lines)):
        #assign new adc value
        y_val[i][-1] = val[i]
        #update plot
        lines[i].set_ydata(y_val[i])

        # #Assign new max/min values for data
        # min_vals[i] = np.min(y_val[i])
        # max_vals[i] = np.max(y_val[i])

        #shift adc values down
        y_val[i] = np.append(y_val[i][1:],0.0)
    #
    # #assign new y-axis lims if applicable
    # if (np.min(min_vals) < minLim) or np.min(min_vals) > minLim:
    #     minLim = np.min(min_vals)
    #     plt.ylim([minLim, maxLim])
    # if (np.max(max_vals) < maxLim) or np.max(max_vals) > maxLim:
    #     maxLim = np.max(max_vals)
        plt.ylim([-90, 360])
ser.close()
