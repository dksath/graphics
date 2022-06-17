# OpenGL Hierarchical Skeleton
- Load skeleton model using path './data/Model(#num).skel'
- Left mouse drag will rotate the model around a mapped axis based on the mouse motion.
- Shift + left mouse drag will translate the model in the screen space based on the mouse
motion.
- Scrolling the mouse will scale the model to make it either smaller or bigger depending
on the mouse scrolling direction
- Pressing key ‘c’ will prompt users to input joint ID and corresponding Euler angles. Pressing ‘enter’ key will execute the function to change the skeleton’s pose.


## Load Skeleton

Similar to Assignment 1, each line of the .skel file is saved as such with the index being the parent joint.
<center><big><pre class="tab">v1 >> v2 >> v3 >> index<tab></big></center>

I initialized a new joint by calling
```cpp
Joint *joint = new Joint;
```
I then must translate the joint based on the saved vector of joint’s translation relative to its parent joint above. Then I must push the joint into the m_joints list. I must also set the root joint and assign the parent pointer to its children.
```cpp
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

```

## Draw Skeleton
I must be able to draw the joints via a stack-based traversal function. In this function I must incorporate a recursion and also explore all joints, thus I created the condition and for loop. Also, a key point to note is that I must stack the transpose because of the matrix multiplication because the push function multiplies on the back of the matrix.
```cpp
// in MatrixStack.cpp
glm::mat4 top = m * m_matrices.back();
```
Then I pop the transformation matrix at the end of the recursion.
```cpp
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
```
Drawing the bones is more challenging. I must do a similar recursion like in the joint function but also need to add three additional transformation matrices. I need to translate, scale and rotate the cylinder by adding a translation,scaling and rotation matrix in that order before adding the top of the stack. I followed the instructions below as listed in the handout.

```
Translate it in z such that
the cylinder ranges from [-0.5, -0.5, 0] to [0.5, 0.5, 1]. 

Scale the cylinder so that it ranges from
[-0.01, -0.01, 0] to [0.01, 0.01, d], where d is the distance to the next joint in your recursion.

Finally, you need to rotate the z-axis so that it is aligned with the direction to the parent joint:

z = parentOffset.normalized(). Since the x and y axes are arbitrary, we recommend mapping

y = (z × r).normalized(), and x = (y × z).normalized(), with r supplied as [0, 0, 1] . <tab></center>
```
I must also got the distance from the current joint to the next joint by extracting the last 3d vector from the transformation matrix. 

```cpp
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
```



## Change Pose of Skeleton
To change the pose, I need to need to set the rotation component of the joint’s
transformation matrix. I created the matrices using the helper function in which I used the basis vectors of the three axis as the axis of rotation.

```cpp
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
```

The skeleton (Model3.skel) shown below is transformed by adjusting the Euler angles of its right leg only.

![Screenshot](./data/Screenshot%202022-06-17-190647.png) 

90 degree change

![Screenshot](./data/Screenshot%202022-06-17-190939.png)


