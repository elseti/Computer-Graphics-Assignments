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
   
    // ifstream file(filename);
    ifstream file("../data/Model1.skel"); // TODO- replace
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
        // WARNING - check if translation is at bottom or right (now is at bottom)
        glm::mat4 jointMatrix = glm::mat4(1.0f);
        // jointMatrix[3][0] = jointFieldList[0];
        // jointMatrix[3][1] = jointFieldList[1];
        // jointMatrix[3][2] = jointFieldList[2];

        jointMatrix[0][3] = jointFieldList[0];
        jointMatrix[1][3] = jointFieldList[1];
        jointMatrix[2][3] = jointFieldList[2];

        // create a new joint and assign its transform
        Joint *currJoint = new Joint;
        currJoint -> transform = jointMatrix;

        // assign to rootJoint if joint is root
        if(jointFieldList[3] == -1){
            m_rootJoint = currJoint;
            m_joints.push_back(currJoint); // not sure if m_joints include m_rootJoint
        }
        else{
            // if it's the first joint, assign the current joint as m_rootJoint's child
            if(jointFieldList[3] == 0){
                m_rootJoint -> children.push_back(currJoint);
                m_joints.push_back(currJoint);
            }
            else{
                m_joints[jointFieldList[3]] -> children.push_back(currJoint);
                // currJoint -> children.push_back(m_joints[jointFieldList[3]]);
                m_joints.push_back(currJoint);
            }
            
        }

        // cout << "jointMatrix" << endl;
        // for (int i = 0; i < 4; ++i) {
        //     for (int j = 0; j < 4; ++j) {
        //         std::cout << currJoint->transform[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        // cout << m_joints.size() << endl;
        // cout << m_rootJoint -> transform << endl;

        // for (const auto& element : m_joints) {
        //     cout << element << " ";
        // }
    }
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

    for (const auto& element : jointMatList) {
        cout << "jointMatList" << endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << element[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint* joint, MatrixStack matrixStack)
{
    // cout << "matrixstack top" << endl;
    // for (int i = 0; i < 4; ++i) {
    //     for (int j = 0; j < 4; ++j) {
    //         std::cout << matrixStack.top()[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    matrixStack.push(joint -> transform); // note: no need to do multiplication, matrix stack already does it for you.
    jointMatList.push_back(matrixStack.top());
    
    if(joint->children.size() == 0){
        matrixStack.pop();
        return;
    }
    for(int x=0; x < joint->children.size(); x++){
        computeJointTransforms(joint -> children[x], matrixStack);
    }

    
}


// Compute a transformation matrix for each bone (i.e., cylinder) between each pair of joints in the skeleton
void SkeletalModel::computeBoneTransforms( )
{
    boneMatList.clear();

    m_matrixStack.clear();

    computeBoneTransforms(m_rootJoint, m_matrixStack);

    for (const auto& element : boneMatList) {
        cout << "boneMatList" << endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << element[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the bones
void SkeletalModel::computeBoneTransforms(Joint* joint, MatrixStack matrixStack)
{
    if(joint->children.size() == 0){
        return;
    }

    for(int x=0; x<joint->children.size(); x++){
        Joint* child = joint->children[x];

        glm::vec3 currJointTranslationVector(joint->transform[0][3], joint->transform[1][3], joint->transform[2][3]);
        glm::vec3 childJointTranslationVector(child->transform[0][3], child->transform[1][3], child->transform[2][3]);

        glm::vec3 boneDirection = childJointTranslationVector - currJointTranslationVector;

        float boneLength = glm::length(boneDirection);

        glm::vec3 boneScale(0.01f, 0.01f, boneLength);

        // glm::mat4 jointTransform = parentTransform * joint->transform;
        glm::vec3 boneTranslate = glm::vec3(joint->transform[3]) * boneDirection;
        // glm::mat4 boneTranslate = glm::mat4(1.0f);
        // boneTranslate[0][3] = boneTranslateVector[0];
        // boneTranslate[1][3] = boneTranslateVector[1];
        // boneTranslate[2][3] = boneTranslateVector[2];


        // glm::vec3 boneTranslation = joint->transform + 0.5f * boneDirection;
        // glm::mat4 boneTranslate = joint->transform;
        // boneTranslate[0][3] = boneTranslate[0][3] * boneDirection;
        // boneTranslate[1][3] = boneTranslate[1][3] * boneDirection;
        // boneTranslate[2][3] = boneTranslate[2][3] * boneDirection;

        // Compute the transformation matrix for the bone between the current joint and its child
        // glm::vec3 boneDirection = child->position - joint->position;
        // float boneLength = glm::length(boneDirection);
        // glm::vec3 boneScale(1.0f, boneLength, 1.0f);
        // glm::vec3 boneTranslation = joint->position + 0.5f * boneDirection; (??)

        // glm::mat4 boneTransform = glm::translate(glm::mat4(1.0f), boneTranslation) *
        //                           glm::scale(glm::mat4(1.0f), boneScale) *
        //                           glm::toMat4(glm::quatLookAt(glm::normalize(boneDirection), glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f); // Default z-axis
        glm::vec3 rotationAxis = glm::cross(zAxis, boneDirection); // Calculate rotation axis
        
        // float rotationAngle = glm::angleBetween(zAxis, boneDirection); // Calculate rotation angle

        glm::mat4 boneTransform = glm::translate(glm::mat4(1.0f), boneTranslate) *
                                  glm::scale(glm::mat4(1.0f), boneScale);


        // Push the bone transformation matrix to the list
        matrixStack.push(boneTransform);
        boneMatList.push_back(matrixStack.top());

        // Recursively compute the bone transforms for the child joint
        computeBoneTransforms(child, matrixStack);

    }

        // for (const auto& element : joint->chi 
        //     for (int i = 0; i < 4; ++i) {
        //         for (int j = 0; j < 4; ++j) {
        //             std::cout << element->transform[i][j] << " ";
        //         }
        //         std::cout << std::endl;
        //     }
        // }
        
        // glm::vec3 translationVector(joint->transform[3][0], joint->transform[3][1], joint->transform[3][2]);
    //     glm::vec3 currJointTranslationVector(joint->transform[0][3], joint->transform[1][3], joint->transform[2][3]);
    //     glm::vec3 childJointTranslationVector(joint->children[0]->transform[0][3], joint->children[0]->transform[1][3], joint->children[0]->transform[2][3]);

            
    //     // // }
    //     // glm::vec3 boneTranslateVector = currJointTranslationVector - childJointTranslationVector;

    //     // // get length of bone
    //     // float length = glm::length(boneTranslateVector);

    //     // // Calculate the direction vector of the bone
    //     // glm::vec3 direction = boneTranslateVector / length;

    //     // // scale vector
    //     // glm::vec3 scaleVector (0.01, 0.01, length);

    //     // cout << boneTranslateVector[0] << boneTranslateVector[1] << boneTranslateVector[2] << endl;
    //     glm::mat4 boneTranslate = glm::mat4(1.0f);
    //     boneTranslate[0][3] = boneTranslateVector[0];
    //     boneTranslate[1][3] = boneTranslateVector[1];
    //     boneTranslate[2][3] = boneTranslateVector[2];

    //     // glm::mat4 boneTranslate = glm::translate(glm::mat4(1.0f), boneTranslateVector);

    //     // glm::mat4 boneScale = glm::scale(boneTranslate, scaleVector);
    //     glm::mat4 boneScale = glm::scale(glm::mat4(1.0f), scaleVector);
    //     // glm::mat4 boneResult = 

    //     matrixStack.push(boneScale); // note: no need to do multiplication, matrix stack already does it for you.
        
    //     boneMatList.push_back(matrixStack.top());
        
    //     // if(joint->children.size() == 0){
    //     //     matrixStack.pop();
    //     //     return;
    //     // }
    //     matrixStack.pop();

    //     for(int x=0; x < joint->children.size(); x++){
    //         computeBoneTransforms(joint -> children[x], matrixStack);
    //     }
    // }
    // else{
    //     return;
    // }
    

}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// TODO: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{

}







