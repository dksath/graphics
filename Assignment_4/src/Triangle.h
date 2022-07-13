#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>
#include <iostream>

using namespace std;

class Triangle: public Object3D
{
public:
	Triangle();
    ///@param a b c are three vertex positions of the triangle
	Triangle( const vec3& a, const vec3& b, const vec3& c, Material* m):Object3D(m)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        hasTex = false;
	}

    /// TODO: implement this function for ray-triangle intersection test.
	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin)
    {
        vec3 origin = ray.getOrigin();
        vec3 direction = ray.getDirection();
        vec3 AX_edge = this->a - origin;
        vec3 AB_edge = this->a - this->b;
        vec3 AC_edge = this->a - this->c;

        //find alpha,beta,gamma using formula
        float area = glm::determinant(glm::mat3(AB_edge,AC_edge,direction));
        float beta = glm::determinant(glm::mat3(AX_edge,AC_edge,direction)) / area;
        float gamma = glm::determinant(glm::mat3(AB_edge,AX_edge,direction)) / area;
        float t = glm::determinant(glm::mat3(AB_edge,AC_edge,AX_edge)) / area;
        

        if (t > tmin && t < hit.getT() && beta + gamma <= 1.0f && beta >= 0.0f && gamma >= 0.0f)
        {   
            float alpha = 1.0f - beta - gamma;
            hit.set(t, this->material, glm::normalize(alpha *normals[0] + beta *normals[1] + gamma *normals[2]));

            return true;
        }

        return false;

    }
    
	bool hasTex;
	vec3 normals[3];
	vec2 texCoords[3];
    
protected:
    vec3 a;
    vec3 b;
    vec3 c;

};

#endif //TRIANGLE_H
