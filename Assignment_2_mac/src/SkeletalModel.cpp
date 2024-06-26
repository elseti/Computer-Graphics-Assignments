///=========================================================================================///
///
///                       Functions to be filled in for Assignment 2   
///
///           IMPORTANT: you ONLY need to work on functions with TODO in this section    
///    
///=========================================================================================///


#include "SkeletalModel.h"


///=========================================================================================///
///                                    Load .skel File    
///=========================================================================================///


// TODO: Load the skeleton from file here, create hierarchy of joints
//       (i.e., set values for m_rootJoint and m_joints)
void SkeletalModel::loadSkeleton( const char* filename )
{
    string fileContents;
   
    ifstream file(filename);
    // ifstream file("../data/Model1.skel");
    if(!file.is_open()){
        cout << "Error opening skel file" << endl;
    }

    string jointLine;
    while(getline(file, jointLine)){

        // extract the 4 fields from line
        istringstream jointStream(jointLine);
        string jointField;
        vector<float> jointFieldList;
        while(getline(jointStream, jointField, ' ')){
            jointFieldList.push_back(stof(jointField));
        }

        // create a translation matrix for joint
        // Note - transposed matrix
        glm::mat4 jointMatrix = glm::mat4(1.0f);
        jointMatrix[3][0] = jointFieldList[0];
        jointMatrix[3][1] = jointFieldList[1];
        jointMatrix[3][2] = jointFieldList[2];

        // create a new joint and assign its transform
        Joint *currJoint = new Joint;
        currJoint -> transform = jointMatrix;

        // assign to rootJoint if joint is root
        if(jointFieldList[3] == -1){
            m_rootJoint = currJoint;
        }
        else{
            // if it's the first joint, assign the current joint as m_rootJoint's child
            if(jointFieldList[3] == 0){
                m_rootJoint -> children.push_back(currJoint);
            }
            // if not, assign the current joint as m_joint[parentIndex]'s child
            else{
                m_joints[jointFieldList[3]] -> children.push_back(currJoint);
            }
        }
        m_joints.push_back(currJoint);
    }

    file.close();
}




///=========================================================================================///
///                         Compute transformations for Joints and Bones     
///=========================================================================================///

void SkeletalModel::computeTransforms()
{
    if( m_joints.size() == 0 )
        return;

    computeJointTransforms();

    computeBoneTransforms();
}

// Compute a transformation matrix for each joint (i.e., ball) of the skeleton
void SkeletalModel::computeJointTransforms( )
{
    jointMatList.clear();

    m_matrixStack.clear();

    computeJointTransforms(m_rootJoint, m_matrixStack);
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint* joint, MatrixStack matrixStack)
{
    matrixStack.push(glm::transpose(joint->transform)); // note: no need to do multiplication, matrix stack already does it for you.
    jointMatList.push_back(matrixStack.top());
    
    if(joint->children.size() == 0){
        matrixStack.pop();
        return;
    }
    for(int x=0; x < joint->children.size(); x++){
        computeJointTransforms(joint->children[x], matrixStack);
    }
}


// Compute a transformation matrix for each bone (i.e., cylinder) between each pair of joints in the skeleton
void SkeletalModel::computeBoneTransforms( )
{
    boneMatList.clear();

    m_matrixStack.clear();

    computeBoneTransforms(m_rootJoint, m_matrixStack);
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the bones
void SkeletalModel::computeBoneTransforms(Joint* joint, MatrixStack matrixStack)
{

    matrixStack.push(glm::transpose(joint->transform));

    for(int x=0; x < joint->children.size(); x++){
        Joint* child = joint->children[x];

        // get vector of child transform and length
        glm::vec3 childJointTranslationVector = glm::vec3(child->transform[3]);
        float boneLength = glm::length(childJointTranslationVector);

        // translate z by 0.5 to align bones
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.5f));

        // scale so bone will be smaller
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, boneLength));

        // get local axes by finding normals of zVectors, etc.
        glm::vec3 zVector = glm::normalize(childJointTranslationVector);
        glm::vec3 yVector = glm::normalize(glm::cross(zVector, glm::vec3(0, 0, 1)));
        glm::vec3 xVector = glm::normalize(glm::cross(yVector, zVector));

        // make rotation matrix using the vectors
        glm::mat3 rotation(xVector, yVector, zVector);
        glm::mat4 rotationMatrix = glm::mat4(rotation);

        // compute transformation matrix
        glm::mat4 transformationMatrix = rotationMatrix * scaleMatrix * translationMatrix;

        matrixStack.push(glm::transpose(transformationMatrix));

        boneMatList.push_back(matrixStack.top());

        matrixStack.pop();

        computeBoneTransforms(child, matrixStack);

    }

}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// TODO: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{
    glm::mat4 jointTransform = m_joints[jointIndex]->transform;

    // format of rotate: glm::mat4 glm::rotate(glm::mat4 const& m, float angle, glm::vec3 const& axis);
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1, 0, 0));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(angleZ), glm::vec3(0, 0, 1));

    glm::mat4 transformationMatrix = jointTransform * rotateZ * rotateY * rotateX;

    m_joints[jointIndex]->transform = transformationMatrix;
}







