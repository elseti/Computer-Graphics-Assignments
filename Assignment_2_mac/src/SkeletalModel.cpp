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
        jointMatrix[3][0] = jointFieldList[0];
        jointMatrix[3][1] = jointFieldList[1];
        jointMatrix[3][2] = jointFieldList[2];

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
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint* joint, MatrixStack matrixStack)
{
   
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
  
}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// TODO: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{

}







