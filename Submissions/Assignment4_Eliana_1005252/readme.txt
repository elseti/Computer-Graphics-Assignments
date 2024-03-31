Assignment 4 - README (Eliana Setiabudi, 1005252)

======================= 3.1 - Ray Intersection =======================

======================= Ray-Sphere Intersection  =======================

1. Substituted r(t) = o + td into the equation of a sphere, (x - s_x)^2 + (y - s_y)^2 + (z - s_z)^2 = r^2, to derive a quadratic equation d^2t^2 + 2d(o-s)t + (o-s)(o-s)-r^2 = 0.

2. Check from determinant (b^2-4ac) if there is a solution. 

3. If there is a solution, use the quadratic formula to find t.

4. If t > tmin and t2 < t value in hit, set solutions of t, material, and normal into hit.

4. Return if ray intersects or not (if have solution or not).


======================= Ray-Plane Intersection  =======================

1. Substituted r(t)=o+td  into the equation of plane, (p-s)n, to derive t=(d-no)/nd.

2. Check if t is more than tmin and less than t value in hit. If true, set the found t solution, material, and normal.


======================= Ray-Triangle Intersection  =======================

1. Used Moller-Tumbore intersection algorithm that utilises barycentric coordinates.

2. First check if ray is parallel to triangle. If parallel, return false. 

3. Use barycentric coordinates and find the coefficients in P = wv1 + uv2 + vv3 through edge cross products and replace w with 1-u-v since all w+u+v=1.

4. Find u and v, the other coefficients, and if they are outside of triangle, return false.

5. Compute t to find where the intersection is, and if t > tmin && t < h.getT(), find the final coefficient w by w=1-u—v, calculate normals, and set t, material, and normal to h. Return true.

======================= Transform  =======================

1. Create inverse of matrix.

2. Transform ray origin and ray direction into vec4 / mat4, and make a new ray with the transformed vectors.

3. If the ray intersects with object, normalise and transpose normal and set it to h.


======================= 3.2 - Lighting Computations =======================

======================= Material  =======================

1. Calculate diffuse lighting based on I(k_d(NL)) where I is light colour, k_d is diffuse colour, N is normal vector, and L is normalised vector from surface to light. Add to the resulting light vector if diffuse intensity > 0.

2. Calculate reflection vector using r=2n(nl)-l where n is normal and l is light direction.

3. Calculate specular lighting based on I(k_s(VR)^n)) where I is light colour, k_s is specular colour, V is vector from surface to the viewer, R is reflection vector, and n is shininess. Add to the resulting light vector if specular intensity > 0.

4. Return total light vector, which is effectively I(k_d(NL)+k_s(VR)^n)).
