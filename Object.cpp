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

// Point Triangle::getIntersection(Ray r){

// 	// YOUR INTERSECTION CODE HERE.
// 	// RETURN THE POINT OF INTERSECTION FOR THIS TRIANGLE.

//     // Step 1: finding P
//     // x(t) = p0 + tv
    
//     Point p0 = r.p;
//     Point directionVector = r.v;

//     //Point normal = this->n;
//     Point normal = getNormal(n);

//     float area2 = normal.length(); 

//     // check if ray and plane are parallel ?
//     float NdotRayDirection = normal*directionVector;

//     // not need probs?
//     if (fabs(NdotRayDirection) < 1e-8 ) // almost 0
//         return Point::Infinite(); // they are parallel


//     // compute distance parameter using equation 2

//     // D is the distance from the origin (0, 0, 0) to the plane (if we trace a line from the origin to the plane, parallel to the plane's normal). 
//     float distance = (normal * p1); // Get d, Dot Product of p1 and Normal

//     // // Calculate t
//     // // t is distance from the ray origin O to P
//     // float t = ((normal * p0) + distance) / (normal * directionVector);    

//     float t = -((normal * p0) + distance) / (normal * directionVector);    
    
//      if (t < 0)   
//         return Point::Infinite(); // triangle is behind the ray cast


//     // compute the intersection point using equation 1
//     // p = orig + t * dir;
//     Point intersectionPoint ((p0.x) + t * directionVector.x, (p0.y) + t * directionVector.y, (p0.z) + t * directionVector.z);
//     //Point intersectionPoint ((p0.x + t) * directionVector.x, (p0.y + t) * directionVector.y, (p0.z + t) * directionVector.z);

//     // Step 2: inside-outside testing

//     Point C; // vector perpendicular to triangle's plane

//     // edge 0
//     Point edge0 = p2 - p1;
//     Point vp0 = intersectionPoint - p1;
//     C = (edge0.cross(vp0));

//     if (normal * C < 0) 
//         return Point::Infinite(); // intersectionPoint is outside

//     // edge 1
//     Point edge1 = p3 - p2;
//     Point vp1 = intersectionPoint - p2;
//     C = edge1.cross(vp1); 
    
//     float u = C.length() / area2;
//     if ((u = (normal * C)) < 0) 
//         return Point::Infinite(); // intersectionPoint is outside

//     // edge 2
//     Point edge2 = p1 - p3;
//     Point vp2 = intersectionPoint - p3;

//     C = edge2.cross(vp2);
//     float v = C.length() / area2;
//     if ((v = (normal * C)) < 0) 
//         return Point::Infinite(); // P is on the right side

//     float denom = normal*normal; 
//     u /= denom;
//     v /= denom;

//     Point p (u,v,p2.z); // dont know about this

//     return p; // Ray hits triangle!
// }


Point Triangle::getIntersection(Ray r){

	// YOUR INTERSECTION CODE HERE.
    // RETURN THE POINT OF INTERSECTION FOR THIS TRIANGLE.
    
    // Ray Equation: x(t) = p0 + tv
    Point p0 = r.p;
    Point directionVector = r.v;

    Point Origin = p1 - p0;
    Point edge1 = p1 - p3;
    Point edge2 = p1 - p2;

   // Check Rules:
    // 1) a1 > 0
    // 2) a2 > 0
    // 3) a1 + 12 < 1

    // Calculate the Determinate
    double det = (edge2.x * ((edge1.y * directionVector.z) - (directionVector.y * edge1.z)));
    det += edge2.y * ((directionVector.x * edge1.z) - (edge1.x * directionVector.z));
    det += edge2.z * ((edge1.x * directionVector.y) - (edge1.y * directionVector.x));


    if (det < EPS && det > -EPS)  // Between the min and max bounds
    {
        return Point::Infinite(); // Return point that will not be rendered
    }

    // Calculate a1
    double a1 = (Origin.x * ((edge1.y * directionVector.z) - (directionVector.y * edge1.z)));
    a1 += Origin.y * ((directionVector.x * edge1.z) - (edge1.x * directionVector.z));
    a1 += Origin.z * ((edge1.x * directionVector.y) - (edge1.y * directionVector.x));
    a1 /= det;

    if (a1 < EPS || a1 > 1) // Check bounds
    {
        return Point::Infinite();
    }
    
    double a2 = (directionVector.z * ((edge2.x * Origin.y) - (Origin.x * edge2.y)));
    a2 += directionVector.y * ((Origin.x * edge2.z ) - (edge2.x * Origin.z));
    a2 += directionVector.x * ((edge2.y * Origin.z ) - (Origin.y * edge2.z));
    a2 /= det;

    if (a2 < EPS || a1 + a2 > 1 ) // a1 + a2 must be < 1
    {
        return Point::Infinite();
    }

    // Intersection!
    Point hitPoint = p1 + ((p2 - p1) * a1) + ((p3 - p1) * a2); 

    return hitPoint;
}

Point Triangle::getNormal(Point p){
    // Point one = p1-p2;
    // Point two = p1-p3;
    // Point ret = one.cross(two);
    // ret.normalize();
    n.normalize();
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
    
    Point intersectionPoint = r.p + (r.v * t);

    //Point intersectionPoint (p0.x + (t0  * directionVector.x), p0.y + (t0 * directionVector.y), p0.z + (t0 * directionVector.z));


    return intersectionPoint; // Ray hits circle!
}


// Point Sphere::getIntersection(Ray r){

//     Point origMinCenter = r.p - center;
//     double discrim = (r.v * origMinCenter) * (r.v * origMinCenter) - 
//                      (r.v * r.v) * ((origMinCenter * origMinCenter) - radius * radius);
//     if (discrim <= EPS)     // less than zero
//         return Point::Infinite();

//     double sqrtDiscrim = sqrt(discrim);
//     double tPos = (-(r.v * origMinCenter) + sqrtDiscrim) /  (r.v * r.v);
//     double tNeg = (-(r.v * origMinCenter) - sqrtDiscrim) /  (r.v * r.v);

//     double t;
    
//     // if (tPos <= EPS && tNeg <= EPS)
//     //     return Point::Infinite();
//     // else if (tPos <= EPS)
//     //     t = tNeg;
//     // else if (tNeg <= EPS)
//     //     t = tPos;
//     // else
//     t = tPos < tNeg ? tPos : tNeg;

//     Point hit = r.p + (r.v * t);
// return hit;
// }

    // double term1 = (directionVector * len) * (directionVector * len);
    // double term2 = a * (len * len) - radius * radius;

    // double discr = term1 - term2;

    // if (discr <= EPS)
    // {
    //     return Point::Infinite();
    // }

    // double t0 = (-(directionVector * len) + sqrt(discr)) /  (directionVector*directionVector);
    // double t1 = (-(directionVector * len) - sqrt(discr)) /  (directionVector*directionVector);