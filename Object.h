#include <vector>
#include <cstdlib>

#include "Material.h"
using namespace std;

// A superclass that represents geometric objects.
// You can extend/modify the class to add support for textured objects.
class Object {
protected:
    Material * m;
    bool isLight;
public:
    Material * getMaterial(){return m;}
    void setMaterial(Material *);
    bool isLightSource(){ return isLight; }
    virtual Point getIntersection(Ray) =0;
    virtual Point getNormal(Point) =0;
};


// A triangle object
class Triangle : public Object {
    
public:
    Point p1,p2,p3,n;
    Point getIntersection(Ray);
    Point getNormal(Point);
    Triangle(Point, Point, Point, Point);
};

// A sphere object
class Sphere : public Object {
    Point center;
    double radius;
public:
    Point getNormal(Point);
    Point getIntersection(Ray);
    Sphere(Point,double);
};


