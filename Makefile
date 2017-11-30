FILES = Color.cpp Object.cpp RayTracer.cpp Geometry.cpp Scene.cpp
CC = g++ -Wall

all: writepixels

Files:
	$(CC) -c $(FILES)
writepixels: Files
	$(CC) -c WritePixels.cpp
	$(CC) -o writepixels Color.o Geometry.o Object.o Scene.o RayTracer.o WritePixels.o
clean:
	rm -f *.o
	rm -f writepixels
	
