CC = g++
CFLAGS=`pkg-config --cflags gtkmm-3.0`
LDFLAGS=`pkg-config --libs gtkmm-3.0` -lm -lpthread

OBJ=main.o global.o tcpserver.o car.o camera.o myarea.o vectors.o map.o sensor.o

exe: $(OBJ)
	$(CC) $(CFLAGS) -o sim.exe *.o $(LDFLAGS)
%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -rf *.o *.exe
