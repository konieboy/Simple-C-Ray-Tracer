#include "Scene.h"

void Scene::startIteration(){
    indexO = 0;
}
// N is width/height
// the field of view is specified in degrees.
Scene::Scene(Point * vo, Point * no, double f, int No){
    v = vo;
    n = no;
    fov = f*acos(-1)/180.0;

    // scale n to account for implicit image plane.
    *n = (*n)*(1/(2*tan(fov/2.0)));
    Point uo = v->cross(*n);
    uo.normalize();
    u = new Point(uo.x,uo.y,uo.z);
    C = Point(0,0,0);
    N = No;

    indexO = 0;
    indexL = 0;
}

Object * Scene::getNextObject(){
    if(indexO == objects.size()) return NULL;
    return objects[indexO++];
}

Point Scene::getNextLight(){
    indexL++;
    return lights[indexL%lights.size()];    
}

void Scene::addObject(Object * o){
    objects.push_back(o);   
}

void Scene::addLight(Point l){
    lights.push_back(l);
}

// Sets the origin of the camera
void Scene::setCamera(Point * c){
    camera = c;
}

// A ray through a point on the image plane.
// The location of the image plane in the n-direction is implicitly taken to be the point where
// the width and height of the image plane are both 1. Please see the constructor above.
Ray Scene::makeRay(double xo, double yo){
    // calculate point on image plane
    double normx = (xo/N)-0.5;
    double normy = (yo/N)-0.5;
    
    Point po = (*u)*normx + (*v)*normy + (* camera) + (*n);
    
    Ray r = Ray(po, po - (*camera));
    return r;
}

