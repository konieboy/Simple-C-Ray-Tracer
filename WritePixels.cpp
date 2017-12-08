#include <iostream>
#include <fstream>

#include <math.h>

#include "RayTracer.h"
#include <fstream>
using std::ofstream;
using std::ifstream;

// store width and height of the render
int width = 512;
int height = 512;

int fov = 55;

// Our ray tracer
RayTracer * rt;

int main(const int argc, const char *argv[]){

	string fileName = "default";
	string test  = argv[1];

	if (argc > 2) // Custom image size
	{
		width = atoi(argv[2]);
		height = atoi(argv[3]);
	}	
	cout<< "\nResolution: \nWidth: "<< width << "\theight: " << height<< endl;

	if (argc > 4) // Custom FOV
	{
		fov = atoi(argv[4]);
	}	
	cout<< "\nFov: " << fov << endl;

	if (test == "--default")
	{
		fileName  = "default.ppm";
		rt = new RayTracer(Scene::initTestScene(width, fov),4,1);

	}
	if (test == "--yours")
	{
		fileName  = "yours.ppm";
		rt = new RayTracer(Scene::initTestScene2(width, fov),4,1);
	}

	cout<< "\nFile saved as: "<< fileName << endl;


    // Test scene with max depth of 4 and sampling of 1
	//rt = new RayTracer(Scene::initTestScene(width),12,4);

    float pixels[width][height][4];
    for(int ctr = 0; ctr < height*width; ctr++){
    	int i = ctr % width;
    	int j = ctr / width;
    	Color rad = rt->calculate(i,j); // Calculate Ray Trace
    	pixels[i][j][0] = rad.r; //Red
    	pixels[i][j][1] = rad.g; //Green
    	pixels[i][j][2] = rad.b; //Blue
    	pixels[i][j][3] = 1.0; //Alpha
    }
    // once we are done calculating, we will write to file.
    ofstream testimage;
	testimage.open(fileName,ios::binary | ios::out);
		testimage << "P3\n" << width << " " << height << endl << width-1 << endl;
	for(int j = height-1; j >=0 ; j-- ) {
	    for(int i = 0; i< width; i++){
			for(int k = 0; k < 3; k++)
			{
				// normalize color value to 0-255.
				// This assumes that the color values are in the
				// range [0,1].
				//char c = int(pixels[i][j][k]*255);
				testimage << int(pixels[i][j][k]*(width-1)) << " ";
				//testimage << c;
			}
			testimage << "\t";	// write tab
		} 
		testimage << "\n"; // Write new line
	}
	testimage.close();
    return 0;   
}
