Assignment 2 - README (Eliana Setiabudi, 1005252)

=====================================================================
======================= PART 1 (Load Skeleton File) =======================

Steps:

1. Read .skel file

2. Iterate through each line in the file and extract the 4 fields of the joint from the line. 

3. Create a transformation matrix for the joint, declare a new joint, and store the transformation matrix in its transform attribute.

4. Assign the joint to m_rootJoint if the parent index is -1. Else, if it's the first joint, assign the current joint as m_rootJoint's child. If it's not the first joint and not the root joint, assign the current joint as jointFieldList[parentIndex]'s child.

5. Store the joint in m_joints.

5. Close the file.



======================= PART 2 (Draw Skeleton) =======================

(computeJointTransforms)

Steps:

1. Push transposed joint's transform matrix into matrixStack.

2. Push matrixStack.top() into jointMatList.

3. If current joint has no children, pop stack and return.

4. Else, for every joint's children, recursively call computeJointTransforms().


(computeBoneTransforms)

Steps:

1. Push transposed joint's transform matrix into matrixStack.

2. For every child in the joint's children:

a. Get vector and its length of the child's transform.
b. Create translation matrix to translate the cylinders' z by 0.5 to align them.
c. Scale bone by (0.01f, 0.01f, boneLength) so it will be smaller and scaled to bone's length.
d. Get local axes and direction by finding normals of the child's transformation.
e. Make rotation matrix using the vectors.
f. Compute transformation matrix by multiplying all the matrices.
g. Push the transformation matrix into the matrixStack.
h. Push matrixStack.top() into boneMatList.
i. Pop the matrixStack.
j. Recursively call computeBoneTransforms().


======================= PART 3 (CHANGE POSE OF SKELETON) =======================

Steps:

1. Get the transformation matrix of the joint at the jointIndex.

2. Make rotation matrices for rotation in X, Y, and Z axes using glm::rotate library.

3. Get the transformation matrix by multiplying them in a ZYX order.

4. Replace the transform of the joint with the new transformation matrix.

=====================================================================
