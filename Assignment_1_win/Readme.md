# OpenGL Mesh Viewer


## Mesh Loader
verList was supposed to be filled up in the order 

<center><big><pre class="tab">v1.x, v1.y, v1.z, vn1.x, vn1.y, vn1.z<tab></big></center>

, with the normal having to correspond to the index given by the indices of the faces

eg.

<center><big><pre class="tab">1/1/4     2/2/3       21/22/2<tab></big></center>
which corresponds to 
<center><big><pre class="tab">a/b/c     d/e/f       g/h/i <tab></big></center>

,meaning vertex a has a normal at the *cth* normal, vertex d at *fth* normal, vertex g at *ith* normal. *(1st *"v"* row of x,y,z values in obj has a normal at 4th *"vn"* row with x,y,z values in obj)*


Firstly, when reading line by line in the .obj file, I filled the coordinates  for "v" and "vn" seperate arrays, *vertexList & normList*

```cpp
if (line.substr(0, 2) == "v " ) {

                istringstream ss(line.substr(2));
                // Transfer vertex point info 'v' to 'vertexList'
                
                ss >> x >> y >> z;

                
                vertexList.push_back(x); vertexList.push_back(y); vertexList.push_back(z); // vertex x,y,z
            
            }
            else if (line.substr(0, 3) == "vn ") {

                istringstream ss(line.substr(3));
                // Transfer vertex normal info 'vn' to 'normList'
                ss >> xn >> yn >> zn;

                
               normList.push_back(xn); normList.push_back(yn); normList.push_back(zn); // vertex normal xn,yn,zn
               
            }

```
and vertex & its normal indices into *triList* and *trinormInexList* respectively. (a, d, g & c, f, i)
```cpp
        else if (line.substr(0, 2) == "f ") {

                istringstream ss(line.substr(2));
                // Transfer face info 'f' to 'triList'
                int a, b, c, d, e, f, g, h, i;
                char slash;

                // loading from obj file into appropariate variables
                
                ss >> a >> slash >> b >> slash >> c;
                ss >> d >> slash >> e >> slash >> f;
                ss >> g >> slash >> h >> slash >> i;

                triList.push_back(a-1); triList.push_back(d-1); triList.push_back(g-1); // vertex indices
                tri_normIndexList.push_back(c-1); tri_normIndexList.push_back(f - 1); tri_normIndexList.push_back(i - 1); // normal indices

        }
```

There is a minus one because the vertex index starts at 1 instead of 0.

Then, we reorder each normal to its corresponding vertex. This is achieved by creating another vector array *norm_reorder* and reassigning them in a for loop.
```cpp
norm_reorder.resize(vertexList.size());
         

for (unsigned int idx = 0; idx < triList.size(); idx+=3)
{
    // vertex indices
    int a, d, g;
    a = triList[idx]; d = triList[idx + 1]; g = triList[idx + 2];
            

    // vertex normal indices
    int c, f, i;
    c = tri_normIndexList[idx]; f = tri_normIndexList[idx + 1]; i = tri_normIndexList[idx + 2];

    // assign/reorder vertex normal values to its indices
    norm_reorder[a*3] = normList[c*3]; norm_reorder[a*3 + 1] = normList[c*3+1]; norm_reorder[a*3 + 2] = normList[c*3+2];
    norm_reorder[d*3] = normList[f*3]; norm_reorder[d*3 + 1] = normList[f*3+1]; norm_reorder[d*3 + 2] = normList[f*3+2];
    norm_reorder[g*3] = normList[i*3]; norm_reorder[g*3 + 1] = normList[i*3+1]; norm_reorder[g*3 + 2] = normList[i*3+2];
            
    }
```

Times 3 is to account for the 3d vector shape.

Lastly, fill up verList with the order as shown all the way above.

```cpp
for (unsigned int i = 0; i < vertexList.size(); i+=3)
        {
            
            verList.push_back(vertexList[i]); verList.push_back(vertexList[i + 1]); verList.push_back(vertexList[i + 2]); 
            verList.push_back(norm_reorder[i]); verList.push_back(norm_reorder[i + 1]); verList.push_back(norm_reorder[i + 2]);
            
        }
```


## Mesh Coloring
Color is toggled upon press the 'c' button. Four colors are given in the *colorTable* and the *colorID* will correspond and change upon 'c' press to rgb given in the *colorTable*
```cpp
void SetMeshColor(int &colorID)
{
        colorID = (colorID + 1) % 4; // toggling color index upon 'c' press
        cout << "Color has been toggled to " << colorID << "." << endl;
}
```
Screenshot of color change

![Screenshot](./Assignment_1_win/data/Screenshot%202022-06-03-173616.png) 



## Mesh Scaling
Scaling is possible using the glm library where the increase/decrease in size is accounted in each x,y,z axis. The mouse scrolling scales it up or down depending on the scrolling movement.

```cpp
void ScaleModel(float scale)
{ 
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
}
```

Screenshot of scaling

![Screenshot](./Assignment_1_win/data/Screenshot%202022-06-03-173742.png) 
![Screenshot](./Assignment_1_win/data/Screenshot%202022-06-03-173836.png)



## Mesh Rotation
Rotation is possible using the mouse thus the angle created from mouse and axis are both called.
```cpp
void RotateModel(float angle, glm::vec3 axis)
{
    modelMatrix = glm::rotate(modelMatrix, angle, axis);
}
```
Screenshot of rotation

![Screenshot](./Assignment_1_win/data/Screenshot%202022-06-03-173900.png) 

## Mesh Translation
Using the same glm library, translation vector is collected and upon clicking the left-shift button translation is possible.
```cpp
void TranslateModel(glm::vec3 transVec)
{
    modelMatrix = glm::translate(modelMatrix, transVec);
}
```
Screenshot of translation
![Screenshot](./Assignment_1_win/data/Screenshot%202022-06-03-173926.png)
