CC=g++
CFLAGS= -Wall -O3
LDFLAGS+= -lSDL

EXEC=Mandelbrot

all: $(EXEC)

$(EXEC): main.cpp
	$(CC) -o $(EXEC) main.cpp $(LDFLAGS)

clean:
	-rm $(EXEC)