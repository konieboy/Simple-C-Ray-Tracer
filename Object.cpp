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


// Code translated from:
// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
Point Triangle::getIntersection(Ray r)
{
    Point vertex0 = p1;
    Point vertex1 = p2;  
    Point vertex2 = p3;
    Point edge1, edge2, h, s, q;
    Point rayVector = r.v;
    Point rayOrigin = r.p;

    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = rayVector.cross(edge2);
    a = edge1* (h);
    if (a > -EPS && a < EPS)
        return Point::Infinite();
    f = 1/a;
    s = rayOrigin - vertex0;
    u = f * (s*(h));
    if (u < 0.0 || u > 1.0)
        return Point::Infinite();
    q = s.cross(edge1);
    v = rayVector * f * q;
    if (v < 0.0 || u + v > 1.0)
        return Point::Infinite();
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = edge2 * f  * q;
    if (t > EPS) // ray intersection
    {
        Point outIntersectionPoint = rayOrigin + rayVector * t; 
        return outIntersectionPoint;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return Point::Infinite();
}

Point Triangle::getNormal(Point p){
    n.normalize(); // Normal is given so just use it
    return n;
}

Point Sphere::getNormal(Point p){
    Point ret = (p-center);
    
    ret.normalize();
    return ret;
}

// Function to solve a quadratic function
// Code from https://www.scratchapixel.com/code.php?id=10&origin=/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes
bool solveQuadratic(const double &a, const double &b, const double &c, double &t0, double &t1)
{
    double discr = b * b - 4.0 * a * c;
    if (discr < 0) 
        return false;
    else if (discr == 0) 
    {
        t0 = t1 = - 0.5 * b / a;
    }
    else 
    {
        double q = (b > 0) ?
        -0.5 * (b + sqrt(discr)) :
        -0.5 * (b - sqrt(discr));
        t0 = q / a;
        t1 = c / q;
    }

    return true;
} 

Point Sphere::getIntersection(Ray r){

	// YOUR INTERSECTION CODE HERE.
    // RETURN THE POINT OF INTERSECTION FOR THIS SPHERE.

    double t0 = 0, t1 = 0;
    
    Point p0 = r.p;
    Point directionVector = r.v;

    //Point normal = this->n;
    Point normal = getNormal(p0);

    Point len = p0 - center; // Distance to the center


    double a = directionVector*directionVector; // v^2

    double b =  (directionVector * len ) * 2;

    //c = ||p0 - c||^2 - r^2
    
    double c = len*len - (radius*radius); // Center of the Circle

    if (solveQuadratic(a, b, c, t0, t1) == false)
    {
        return Point::Infinite();  
    }

    if (t0 > t1) std::swap(t0, t1); // Use bigger result

    double t = t0;

    //Point intersectionPoint(t0,t1,p0.z);
    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return Point::Infinite(); // both t0 and t1 are negative
    } 

    Point intersectionPoint = r.p + (r.v * t);

    //Point intersectionPoint (p0.x + (t0  * directionVector.x), p0.y + (t0 * directionVector.y), p0.z + (t0 * directionVector.z));

    return intersectionPoint; // Ray hits circle!
}
