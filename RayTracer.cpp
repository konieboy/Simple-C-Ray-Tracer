#include "RayTracer.h"
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <random>

using namespace std;
#define EPS 1E-2

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
  if (depth > this->maxdepth)
  {
    return rad; //End recurssion
  }

  Object* hitObj = intersect(r);
  if (hitObj == NULL)
  {     
    return rad; // No object not hit by ray
  }

  // Determine the material type of that object 
  Material* mat = hitObj->getMaterial();

  Point intersection =  hitObj->getIntersection(r);

  rad = Phong(hitObj->getNormal(intersection), intersection, r, mat, hitObj);



  // Recursive Reflection
  if (mat->type == REFLECTIVE)
  {
    depth += 1;

    Ray reflectedRay = r.reflect(hitObj->getNormal(intersection), intersection);
    reflectedRay.p = reflectedRay.p + (reflectedRay.v * EPS); // Move along a smidge

    rad = rad + RayTracer::trace(reflectedRay, depth);
  }

  //hitObj->getMaterial();

  //return Shade(r, P, material);

  return rad;
}

// Local Phong illumination at a point.
// Coded based of https://steveharveynz.wordpress.com/category/programming/c-raytracer/

Color RayTracer::Phong(Point normal, Point p, Ray r, Material * m, Object * o)
{
  Color ret = Color(0.0, 0.0, 0.0, 1.0);
  normal.normalize();
  // YOUR CODE HERE.
  // There is ambient lighting irrespective of shadow.
  // For each Light Source L do:
  // Point pointLight =  scene->getNextLight();
  vector <Point> pointLights =  scene->lights;


  Color LightColor = Color(1.0,1.0,1.0,1.0); // White light
  //LightColor = LightColor + LightColor;
  double LightPower = 0.33; // Should be based on distance to light maybe?

  // Everything get hits by ambient light
  Color ambientColor = m->getAmbient(p) * LightPower;
   
  Color diffuse =  Color(0.0, 0.0, 0.0, 1.0);
  Color specularHighlight =  Color(0.0, 0.0, 0.0, 1.0);

  for (int i = 0 ;i < pointLights.size(); i++)
  {
    // If L visable
    // Draw ray from light to object, end if blocked by an object

    // Find Direction towards light
    Point lightDirection = (pointLights[i] - p);  
    lightDirection.normalize();

    Ray lightRay(p + lightDirection * EPS, lightDirection); // Ray from light to Object. Point in direction of light and make sure it hits a light


    Object* hitObj = intersect(lightRay);
    
    double reflectiveness = 1000.0 * m->kr;

    // Check if in shadow
    // Object should be in the way of the light

   if (hitObj == NULL || (normal * lightDirection < 0))  
    {
        // Add Difuse component
        Color diffuseColor = m->getDiffuse(p);
        // ret = ret + diffuseColor;

        // // Add specular component
        Color specularColor = m->getSpecular(p);
        // ret = ret + specularColor;

        //double distance = (pointLights[i] - p).length;

        // Calc Diffuse 

        double lDotNormal = normal * lightDirection;

        lDotNormal = lDotNormal < 0 ? 0.0f : lDotNormal; // Makes sure angle > 0
        
        diffuse = diffuse + (diffuseColor * LightPower * lDotNormal);

        diffuse = diffuse + (diffuseColor * LightPower * m ->kd * lDotNormal);

        // Calc Specular
        // SPECULAR
        Point toCamera = r.v * -1;
        toCamera.normalize();

        Point disFromLight = toCamera + lightDirection;
        disFromLight.normalize();   // divide by length
        double specReflec = disFromLight * normal;
        specReflec = specReflec < 0 ? 0.0f : specReflec; // Makes sure angle > 0

        double refectivity = pow(specReflec,  reflectiveness);

        specularHighlight = specularHighlight + specularColor * LightPower * refectivity;       
    }
      
      //else // In shadow, some object blocks light 
      {
        // cout << "\n Lightx: " << pointLights[i].x << endl;
        // cout << "\n Lighty: " << pointLights[i].y << endl;

        // Calculate Radance, rad at point
        // rad = rad + Radiance at point
        // Code taken from Assignment 3 shader

        //ret = ret + pointLights[i]
        // Something blocking light path

      }


  }

  
  // if (typeid(*o) == typeid(Triangle) )
  //   ret = Color(0.1,1.0,0.0,0.0);
  // else
  //   ret = Color(0.1,1.0,1.0,0.0);

  Color finalColor = ambientColor + diffuse + specularHighlight; // Add all components of light together

  return finalColor;
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

