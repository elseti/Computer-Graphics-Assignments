////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 1 of SUTD Course 50.017
//
//    Mesh Viewer
//
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
    // STEP 0 - load OBJ file
    // load file
    string fileContents;
   
    ifstream file("../data/sphere.obj");
    if(!file.is_open()){
        cout << "Error opening OBJ file" << endl;
        return 0;
    }

    // append line with \n
    string tempLine;
    while(getline(file, tempLine)){
        // cout << tempLine;
        // check if format is roughly correct
        if(tempLine[0] == 'v' || tempLine[0] == 'f'){
            // cout << tempLine << endl;
            fileContents += tempLine;
            fileContents += "\n";
        }
    }
    
    // temp - to store normals (vn)
    vector<float> normalList;

    // temp - to store complete face list
    vector<float> completeFaceList;

    // temp - to store JUST vertices list
    vector<float> justVertexList;

    // turn fileContents into stream fileStream
    istringstream fileStream(fileContents); 
    string line;

    // STEP 1 - get complete face list and normal list first
    while(getline(fileStream, line, '\n')){
        // cout << line << endl;

        // get the substrings (each element inside the line) first
        istringstream lineStream(line);
        vector<string> substrings;
        string substring;
        while(getline(lineStream, substring, ' ')){
            substrings.push_back(substring);
        }

        // store just vertices in justVertexList
        if(substrings[0] == "v"){
            justVertexList.push_back(stof(substrings[1])); // x
            justVertexList.push_back(stof(substrings[2])); // y
            justVertexList.push_back(stof(substrings[3])); // z
        }

        // store normals in normalList
        else if(substrings[0] == "vn"){
            normalList.push_back(stof(substrings[1])); // x
            normalList.push_back(stof(substrings[2])); // y
            normalList.push_back(stof(substrings[3])); // z
        }

        // store face list (triList) and complete face list (completeFaceList)
        // store faces (first index of each substring) into triList
        else if(substrings[0] == "f"){
            
            // split substrings into sth like "f 123/122/121 123/122/121 123/122/121" (excluding 'f' cuz i starts from 1)
            for(int i=1; i < substrings.size(); i++){
                istringstream substringsStream(substrings[i]);
                string element;
                vector<float> elementList;

                // contains the "123/122/121"
                while(getline(substringsStream, element, '/')){
                    // elementList now contains individual 123, 122, 121
                    try {
                        // cout << element << endl;
                        elementList.push_back(stof(element));
                    } 
                    catch (const std::invalid_argument& e) {
                        cerr << "Invalid argument: " << element << endl;
                        return 0;
                    }
                }

                // cout << endl << "Element list: ";
                // for (const auto& element : elementList) {
                //     cout << element << " ";
                // }

                // put only the first element into triList. (converts char to float, -1 because OpenGL starts from index 0)
                // cout << elementList[0] << endl;
                triList.push_back(elementList[0] - 1);
                
                // put first and third/last element inside completeFaceList (vertex index and normal index)
                completeFaceList.push_back(elementList[0] - 1); // vertex index
                completeFaceList.push_back(elementList[2] - 1); // normal index

            }

        }
    }

    // cout << "Complete face list: ";
    // for (const auto& element : completeFaceList) {
    //     cout << element << " ";
    // }

    // STEP 2 - create an indexList to store index of normal as a value, which corresponds to the index of the vertex as the array's index idk how to explain...
    
    // temp - to store [index_of_vertex, index_of_normal]
    vector<float> indexList;

    for(int x = 0; x < completeFaceList.size()-1; x+=2){
        // remember we only store index and normal index here (ignore texture index)
        
        int vertex = static_cast<int>(completeFaceList[x]); // vertex index

        int normal = static_cast<int>(completeFaceList[x+1]); // vertex normal

        // resize if indexList isn't large enough
        if (vertex >= static_cast<int>(indexList.size())) {
            indexList.resize(vertex + 1);
        }

        indexList[vertex] = normal;
    }

    // string filePath = "output.txt";
    // string outputFileContents;
    // ofstream outputFile(filePath);
    // if (outputFile.is_open()) {
    //     for(int x=0; x<completeFaceList.size(); x++){
    //         outputFileContents += completeFaceList[x];
    //     }
    //     outputFile << outputFileContents;
    //     outputFile.close();
    //     std::cout << "String exported to file: " << filePath << endl;
    // } else {
    //     std::cerr << "Error opening file: " << filePath << endl;
    // }

    // cout << endl << "Normal list: ";
    // for (const auto& element : normalList) {
    //     cout << element << " ";
    // }

    // cout << endl << "Index list: ";
    // for (const auto& element : indexList) {
    //     cout << element << " ";
    // }

    // cout << endl << "Just vertex list: ";
    // for (const auto& element : justVertexList) {
    //     cout << element << " ";
    // }

    // STEP 3 - combine and store justVertexList and normalList into verList
    
    // assume number of normals is the same as number of vertices
    int indexCount = 0;
    for(int x=0; x < justVertexList.size()-2; x+=3){
        // cout << endl << "x: " << x;
        // store just vertices
        verList.push_back(justVertexList[x]); // vertex x
        verList.push_back(justVertexList[x+1]); // vertex y
        verList.push_back(justVertexList[x+2]); // vertex z

        // store normals
        verList.push_back(normalList[indexList[indexCount]*3]); // normal x
        verList.push_back(normalList[indexList[indexCount]*3+1]); // normal y
        verList.push_back(normalList[indexList[indexCount]*3+2]); // normal z

        indexCount += 1;
    }

    // cout << endl << "verList: ";
    // for (const auto& element : verList) {
    //     cout << element << " ";
    // }

    // cout << endl << "triList: ";
    // for (const auto& element : triList) {
    //     cout << element << " ";
    // }

    file.close();

    // Note: these two lines of code is to avoid runtime error; 
    //       please remove them after you fill your own code for 3D model loading
    // verList.push_back(0); 
    // triList.push_back(0);
    
    return 0;
}

