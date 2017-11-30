#include "Object.h"
#include <iostream>


void Object::setMaterial(Material * mat){
    m = mat;   
}

Triangle::Triangle(Point v1,Point v2, Point v3, Point no){
    p1 = v1;
    p2 = v2;
    p3 = v3;
    
    n = no;
    isLight = false;
}

Sphere::Sphere(Point p, double r){
    center = p;
    radius = r;
    
    isLight = false;
}
#define EPS 1E-6

Point Triangle::getIntersection(Ray r){

	// YOUR INTERSECTION CODE HERE.
	// RETURN THE POINT OF INTERSECTION FOR THIS TRIANGLE.

    // Step 1: finding P

    // x(t) = p0 + tv
    
    Point p0 = r.p;
    Point directionVector = r.v;

    //Point normal = this->n;
    Point normal = getNormal(n);

    float area2 = normal.length(); 

    // check if ray and plane are parallel ?
    float NdotRayDirection = normal*directionVector;

    // not need probs?
    if (fabs(NdotRayDirection) < 1e-8 ) // almost 0
        return Point::Infinite(); // they are parallel


    // compute distance parameter using equation 2

    // D is the distance from the origin (0, 0, 0) to the plane (if we trace a line from the origin to the plane, parallel to the plane's normal). 
    float distance = normal * (this->p1); // Get d, Dot Product of p1 and Normal

    // // Calculate t
    // // t is distance from the ray origin O to P
    float t = -((normal * p0) + distance) / (normal * directionVector);    
    
     if (t < 0)   
        return Point::Infinite(); // triangle is behind the ray cast


    // compute the intersection point using equation 1
    // p = orig + t * dir;
    Point intersectionPoint ((p0.x + t) * directionVector.x, (p0.y + t) * directionVector.y, (p0.z + t) * directionVector.z);

    // Step 2: inside-outside testing

    Point C; // vector perpendicular to triangle's plane

    // edge 0
    Point edge0 = p2 - p1;
    Point vp0 = intersectionPoint - p1;
    C = (edge0.cross(vp0));

    if (normal*(C) < 0) 
        return Point::Infinite(); // P is on the right side

    // edge 1
    Point edge1 = p3 - p2;
    Point vp1 = intersectionPoint - p2;
    C = edge1.cross(vp1); 
    
    float u = C.length() / area2;
    if ((u = (normal * C)) < 0) 
        return Point::Infinite(); // P is on the right side

    // edge 2
    Point edge2 = p1 - p3;
    Point vp2 = intersectionPoint - p3;

    C = edge2.cross(vp2);
    float v = C.length() / area2;
    if ((v = (normal * C)) < 0) 
        return Point::Infinite(); // P is on the right side

    return intersectionPoint; // Ray hits triangle!
}

Point Triangle::getNormal(Point p){
    Point one = p1-p2;
    Point two = p1-p3;
    Point ret = one.cross(two);
    ret.normalize();
    return ret;
}

Point Sphere::getNormal(Point p){
    Point ret = (p-center);
    
    ret.normalize();
    return ret;
}

// Function to solve a quadratic function
// Code from https://www.scratchapixel.com/code.php?id=10&origin=/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) 
        return false;
    else if (discr == 0) 
    {
        x0 = x1 = - 0.5 * b / a;
    }
    else 
    {
        float q = (b > 0) ?
        -0.5 * (b + sqrt(discr)) :
        -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    return true;
} 



Point Sphere::getIntersection(Ray r){

	// YOUR INTERSECTION CODE HERE.
    // RETURN THE POINT OF INTERSECTION FOR THIS SPHERE.

    float t0 = 0, t1 = 0;
    
    Point p0 = r.p;
    Point directionVector = r.v;

    //Point normal = this->n;
    Point normal = getNormal(p0);


    Point len = p0 - center;

    float a = directionVector*directionVector; // v^2

    float b =  (directionVector * len ) * 2;


    //c = ||p0 - c||^2 - r^2

    float c = len*len - (radius*radius);
    if (solveQuadratic(a, b, c, t0, t1) == false)
    {
        return Point::Infinite();  
    }

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0)      
            return Point::Infinite(); // both t0 and t1 are negative
    }


    Point intersectionPoint(t0,t1,p0.z);
    //Point intersectionPoint (p0.x + (t0  * directionVector.x), p0.y + (t0 * directionVector.y), p0.z + (t0 * directionVector.z));


    return intersectionPoint; // Ray hits circle!
}

