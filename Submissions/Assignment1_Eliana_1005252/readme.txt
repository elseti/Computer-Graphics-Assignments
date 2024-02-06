Assignment 1 - README (Eliana Setiabudi, 1005252)

=====================================================================
======================= PART 1 (MESH LOADING) =======================

Steps:

1. Read OBJ file

2. Iterate each line and get completeFaceList (containing vertex index and normal index from the face list "f", ignoring texture index), normalList (containing all normals "vn"), justVertexList (storing only vertices "v"), and triList (first element of the face list "f").

3. Create an indexList to store the index of a normal as an element, which corresponds/maps to the vertex's index.

4. Combine and store justVertexList and normalList into verList.

5. Close the file.



======================= PART 2 (MESH RENDERING) =======================

Steps:

0. Nothing much. Did kind of read the shader.h.


======================= PART 3 (MESH COLORING) =======================

Steps:

1. Realized that the main loop calls colorTable[colorID][0].

2. Coded the rotation/swap of the color order in colorTable whenever "c" is pressed so that the next color is in index 0.


======================= PART 4 (MESH COLORING) =======================

ROTATE MODEL

Steps:

1. Used Rodrigues' Rotation Formula to compute the rotationMatrix.

2. Multiplied the rotationMatrix with the modelMatrix.


TRANSLATE MODEL

Steps:

1. Created a 4x4 identity matrix.

2. Added the translation vector into the bottom-most translation matrix except the right-most bottom. I.e., [[1,0,0,0], [0,1,0,0], [0,0,1,0], [tx,ty,tz,1]].

3. Multiplied the translation matrix with the modelMatrix.


SCALE MODEL

Steps:

1. Created a 4x4 scale matrix with the scale as a diagonal, i.e. [[sx,0,0,0], [0,sy,0,0], [0,0,sz,0], [0,0,0,1]].

2. Multiplied the scale matrix with the modelMatrix.

=====================================================================


I learnt a lot. It was fun. I do think there should be a better way to load the OBJ file. My method takes too many steps and memory.
