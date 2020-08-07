import serial
ser = serial.Serial('/dev/ttyS0', 2000000)
while 1:
    line = ser.read()
    print(line,)

