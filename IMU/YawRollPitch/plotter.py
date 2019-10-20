import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import serial

ser = serial.Serial(port='COM3', baudrate=115200, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=0)
line = []
# style.use('ggplot')
#
# fig = plt.figure()
# ax1 = fig.add_subplot(1,1,1)
#
#
#
# def animate(i):
#     graph_data = open('example.txt','r').read()
#     lines = graph_data.split('\n')
#     xs = []
#     ys = []
#     for line in lines:
#         if len(line) > 1:
#             x, y = line.split(',')
#             xs.append(float(x))
#             ys.append(float(y))
#     ax1.clear()
#     ax1.plot(xs, ys)

update_var = ""
data = {"r": 0.00, "p": 0.00, "y": 0.00}
temp = {"r": "", "p": "", "y": ""}

while True:

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
'''
    if true:
        char = str(bits)
        char = char[-2]
        if char == 'r':
            char = str(ser.read())
            char = char[-2]
            if char == 'n':
                data[update_var] = float(temp[update_var])
                temp[update_var] = ""
                print("EOM")
            else:
    
                update_var = 'r'
        elif char.isalpha() and char != 'n':
            update_var = char

        elif char.isdigit() or char == '-':
            print(char)
            temp[update_var] += char
        # decoded_bytes = bits[0:len(bits)].decode("utf-8")
        # print(decoded_bytes)

        print(data)

    for c in ser.read():

        print(concat(c))

        line.append(c)
        if c == 'X':
            print(.join(line))
            line = []
            break
'''
ser.close()
