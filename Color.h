#include <algorithm>
using namespace std;
class Color {
    
public:
    float r,g,b,a;
    Color(){r = g = b = a = 0.0;}
    Color(float,float,float,float);
    Color operator+(Color);
    Color operator*(double);
    void clamp(float); // clamp each component of the color to the provided floating value.
    
};
