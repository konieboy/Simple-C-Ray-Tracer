#include "Color.h"

// Implementation of the Color class.

Color::Color(float ro, float go, float bo, float ao){
    r = ro;
    g = go;
    b = bo;
    a = ao;
}

Color Color::operator+(Color c){
    return Color(c.r+r,c.g+g,c.b+b,c.a+a);   
}

Color Color::operator*(double s){
    return Color(r*s,g*s,b*s,a*s);   
}

void Color::clamp(float m){
    r = max(0.0f,min(m,r));
    g = max(0.0f, min(m,g));
    b = max(0.0f,min(m,b));
    a = max(0.0f,min(m,a));
}

