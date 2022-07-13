#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include "vecmath.h"

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"


class Material
{
public:
	
    Material( const vec3& d_color ,const vec3& s_color=vec3(0,0,0), float s=0):
    diffuseColor( d_color),specularColor(s_color), shininess(s)
    {

    }

    virtual ~Material()
      {

      }

    virtual vec3 getDiffuseColor() const 
    { 
      return  diffuseColor;
    }
      
    /// TODO: Implement this function to compute diffuse and specular components of Phong lighting 
    vec3 Shade( const Ray& ray, const Hit& hit, const vec3& dirToLight, const vec3& lightColor ) 
    {
        vec3 color = vec3(0,0,0);  //default color black
        vec3 normal = glm::normalize(hit.getNormal());

        float diffuse_intensity = glm::dot(dirToLight, normal);
        //check if no illumination
        if (diffuse_intensity > 0)
        {
            color += diffuse_intensity * lightColor * this->diffuseColor;
        }

        vec3 reflect =  2*glm::dot(dirToLight, normal)*normal - dirToLight;

        //direction of camera opposite ray
        float specular_intensity = glm::dot(glm::normalize(-ray.getDirection()), reflect);
        //check illumniation
        if (specular_intensity && diffuse_intensity > 0)
        {
            color += pow(specular_intensity, this->shininess) * lightColor * this->specularColor;
        }

        return color;
    }

    void loadTexture(const char * filename)
    {
      t.load(filename);
    }

    Texture t;

 protected:
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};



#endif // MATERIAL_H
