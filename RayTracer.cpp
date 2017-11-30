#include "RayTracer.h"
#include <typeinfo>
RayTracer::RayTracer(Scene * s, int maxd, int sm){
  scene = s;
  maxdepth = maxd;
  samples = sm;
}

// This function determines the closest object that a ray intersects.
// The getIntersection() method is polymorphic. Please provide implementations
// for all the objects in your scene (see Object.h and Object.cpp).
Object * RayTracer::intersect(Ray r){
  scene->startIteration();
  Object * current = NULL;
  Object * minObject = NULL;
  Point inter;
  //Initialize min to infinite point;
  Point min = Point::Infinite();
  while((current = scene->getNextObject()) != NULL){
    inter = current->getIntersection(r);
    if((inter - r.p).length() > 1E-6){
      if((inter-r.p).length()  < (min-r.p).length()){
        min = inter;
        minObject = current;
      }
    }
  }
  return minObject;
}


// Trace a ray recursively
Color RayTracer::trace(Ray r, int depth){
  Color rad=Color(0.0,0.0,0.0,0.0);
  
  // YOUR CODE FOR RECURSIVE RAY TRACING GOES HERE

  // Calculate intersection P between ray and closest object
  
  Object* hitObj = intersect(r);
  if (hitObj == NULL)
  {     
    return rad;
  }

  if (typeid(*hitObj) == typeid(Triangle) )
    rad = Color(0.1,1.0,0.0,0.0);
  else
    rad = Color(0.1,1.0,1.0,0.0);

  // Determine the material type of that object 
  Material* mat = hitObj->getMaterial();

  return Phong(r, P);

  //hitObj->getMaterial();

 

  //return Shade(r, P, material);



  return rad;
}

// Local Phong illumination at a point.
Color RayTracer::Phong(Point normal,Point p, Ray r, Material * m, Object * o){
  Color ret = Color(0.0, 0.0, 0.0, 0.0);
  
  // YOUR CODE HERE.
  // There is ambient lighting irrespective of shadow.
  // Specular-diffuse lighting only if the point is not in shadow
  
  // Remember, you need to account for all the light sources.
  
  return ret;
}


// This function generates point on the image plane and starts a trace 
// through them.
// Grid supersampling is also implemented.
Color RayTracer::calculate(int x, int y){
  Color c = Color(0.0,0.0,0.0,0.0);
  for(int i = 1; i <= samples; i++){
    for(int j = 1; j <= samples; j++){
      double one, two;
      // We subtract 0.5 so that the (u,v)-origin is at the
      //center of the image plane.
      one = double(x)-0.5+double(i)/double(samples);
      two = double(y)-0.5+double(j)/double(samples);
      Ray r = scene->makeRay(one,two);
      c = c+trace(r,0);
    }
  }
  c = c*(1.0/double(samples*samples));
  return c;
}

