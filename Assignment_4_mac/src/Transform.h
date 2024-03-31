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
      // create inverse of matrix
      mat4 matInv = inverse(m);

      // transform ray origin
      vec4 oVec4 = vec4(r.getOrigin(), 1.0f);
      vec3 oTrans = vec3(matInv * oVec4);

      // transform ray direction
      vec4 dVec4 = vec4(r.getDirection(), 0.0f);
      vec3 dTrans = vec3(matInv * dVec4);

      // make new ray with the transformed
      Ray transRay = Ray(oTrans, dTrans);

      // transform normal
      vec4 n = vec4(h.getNormal(), 0.0f);
      vec3 nTrans = normalize(vec3(transpose(matInv) * n));

      // delegate to intersect function of object
      if (o->intersect(transRay, h, tmin)) {
        h.set(h.getT(), h.getMaterial(), nTrans);
        return true;
      }

      return false;
  
    }

 protected:
    Object3D* o; //un-transformed object
    mat4 m;
};

#endif //TRANSFORM_H
