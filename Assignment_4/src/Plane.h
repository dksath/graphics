#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>
using namespace std;


class Plane: public Object3D
{
public:
	Plane(){}

	Plane( const vec3& normal , float d , Material* m):Object3D(m)
    {
        this->normal = glm::normalize(normal);
        this->d = -d;
	}

	~Plane(){}

    /// TODO: implement this function for ray-plane intersection test.
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {   

        vec3 origin = r.getOrigin();
        vec3 direction = r.getDirection();
        float t = -(this-> d + glm::dot(this-> normal, origin) ) / glm::dot(this-> normal, direction);
        if (t > tmin && t < h.getT())
        {
            h.set(t, this->material, this->normal);
            return true;
        }
        return false;
	}

protected:
    vec3 normal;
    float d;

    
};
#endif //PLANE_H
		

