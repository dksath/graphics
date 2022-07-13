#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vecmath.h"
#include "Object3D.h"

class Transform: public Object3D
{
public: 
    Transform(){}

    Transform( const mat4& m, Object3D* obj ):o(obj)
    {
      this->m = m;
      // this->o = obj;
    }

    ~Transform()
    {

    }
    /// TODO: implement this function so that the intersect function first transforms the ray into the object's local coordindate frame
    virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {   
        vec3 origin = r.getOrigin();
        vec3 direction = r.getDirection();
        glm:: mat4 inv = glm::inverse(this-> m);
        vec3 trans_origin = (inv * vec4(origin, 1.0f));
        vec3 trans_direction = (inv*vec4(direction,0.0f));
        Ray trans_ray = Ray(trans_origin, trans_direction);

        if (o->intersect(trans_ray, h, tmin)) {
          vec3 trans_normal = glm::normalize(glm::transpose(inv)*glm::vec4(h.getNormal(),0.0f));
          h.set(h.getT(), h.getMaterial(), trans_normal);
          return true;
        }
        return false;
    }

 protected:
    Object3D* o; //un-transformed object
    mat4 m;
};

#endif //TRANSFORM_H
