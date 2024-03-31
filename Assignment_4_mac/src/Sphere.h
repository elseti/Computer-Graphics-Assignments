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
        
        vec3 radiusVector = vec3(radius, radius, radius);

        // printVector("radiusVector", radiusVector);
        // printVector("center", center);
        // printVector("direction", r.getDirection());
        // printVector("origin", r.getOrigin());
        // cout << "tmin " << tmin << endl;

        // set up variables
        vec3 d = r.getDirection();
        vec3 o = r.getOrigin();
        vec3 s = center;

        // set up for quadratic equations ( (-b +/- sqrt(b^2 - 4ac)) / 2a )
        float a = dot(d, d);
        float b = 2.0f * dot(d, (o - s));
        float c = dot((o - s), (o - s)) - radius * radius;

        // find determinant and solve quadratic equations
        float det = (b * b) - (4.0f * a * c);

        // if det > 0, have 2 intersections
        if (det >= 0){
            float t1 = (- b + sqrt(det)) / (2.0f * a);
            float t2 = (- b - sqrt(det)) / (2.0f * a);
            if(t1 > tmin && t1 < h.getT()){
                // update h's ray to t, material, and normal
                vec3 pointOnSurface = r.pointAtParameter(t1); // returns o + td
                vec3 normal = glm::normalize(pointOnSurface - center);
                h.set(t1, material, normal);
                return true;
            }
            if(t2 > tmin && t2 < h.getT()){
                // update h's ray to t, material, and normal
                vec3 pointOnSurface = r.pointAtParameter(t2); // returns o + td
                vec3 normal = glm::normalize(pointOnSurface - center);
                h.set(t2, material, normal);
                return true;
            }
        }

        // // if det == 0, have 1 intersection
        // else if (det == 0){
        //     float t1 = (- b + sqrt(det)) / (2.0f * a);
        //     float t2 = (- b - sqrt(det)) / (2.0f * a);
        //     return true;
        // }

        // if det < 0, no intersections
        else {
            return false;
        }

        return false;
	}

    void printVector(string title, vec3 vector ){
        cout << title << ": ";
        for(int i=0; i<3; i++){
            cout << vector[i] << ", ";
        }
        cout << endl;
    }

    // for(int i=0; i<5; i++){
    //     cout << myObject.vertices[0].v[i] << endl;
    // }

    // for (const auto& element : myObject.vertices.v) {
    //     cout << "jointMatList" << endl;
    //     for (int i = 0; i < 4; ++i) {
    //         for (int j = 0; j < 4; ++j) {
    //             std::cout << element[i][j] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }

protected:
    vec3 center;
    float radius;
  

};


#endif
