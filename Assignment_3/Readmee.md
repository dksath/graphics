# OpenGL Texture Mapping
- Left mouse drag will rotate the model around a mapped axis based on the mouse motion
- Shift + left mouse drag will translate the model in the screen space based on the mouse motion
- Scrolling the mouse will scale the model to make it either smaller or bigger depending on the mouse scrolling direction.
- Press key ‘p’ to show 3D model with planer texture mapping
- Press key ‘c’ to show 3D model with cylindrical texture mapping
- Press key ‘s’ to show 3D model with spherical texture mapping
- load .obj file as './data/example.obj'

## Functions

Based on the mathematical formula from the slides, we converted the 3d coordinates back to 2d to store in *objModel.vertices[i].t[2]* . 


Planar Parameterization
![Screenshot](./data/Screenshot%202022-07-04-163730.png)
```cpp
//// TODO: fill this function to realize plane mapping
void calcPlaneMapping(void)
{
  // loop over all vertices and update myObject.vertices[i].t
    for (int i = 0; i < myObject.vertices.size(); i++) {
        float x = myObject.vertices.at(i).v[0];
        float y = myObject.vertices.at(i).v[1];

        myObject.vertices.at(i).t[0] = x + 0.5;
        myObject.vertices.at(i).t[1] = y + 0.5;
    }

}
```
Cylindrical Parameterization
![Screenshot](./data/Screenshot%202022-07-04-163756.png)
```cpp
//// TODO: fill this function to realize cylindrical mapping
void calcCylindricalMapping(void)
{
    for (int i = 0; i < myObject.vertices.size(); i++) {
        float x = myObject.vertices.at(i).v[0];
        float y = myObject.vertices.at(i).v[1];
        float z = myObject.vertices.at(i).v[2];
        float angle = atan2f(z, x);

        myObject.vertices.at(i).t[0] = (angle + PI) / (2 * PI);;
        myObject.vertices.at(i).t[1] = y + 0.5;
    }
}
```
Spherical Parameterization
![Screenshot](./data/Screenshot%202022-07-04-163814.png)
```cpp

//// TODO: fill this function to realize sphere mapping
void calcSphereMapping(void)
{
    for (int i = 0; i < myObject.vertices.size(); i++) {
        float x = myObject.vertices.at(i).v[0];
        float y = myObject.vertices.at(i).v[1];
        float z = myObject.vertices.at(i).v[2];

        //angles
        float p = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
        float theta = atan2f(z, x);
        float phi = acosf(y / p);

        myObject.vertices.at(i).t[0] = (theta + PI) / (2 * PI);
        myObject.vertices.at(i).t[1] = phi / PI;
    }
}
```



The screenshots on the .obj models are seen as below

Planar Parameterization
![Screenshot](./data/Screenshot%202022-07-04-164827.png) 

Cylindrical Parameterization
![Screenshot](./data/Screenshot%202022-07-04-164844.png)

Spherical Parameterization
![Screenshot](./data/Screenshot%202022-07-04-164859.png)

