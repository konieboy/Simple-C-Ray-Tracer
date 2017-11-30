#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <cmath>

// Helper class to represent points and vectors.

class Point {
    
public:
    double x,y,z;
    Point(double,double,double);
    Point(){x = y = z = 0.0;}
    Point cross(Point);
    void normalize(void);
    bool operator < (Point);
    Point operator - (Point);
    double operator * (Point);
    double length(void);
    Point operator * (double);
    Point operator + (Point);
    static Point Infinite(void);
};

// A class that represents a ray

class Ray {
    
public:
    Ray();
    Ray(Point,Point);
    // reflect ray wrt normal and point on ray
    Ray reflect(Point, Point);
    Point p,v;  // p is the starting point, v is the (un-normalized) direction vector.

    // Can add a transmit function here.

};

#endif

