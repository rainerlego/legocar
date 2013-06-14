CC = armv6j-hardfloat-linux-gnueabi-gcc
CFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
LDFLAGS=-lm -lpthread -lcwiid

OBJ=main.o tcpserver.o servo.o servoboard.o servosim.o wiimote.o

exe: $(OBJ)
	$(CC) $(CFLAGS) -o raspi.exe *.o $(LDFLAGS)
%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -rf *.o *.exe