// A test scene based on description handed out with the assignment.
// Please modify material properties as necessary to test your ray tracer.
Scene * Scene::initTestScene(int N){

    // make new scene with up vector, direction vector and fov
    Scene * ret = new Scene(new Point(0,1.0,0),new Point(0,0,1.0),55.0,N);  
    // Add in sphere
    Material * test = new Material();
    test->type = DIFFUSE;
    test->kr = 0.03;
    test->ambient = Color(0.0,0.0,0.0,1.0);
    test->diffuse = Color(0.0,0.6,0.6,1.0);
    test->specular = Color(0.2,0.2,0.2,1.0);
    
    Material * test1 = new Material();
    test1->kr = 0.03;
    test1->type = DIFFUSE;
    test1->ambient = Color(0.3,0.3,0.3,1.0);
    test1->diffuse = Color(0.8,0.8,0.8,1.0);
    test1->specular = Color(0.2,0.2,0.2,1.0);
    Material * test2 = new Material();
    test2->kr = 0.25;
    test2->type = DIFFUSE;
    test2->ambient = Color(0.4,0.25,0.25,1.0);
    test2->diffuse = Color(0.5,0.1,0.1,1.0);
    test2->specular = Color(0.2,0.2,0.2,1.0);

    Material* sphere = new Material();
    sphere->type = REFLECTIVE;
    // give sphere some reflectivity;
    sphere->kr = 0.85;
    sphere->ambient = Color(0.00,0.00,0.00,1.0);
    sphere->diffuse = Color(0.0,0.0,0.0,1.0);
    sphere->specular = Color(0.0,0.0,0.0,1.0);
    Object * s1 = new Sphere(Point(400.0,130.0,320.0),120.0);
    // Make points for square
    Point p1 = Point(0,0,0);
    Point p2 = Point(550,0,0);
    Point p3 = Point(0,0,560);
    Point p4 = Point(550,0,560);
    Point p5 = Point(560,550,0);
    Point p6 = Point(560,550,560);
    Point p7 = Point(0,550,560);
    Point p8 = Point(0,550,0);
    // make normals for triangle
    Point n1 = Point(0.0,-1.0,0.0);
    Point n2 = Point(0.0,1.0,0.0);
    Point n3 = Point(1.0,0.0,0.0);
    Point n4 = Point(-1.0,0.0,0.0);
    Point n5 = Point(0.0,0.0,-1.0);
    s1->setMaterial(sphere);
    ret->addObject(s1);
    // Add in bottom square
    Object * t1 = new Triangle(p1,p3,p2,n2);
    Object * t2 = new Triangle(p4,p2,p3,n2);
    ret->addObject(t1);
    ret->addObject(t2);
    t1->setMaterial(test);
    t2->setMaterial(test);
    // Add in top square
    t1 = new Triangle(p5,p7,p6,n1);
    t2 = new Triangle(p8,p7,p5,n1);
    t1->setMaterial(test);
    t2->setMaterial(test);

    // Add in left square
    t1 = new Triangle(p3,p1,p8,n3);
    t2 = new Triangle(p3,p8,p7,n3);
    t1->setMaterial(test);
    t2->setMaterial(test);
    ret->addObject(t1);
    ret->addObject(t2);

    // Add in back square
    t1 = new Triangle(p4,p3,p7,n5);
    t2 = new Triangle(p4,p7,p6,n5);
    t1->setMaterial(test);
    t2->setMaterial(test);
    ret->addObject(t1);
    ret->addObject(t2);

    // Add in right square
    t1 = new Triangle(p4,p2,p5,n4);
    t2 = new Triangle(p4,p5,p6,n4);
    t1->setMaterial(test2);
    t2->setMaterial(test2);
    ret->addObject(t1);
    ret->addObject(t2);

    // Add light sources
    ret->addLight(Point(185.0,2000.0,169.0));
    ret->addLight(Point(400.0,2000.0,320.0));

    // set Camera location
    ret->setCamera(new Point(278,273,-500));

    // Now we will add in smaller box
    Point v1 = Point(100,165,65);
    Point v2 = Point(52,165,225);
    Point v3 = Point(210,165,272);
    Point v4 = Point(260,165,114);
    Point v5 = Point(260,0,114);
    Point v6 = Point(260,165,114);
    Point v7 = Point(210,165,272);
    Point v8 = Point(210,0,272);
    Point v9 = Point(100,0,65);
    Point v10 = Point(100,165,65);
    Point v11 = Point(52,0,225);
    
    Point no1 = Point(0.0,1.0,0.0);
    Point no2 = Point(0.0,0.0,-1.0);
    Point no3 = Point(-1.0,0.0,0.0);
    Point no4 = Point(1.0,0.0,0.0);
    Point no5 = Point(0.0,0.0,1.0);
    
    t1 = new Triangle(v2,v3,v1,no1);
    t2 = new Triangle(v4,v1,v3,no1);
    ret->addObject(t1);
    ret->addObject(t2);
    t1->setMaterial(test1);
    t2->setMaterial(test1);
    
    t1 = new Triangle(v7,v4,v9,no4);
    t2 = new Triangle(v5,v7,v8,no4);
    t1->setMaterial(test1);
    t2->setMaterial(test1);
    ret->addObject(t1);
    ret->addObject(t2);
    
    t1 = new Triangle(v9,v10,v4,no4);
    t2 = new Triangle(v9,v4,v5,no4);
    t1->setMaterial(test1);
    t2->setMaterial(test1);
    ret->addObject(t1);
    ret->addObject(t2);
    
    t1 = new Triangle(v11,v2,v10,no4);
    t2 = new Triangle(v11,v2,v9,no4);
    t1->setMaterial(test1);
    t2->setMaterial(test1);
    ret->addObject(t1);
    ret->addObject(t2);
    
    t1 = new Triangle(v8,v7,v2,no4);
    t2 = new Triangle(v8,v7,v11,no4);
    t1->setMaterial(test1);
    t2->setMaterial(test1);
    ret->addObject(t1);
    ret->addObject(t2);

    // Test Points
    //   Point p1 = Point(0,0,0);
    // Point p2 = Point(40,0,0);
    // Point p3 = Point(20,100,0);
    
    return ret;
}
