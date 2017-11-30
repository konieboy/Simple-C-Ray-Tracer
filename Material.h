#ifndef COLOR_H
#include "Color.h"
#endif
#ifndef GEOMETRY_H
#include "Geometry.h"
#endif

// An enumerated type to represent different types of materials.
enum Type{
    REFLECTIVE,
    DIFFUSE
};

// A class that represents material properties.
// This can be extended to represent textured materials.
class Material {
    
public:
    Color diffuse, ambient, specular;
    double kr,kt,kd;
    Type type; 
    // this is for materials to decide!
    Material(){
        kr = 0.0;
        kt = 0.0;
        kd = 0.0;
    }
    virtual Color getAmbient(Point p){ return ambient;}
    Color getDiffuse(Point p){return diffuse;}
    Color getSpecular(Point p){return specular;}
    void setAmbient(Color c){ambient = c;}
    void setSpecular(Color c){specular = c;}
    void setDiffuse(Color c){diffuse = c;}
};
