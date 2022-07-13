#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>

#include <iostream>
using namespace std;

class Sphere: public Object3D
{
public:
	Sphere()
    { 
		//unit ball at the center
        this->center = vec3(0,0,0);
        this->radius = 1.0;
	}

	Sphere( vec3 center , float radius , Material* material ):Object3D(material)
    {
        this->center = center;
        this->radius = radius;
	}
	

	~Sphere(){}

    /// TODO: implement this function for ray-sphere intersection test.
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {   
        // new instance of origin
        vec3 origin = r.getOrigin() - this ->center;

        // solve quadratic equation || o + td - c || - r = 0 to find t
        const float a  = glm::dot(r.getDirection(),(r.getDirection()));
        const float b = 2.0 * glm::dot(r.getDirection(),(origin));
        const float c = glm::dot(origin, origin) - this->radius * this->radius;

        bool intersected = false;
        //check real root
        if (pow(b,2) - 4 * a * c >= 0)
        {
            float t1 = (-b + sqrt(pow(b,2) - 4 * a * c)) / (2 * a);
            float t2 = (-b - sqrt(pow(b,2) - 4 * a * c)) / (2 * a);
            if (t1 > tmin && t1 < h.getT())
            {
                h.set(t1, this->material, glm::normalize(r.pointAtParameter(t1) -this ->center));
                intersected = true;
            }
            if (t2 > tmin && t2 < h.getT())
            {
                h.set(t2, this->material, glm::normalize(r.pointAtParameter(t2) - this ->center));
                intersected = true;
            }
        }
        return intersected;

	}

protected:
    vec3 center;
    float radius;
  

};


#endif
