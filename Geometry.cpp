#include "Geometry.h"

// Implementation of Point and Ray classes.

Point Point::Infinite(){
    return Point(1E20,1E20,1E20);
}
Point Point::cross(Point p){
    return Point(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);   
}

void Point::normalize(){
    double l = this->length();
    x /= l;
    y /= l;
    z /= l;
}

Point::Point(double xo, double yo, double zo){
    x = xo;
    y = yo;
    z = zo;
}

Point Point::operator- (Point p){
    return Point(x-p.x,y-p.y,z-p.z);   
}

Point Point::operator*(double s){
    return Point(x*s,y*s,z*s);   
}

Point Point::operator+(Point p){
    return Point(p.x+x,p.y+y,p.z+z);   
}

bool Point::operator < (Point p){
    return (this->length() < (p).length());
}

// Dot product
double Point::operator* (Point p){
    return x*p.x+y*p.y+z*p.z;   
}

double Point::length(){
    return sqrt(x*x+y*y+z*z);
}

Ray::Ray(){
}

Ray::Ray(Point po, Point vo){
    p = po;
    v = vo;
    v.normalize();
}

Ray Ray::reflect(Point normal, Point m){
    Ray ret;
    double tmp = -(normal*v);
    
    Point vo = v+(normal*tmp*2.0);
    
    Point p = Point(m.x,m.y,m.z);
    return Ray(p,vo);
}
