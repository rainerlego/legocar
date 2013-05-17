import serial
import time
import sys

s = serial.Serial(port="/dev/ttyUSB0", baudrate=115200, timeout=0.1, writeTimeout=0.1)
s.write("#\n")
print s.read(1024)    
s.write("m\n")
s.write("4\n")
s.write("1\n")
s.flush()
print s.read(1024)

i = int(sys.argv[2]);

print(str(i) + "\n" )
s.write("[")
s.write("0x02,")
s.write("0xff,")
s.write("0x0");
s.write(sys.argv[1]);
s.write(",")
s.write(str(i / 256))
s.write(",")
s.write(str(i % 256))
s.write("]\r\n")
s.flush()
print s.read(1024)
