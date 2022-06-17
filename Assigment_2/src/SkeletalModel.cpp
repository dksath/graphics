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
void SkeletalModel::loadSkeleton(const char *filename)
{
    ifstream infile(filename);
    float v1, v2, v3;
    int index;
    while (infile >> v1 >> v2 >> v3 >> index)
    {
        Joint *joint = new Joint; // assign pointer to Joint class
        joint->transform = glm::translate(glm::mat4(1.0f), glm::vec3(v1, v2, v3)); // assigning translation in Joint class

        m_joints.push_back(joint);

        if (index == -1)
        {
            m_rootJoint = joint; // assign root pointer to current pointer
        }
        else
        {
            m_joints.at(index)->children.push_back(joint); // assign parent pointer to child and append current joint to joints
        }

    }
}

///=========================================================================================///
///                         Compute transformations for Joints and Bones
///=========================================================================================///

void SkeletalModel::computeTransforms()
{
    if (m_joints.size() == 0)
        return;

    computeJointTransforms();

    computeBoneTransforms();
}

// Compute a transformation matrix for each joint (i.e., ball) of the skeleton
void SkeletalModel::computeJointTransforms()
{
    jointMatList.clear();

    m_matrixStack.clear();

    computeJointTransforms(m_rootJoint, m_matrixStack);
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint *joint, MatrixStack matrix_stack)
{
    matrix_stack.push(glm::transpose(joint->transform));
    jointMatList.push_back(matrix_stack.top()); //save matrix

    if (joint->children.size() != 0)
    {   // loop over children (joints)
        for (Joint* child : joint->children)
        {
            computeJointTransforms(child, matrix_stack);
        }
    }

    matrix_stack.pop();
}

// Compute a transformation matrix for each bone (i.e., cylinder) between each pair of joints in the skeleton
void SkeletalModel::computeBoneTransforms()
{
    boneMatList.clear();

    m_matrixStack.clear();

    computeBoneTransforms(m_rootJoint, m_matrixStack);
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the bones
void SkeletalModel::computeBoneTransforms(Joint *joint, MatrixStack matrixStack)
{
    glm::mat4 T, R, S;
    glm::vec3 vect,x_axis,y_axis, z_axis, normal;

    float len;
    matrixStack.push(glm::transpose(joint->transform));

    // checking number of available joints
    if (joint->children.size() != 0)
    {
        // loop over children (joints)
        for (Joint* child : joint->children)
        {
            vect = child->transform[3];        // getting vector from current joint to the next
            len = glm::length(vect);           // distance from current joint to next

            // computing rotation axis
            z_axis = glm::normalize(vect);
            glm::vec3 r = {0.0, 0.0, 1.0};
            y_axis = glm::normalize(glm::cross(r, z_axis)); 
            x_axis = glm::normalize(glm::cross(y_axis, z_axis));

           
            R = glm::mat4(glm::mat3(x_axis, y_axis, z_axis)); // rotation matrix
            
           //computing translation matrix                                                                               // computing transformation matrices
            glm::mat4 identityMatrix = glm::mat4(1.0f); // identity matrix

            glm::vec3 t = {0.0, 0.0, 0.5};
            T = glm::translate(identityMatrix, t); // translation matrix

            //computing scaling matrix
            glm::vec3 s = {0.01f, 0.01f, len};
            S = glm::scale(identityMatrix, s); // scale matrix

            //push matrix stack
            boneMatList.push_back(glm::transpose(R * S * T)*matrixStack.top()); // save matrix
           

            // recursive call to function
            computeBoneTransforms(child, matrixStack);
        }
    }
    matrixStack.pop();
}

///=========================================================================================///
///                              Set Joint Angles for Transform
///=========================================================================================///

// TODO: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int joint_index, float angle_x, float angle_y, float angle_z)
{
    glm::mat4 mx, my, mz;
    glm::mat4 identity_matrix = glm::mat4(1.0f);

    mx = glm::rotate(identity_matrix, angle_x, glm::vec3(1, 0, 0));
    my = glm::rotate(identity_matrix, angle_y, glm::vec3(0, 1, 0));
    mz = glm::rotate(identity_matrix, angle_z, glm::vec3(0, 0, 1));

    Joint *joint = m_joints.at(joint_index);
    joint->transform = joint->transform * mx * my * mz;
}
