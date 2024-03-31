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
	virtual bool intersect( const Ray& r,  Hit& h , float tmin)
    {
        /*
        vec3 constCol = this->a - r.getOrigin();
        vec3 betaCol = this->a - this->b;
        vec3 gammaCol = this->a - this->c;
        vec3 tCol = r.getDirection();

        float coefDet = determinant(glm::mat3(betaCol, gammaCol, tCol));
        float betaDet = determinant(glm::mat3(constCol, gammaCol, tCol));
        float gammaDet = determinant(glm::mat3(betaCol, constCol, tCol));
        float tDet = determinant(glm::mat3(betaCol, gammaCol, constCol));

        float beta = betaDet / coefDet;
        float gamma = gammaDet / coefDet;
        float t = tDet / coefDet;

        if (t > tmin && t < h.getT() && beta + gamma <= 1.0f && beta >= 0.0f && gamma >= 0.0f) {
            float alpha = 1.0f - beta - gamma;
            vec3 normal = alpha * normals[0] + beta * normals[1] + gamma * normals[2];
            h.set(t, this->material, glm::normalize(normal));
            return true;
        }

        return false;
        */

        
        // using Moller-Trumbore intersection algorithm
        // set up variable
        float epsilon = std::numeric_limits<float>::epsilon();
        vec3 d = r.getDirection();
        vec3 o = r.getOrigin();

        // check if ray is parallel to the triangle
        vec3 edge1 = b - a;
        vec3 edge2 = c - a;
        vec3 rayCrossEdge2 = cross(d, edge2);
        float det = dot(edge1, rayCrossEdge2);

        // if it is parallel, then return false.
        if(det > -epsilon && det < epsilon){
            return false;
        }

        // using barycentric coordinates: P = wv1 + uv2 + vv3
        // replace w = 1 - u - v since all w+u+v = 1
        float invDet = 1.0f / det;
        vec3 s = o - a;

        // find u
        float u = invDet * dot(s, rayCrossEdge2);

        // outside of triangle
        if(u < 0 || u > 1){
            return false;
        }  
        
        // find v
        vec3 sCrossEdge1 = cross(s, edge1);
        float v = invDet * dot(d, sCrossEdge1);

        // outside of triangle
        if(v < 0 || u + v > 1){
            return false;
        }

        // compute t to find where intersection is
        float t = invDet * dot(edge2, sCrossEdge1);
        if(t > tmin && t < h.getT()){
            float w = 1.0f - u - v;
            vec3 normal = u * normals[0] + v * normals[1] + w * normals[2];
            h.set(t, material, normalize(normal));
            return true;
        }

        return false;   // remove this sentence in your implementation
        
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
