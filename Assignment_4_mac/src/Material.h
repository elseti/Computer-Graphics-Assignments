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
      vec3 totalVec = vec3(0.0f, 0.0f, 0.0f);
      
      vec3 normal = normalize(hit.getNormal());
      
      // calculate diffuse lighting
      float diffuseIntensity = dot(normal, dirToLight);
      vec3 diffuseVec = diffuseColor * diffuseIntensity * lightColor;

      if(diffuseIntensity > 0){
        totalVec += diffuseVec;
      }

      // calculate reflection using r = 2n(n.l)-l (l is light direction)
      vec3 reflectionVec = 2.0f * normal * dot(normal, dirToLight) - dirToLight;

      // calculate specular lighting and pow shininess
      float specularIntensity = dot(-normalize(ray.getDirection()), reflectionVec);
      vec3 specularVec = specularColor * pow(specularIntensity, shininess) * lightColor;

      if(diffuseIntensity > 0 && specularIntensity > 0){
        totalVec += specularVec;
      }
      
      return totalVec;
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
