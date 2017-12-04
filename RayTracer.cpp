#include "RayTracer.h"
#include <typeinfo>
#include <iostream>


#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

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

  // Determine the material type of that object 
  Material* mat = hitObj->getMaterial();

  Point intersection =  hitObj->getIntersection(r);

  return Phong(hitObj->getNormal(intersection), intersection, r, mat, hitObj);

  //hitObj->getMaterial();

 

  //return Shade(r, P, material);



  return rad;
}

// Local Phong illumination at a point.
Color RayTracer::Phong(Point normal,Point p, Ray r, Material * m, Object * o)
{
  Color ret = Color(0.0, 0.0, 0.0, 0.0);
  
  // YOUR CODE HERE.
  // There is ambient lighting irrespective of shadow.
  // For each Light Source L do:
  // Point pointLight =  scene->getNextLight();
  vector <Point> pointLights =  scene->lights;


  Color LightColor = Color(1.0,1.0,1.0,1.0); // White light

  float LightPower = 30.0f;

  Color ambientColor = m->getAmbient(p);
  //ret = ret + ambientColor;


  for (int i = 0 ;i < pointLights.size(); i++)
  {
    // If L visable
    // Draw ray from light to object, end if blocked by an object


    Ray lightRay(pointLights[i],p); // Ray from light to Object

    Object* hitObj = intersect(lightRay);
    if (hitObj == o)
    {
      cout << "\n Lightx: " << pointLights[i].x << endl;
      cout << "\n Lighty: " << pointLights[i].y << endl;

      // Add difuse color
      //ret = ret + ambientColor;

    // Calculate Radance, rad at point
    // rad = rad + Radiance at point
    // Code taken from Assignment 3 shader

    //vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;


    // Vec3 LightColor = vec3(1,1,1); // White light
    // float LightPower = 30.0f;
    // vec3 lightPos = vec3(4,0,4);

    //ret = ret + pointLights[i]
      // Something blocking light path


    }


  }


  //return ret;


  // Specular-diffuse lighting only if the point is not in shadow
  
  // Remember, you need to account for all the light sources.
  
  if (typeid(*o) == typeid(Triangle) )
    ret = Color(0.1,1.0,0.0,0.0);
  else
    ret = Color(0.1,1.0,1.0,0.0);
    //Color ambientColor = m->getAmbient(p);

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

