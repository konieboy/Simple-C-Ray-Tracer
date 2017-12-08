FILES = Color.cpp Object.cpp RayTracer.cpp Geometry.cpp Scene.cpp
CC = g++ -Wall

all: rayTracer

Files:
	$(CC) -c $(FILES)
rayTracer: Files
	$(CC) -c WritePixels.cpp
	$(CC) -o rayTracer Color.o Geometry.o Object.o Scene.o RayTracer.o WritePixels.o
clean:
	rm -f *.o
	rm -f writepixels
	
