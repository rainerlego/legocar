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

s.write("[")
s.write("0x02,")
s.write("0xff,")
ledv = 0x10
print ledv
print sys.argv[1]
if sys.argv[1] == "on":
  print "on"
  ledv |=1<<3
for i in range(2,len(sys.argv)):
  tmp1 = int(sys.argv[i])-1
  ledv |= 1<<tmp1
print "ledv = "
print ledv

s.write(str(ledv))
s.write("]\r\n")
s.flush()
print s.read(1024)
