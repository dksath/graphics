////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 1 of SUTD Course 50.017 (May-Aug Term, 2022)
//
//    Mesh Viewer
//
//  2022-May-26
//
//
////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderSource.h"
#include "shader.h"


using namespace std;


#define MAX_BUFFER_SIZE            1024

#define _ROTATE_FACTOR              0.005f
#define _SCALE_FACTOR               0.01f
#define _TRANS_FACTOR               0.02f

#define _Z_NEAR                     0.001f
#define _Z_FAR                      100.0f



/***********************************************************************/
/**************************   global variables   ***********************/
/***********************************************************************/


// Window size
unsigned int winWidth  = 800;
unsigned int winHeight = 600;

// Camera
glm::vec3 camera_position = glm::vec3 (0.0f, 0.0f, 15.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
float camera_fovy = 45.0f;    
glm::mat4 projection;

// Mouse interaction 
bool leftMouseButtonHold = false;
bool isFirstMouse = true;
float prevMouseX;
float prevMouseY;
 glm::mat4 modelMatrix = glm::mat4(1.0f);

 // Mesh color table
glm::vec3 colorTable[4] = 
 {
    glm::vec3(0.6, 1.0, 0.6),
    glm::vec3(1.0, 0.6, 0.6),
    glm::vec3(0.6, 0.6, 1.0),
    glm::vec3(1.0, 1.0, 0.6) 
};

// Mesh rendering color
int colorID = 0;
glm::vec3  meshColor;


// declaration
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);





/******************************************************************************/
/***************   Functions to be filled in for Assignment 1    **************/
/***************    IMPORTANT: you ONLY need to work on these    **************/
/***************                functions in this section        **************/
/******************************************************************************/


// TODO: insert your code in this function for Mesh Loading
//       1) store vertices and normals in verList with order (v.x, v.y, v.z, n.x, n.y, n.z)
//       2) store vertex indices of each triangle in triList 
int LoadInput(vector<float> &verList, vector<unsigned> &triList)
{
    // filename from standard input
    getFilename:
        cout << "Enter the filename of the object you wish to render. File must be in the same directory.\n";
        char filename[1024];
        cin.getline(filename, 1024);
        ifstream in(filename, ios::in);
        

        // error config
        if (!in)
        {
            // Print an error and prompt for another file
            cerr << "Unable to read file input. Please try again or input another file." << endl;
            goto getFilename;
        }

  
        cout << "OBJ data found. Proceeding to render..." << endl;

        string line;
        int validCounter = 0;
        vector<float> vertexList;
        vector<float> normList;
        vector<float> norm_reorder;
        vector<float> ver_reorder;
        vector<unsigned> tri_normIndexList;

        /*getline(inf, buffer);
        istringstream ss(buffer);
        ss >> key;*/
        
       

       
        // extract geometry info
        while (getline(in, line)) {

            // Read lines from file
            /*istringstream ss(buffer);*/

            // initialize floats
            float x, y, z, xn, yn, zn = 0;
            
            
            // Break loop when end of file is reached
            /*if (!(ss >> key))
            {
                cout << "End of file reached." << endl;
                break;
            }*/

            // Extract info if line contains 'v', 'vn' and 'f' header
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
                //triList.push_back(b); triList.push_back(e); triList.push_back(h); // ignore for this assignment
                //std::cout << a << " " << d << " " << g << " " << c << " " << f << " " << i << endl;

            }
        }

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
         
        for (unsigned int i = 0; i < vertexList.size(); i+=3)
        {
            
            verList.push_back(vertexList[i]); verList.push_back(vertexList[i + 1]); verList.push_back(vertexList[i + 2]); 
            verList.push_back(norm_reorder[i]); verList.push_back(norm_reorder[i + 1]); verList.push_back(norm_reorder[i + 2]);
            
        }

        cout << "OBJ data loaded." << endl;

        return 0;
}

// TODO: insert your code in this function for Mesh Coloring
void SetMeshColor(int &colorID)
{
        colorID = (colorID + 1) % 4; // toggling color index upon 'c' press
        cout << "Color has been toggled to " << colorID << "." << endl;
   
}

// TODO: insert your code in this function for Mesh Transformation (Rotation)
void RotateModel(float angle, glm::vec3 axis)
{
    modelMatrix = glm::rotate(modelMatrix, angle, axis);
    
}

// TODO: insert your code in this function for Mesh Transformation (Translation)
void TranslateModel(glm::vec3 transVec)
{
    modelMatrix = glm::translate(modelMatrix, transVec);
}

// TODO: insert your code in this function for Mesh Transformation (Scaling)
void ScaleModel(float scale)
{ 
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
}



/******************************************************************************/
/***************                  Callback Function              **************/
/******************************************************************************/

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);

    winWidth  = width;
    winHeight = height;
}


// glfw: whenever a key is pressed, this callback is called
// ----------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        SetMeshColor( colorID );
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        leftMouseButtonHold = true;
    }
    else
    {
    	leftMouseButtonHold = false;
    }
}


// glfw: whenever the cursor moves, this callback is calledTranslateModel
// -------------------------------------------------------
void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY)
{
	float  dx, dy;
	float  nx, ny, scale, angle;
    

	if ( leftMouseButtonHold )
	{
		if (isFirstMouse)
	    {
	        prevMouseX = mouseX;
	        prevMouseY = mouseY;
	        isFirstMouse = false;
	    }

	    else
	    {
            if( glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
            {
                float dx =         _TRANS_FACTOR * (mouseX - prevMouseX);
                float dy = -1.0f * _TRANS_FACTOR * (mouseY - prevMouseY); // reversed since y-coordinates go from bottom to top

                prevMouseX = mouseX;
                prevMouseY = mouseY;

                TranslateModel( glm::vec3(dx, dy, 0) );  
            }

            else
            {
                float dx =   mouseX - prevMouseX;
                float dy = -(mouseY - prevMouseY); // reversed since y-coordinates go from bottom to top

                prevMouseX = mouseX;
                prevMouseY = mouseY;

               // Rotation
                nx    = -dy;
                ny    =  dx;
                scale = sqrt(nx*nx + ny*ny);

                // We use "ArcBall Rotation" to compute the rotation axis and angle based on the mouse motion
                nx    = nx / scale;
                ny    = ny / scale;
                angle = scale * _ROTATE_FACTOR;

                RotateModel( angle, glm::vec3(nx, ny, 0.0f) );
            }
	    }  
	}  

	else
	{
		isFirstMouse = true;
	}
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	float scale = 1.0f + _SCALE_FACTOR * yOffset;

	ScaleModel( scale ); 
}




/******************************************************************************/
/***************                    Main Function                **************/
/******************************************************************************/

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Assignment 1 - Mesh Viewer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    shader myShader;
    myShader.setUpShader(vertexShaderSource,fragmentShaderSource);

    // Load input mesh data
    vector<float> verList;          // This is the list of vertices and normals for rendering
    vector<unsigned> triList;       // This is the list of faces for rendering
    LoadInput(verList, triList);

    // create buffers/arrays
    unsigned int VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verList.size() * sizeof(float), &verList[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triList.size() * sizeof(unsigned int), &triList[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3* sizeof(float))));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    myShader.use();


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera_fovy), (float)winWidth / (float)winHeight, _Z_NEAR, _Z_FAR);
        glm::mat4 view = glm::lookAt(camera_position, camera_target, camera_up);

        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        // render the loaded model
        //glm::vec3 aColor = glm::vec3 (0.6f, 1.0f, 0.6f);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "meshColor"), 1, &colorTable[colorID][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "viewPos"), 1, &camera_position[0]);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, triList.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(myShader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