// TODO: insert your code in this function for Mesh Coloring
void SetMeshColor(int &colorID)
{
    // glUniform3fv(glGetUniformLocation(myShader.ID, "meshColor"), 1, &colorTable[colorID][0]);
    glm::vec3 swapColor = colorTable[colorID];

    // swap colours in colorTable whenever "C" is pressed
    int colorTableSize = sizeof(colorTable) / sizeof(colorTable[0]);

    for(int x=0; x < colorTableSize - 1; x++){
        colorTable[x] = colorTable[x+1];
    }
    colorTable[colorTableSize-1] = swapColor;
}

// TODO: insert your code in this function for Mesh Transformation (Rotation)
void RotateModel(float angle, glm::vec3 axis)
{
    
}

// TODO: insert your code in this function for Mesh Transformation (Translation)
void TranslateModel(glm::vec3 transVec)
{
    // cout << transVec[0] << transVec[1] << transVec[2] << endl;
    glm::mat4 translateMatrix = glm::mat4(1.0f);
    translateMatrix[0][3] = transVec[0];
    translateMatrix[1][3] = transVec[1];
    translateMatrix[2][3] = transVec[2];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << translateMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    modelMatrix = modelMatrix * translateMatrix;

}

// TODO: insert your code in this function for Mesh Transformation (Scaling)
void ScaleModel(float scale)
{
    /*
     * make a:
     * sx 0 0 0
     * 0 sy 0 0
     * 0 0 sz 0
     * 0 0 0 1
     */
    glm::mat4 scaleMatrix = glm::mat4(scale);
    scaleMatrix[3][3] = 1.0f;
    modelMatrix = modelMatrix * scaleMatrix;

    // for (int i = 0; i < 4; ++i) {
    //     for (int j = 0; j < 4; ++j) {
    //         std::cout << scaleMatrix[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Alternative method: (using glm::scale)
    // glm::vec3 scaleVector(scale);
    // modelMatrix = glm::scale(modelMatrix, scaleVector);
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


// glfw: whenever the cursor moves, this callback is called
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

