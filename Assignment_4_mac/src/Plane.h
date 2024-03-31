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
        this->normal = normalize(normal);
        this->d = -d;
	}

	~Plane(){}

    /// TODO: implement this function for ray-plane intersection test.
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {   
        vec3 n = normal;
        vec3 o = r.getOrigin();

        // equation, solve for t = (d - on) / nd
        float t = (-d - dot(n, o)) / dot(n, r.getDirection()); 

        if(t > tmin && t < h.getT())
        {
            h.set(t, material, normal);
            return true;
        }

        return false;
	}

protected:
    vec3 normal;
    float d;

    
};
#endif //PLANE_H
		

