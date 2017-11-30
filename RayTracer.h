#include <vector>
#include <cstdlib>
#include "Scene.h"

using namespace std;

class RayTracer {
    Scene * scene; // pointer to the scene object
    int samples;   // will use a samples x samples subgrid within each pixel for supersampling
    int maxdepth;  // the maximum depth for recursive raytracing.
public:
    RayTracer(Scene *,int,int); //constructor
    Color trace(Ray, int);  // recursively traces a ray
    Object * intersect(Ray); // compute intersection of ray and return the closest object

    Color calculate(int,int); // compute the colour at a pixel
    Color Phong(Point,Point,Ray,Material*,Object *); // Local illumination at a point
};

