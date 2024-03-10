Assignment 3 - README (Eliana Setiabudi, 1005252)

======================= 2.1 Planar Parameterisation =======================

Steps:

1. Get xMin, yMin, xMax, yMax of the obj file.

2. Iterate through each vertex, get xPos and yPos.

3. Calculate u and v using formula u = (xPos - xMin) / (xMax - xMin), and v = (yPos - yMin) / (yMax - yMin).

3. Create an indexList to store the index of a normal as an element, which corresponds/maps to the vertex's index.

4. Store (u, v) texture coordinate.



======================= 2.2 Cylindrical Parameterisation =======================

Steps:

1. Iterate through each vertex and get xPos, yPos, and zPos.

2. Derive theta (angle between x-axis and r) from equation x = rcos(theta) and y = rsin(theta) to get theta = arctan(yPos/xPos)

3. Normalise theta so it's between 0 and 1.


======================= 2.3 Spherical Parameterisation =======================

Steps:

1. Iterate through each vertex and get xPos, yPos, and zPos.

2. Calculate theta by calculating arctan(yPos/xPos).

3. Derive rho by rearranging x^2 + y^2 + z^2 = rho * sin(phi) * cos(theta) + rho * sin(phi) * sin(theta) + rho * cos(phi), in terms of x, y, z, to get rho = sqrt(x^2 + y^2 + z^2).

4. Derive phi rearranging z = rho * cos(phi) to get phi = arccos(z/rho).

5. Normalise angle so it's between 0 and 1.

