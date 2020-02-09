//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
//https://learnopengl.com/Getting-started/Camera
#define GLFW_DLL
#define GLEW_DLL
#define GLEW_BUILD
//http://www.grhmedia.com/glew.html
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "loadShader.cpp"

using namespace std;



static float eyex = 0.0;
static float eyey = 5.7;
static float eyez = 0.0;
static glm::vec3 eye = glm::vec3(eyex, eyey, eyez); //"eye" is actually location of the player
static glm::vec3 dirvec = glm::vec3(0.0f, 0.0f, 1.0f);
static float cameraSpeed = 0;
static float cameraSpeedBase = 10;
static bool goForward = false;
static bool goBackward = false;
static bool goLeft = false;
static bool goRight = false;
static const float mouseSensitivity = .05f;
static float yaw = -90.0f;
static float pitch = 0.0f;
static bool firstMouse = true;
static float lastX;
static float lastY;
static double mousex;
static double mousey;
static bool inJumpSequence = false;


void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_G && action == GLFW_PRESS){
    glClearColor(0.0, 1.0, 0.0, 0.0);
  }
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  if(key == GLFW_KEY_W){
    if(action == GLFW_PRESS) goForward = true;
    if(action == GLFW_RELEASE) goForward = false;
  }
  if(key == GLFW_KEY_A){
    if(action == GLFW_PRESS) goLeft = true;
    if(action == GLFW_RELEASE) goLeft = false;
  }
  if(key == GLFW_KEY_S){
    if(action == GLFW_PRESS) goBackward = true;
    if(action == GLFW_RELEASE) goBackward = false;
  }
  if(key == GLFW_KEY_D){
    if(action == GLFW_PRESS) goRight = true;
    if(action == GLFW_RELEASE) goRight = false;
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
  //mouse input
  if(firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;
  lastX = xpos;
  lastY = ypos;

  yaw += xoffset;
  pitch += yoffset;
  if(pitch > 89.9f) pitch = 89.9f;
  if(pitch < -89.9f) pitch = -89.9f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  dirvec = glm::normalize(direction);
}

void drawWorld(){
  //Set up the game window
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //draw the world
  glColor3f(1.0, 1.0, 1.0);

  glBegin(GL_POLYGON);
  glVertex3f(-100.0, 0, -100.0);
  glVertex3f(-100.0, 0, 100.0);
  glVertex3f(100.0, 0, 100.0);
  glVertex3f(100.0, 0, -100.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex3f(-.5, -.5, 0);
  glVertex3f(-.5, .5, 0);
  glVertex3f(.5, .5, 0);
  glVertex3f(.5, -.5, 0);
  glEnd();
}
int main(){
    int window_width;
    int window_height;

    GLFWwindow* window;

    //Initialize the library
    if (!glfwInit()){
      cout << "GLFW failed to initialize." << endl;
      return -1;
    }
    //Create a windowed mode window and its OpenGL context
    // creates full screen window
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window_width = mode->width;
    window_height = mode->height;

    window = glfwCreateWindow(window_width, window_height, "Physics Engine", monitor, NULL);
    if (!window){
        cout << "Window failed to initialize." << endl;
        glfwTerminate();
        return -1;
    }
    //Make the window's context current
    glfwMakeContextCurrent(window);
    //set the function that responds to keyboard input
    glfwSetKeyCallback(window, keyboardFunc);

    //Mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //if (glfwRawMouseMotionSupported())  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetCursorPosCallback(window, mouse_callback);

    //initialize glew
    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //making Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //verticies of a triangle
    static const GLfloat g_vertex_buffer_data[] = {
      -100.0f, 0.0f, -100.0f, //tri 1
      -100.0f, 0.0f, 100.0f,
      0.0f, 0.0f, 100.0f,
      -100.0f, 0.0f, -100.0f, //tri 2
      0.0f, 0.0f, 100.0f,
      0.0f, 0.0f, -100.0f,
      0.0f, 0.0f, -100.0f, //tri 3
      0.0f, 0.0f, 100.0f,
      100.0f, 0.0f, -100.0f,
      0.0f, 0.0f, 100.0f, //tri 4
      100.0f, 0.0f, 100.0f,
      100.0f, 0.0f, -100.0f,
    };

    //BEGIN COLOR
    //colors of the vertices
    static const GLfloat g_color_buffer_data[] = {
      1.0f, 0.0f, 0.0f, //tri 1
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, //tri 2
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f, //tri 3
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 0.0f, //tri 4
      1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
    };

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    //END COLOR

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    //CAMERA
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)window_width/(float)window_height, 0.1f, 100.0f); //projection matrix
    glm::vec3 upwards = glm::vec3(0.0, 1.0, 0.0);
    glm::mat4 model = glm::mat4(1.0); //model matrix as identity




    //making the buffer
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



    //Time / movement management
    float lastFrame = 0;

    //GAME LOOP
    while (!glfwWindowShouldClose(window)){
        //normalizing movement to elapsed time
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cameraSpeed = cameraSpeedBase * deltaTime;
        //mouse movement
        glfwGetCursorPos(window, &mousex, &mousey);
        mouse_callback(window, mousex, mousey); //modifies dirvec with mouse

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Update camera
        glm::mat4 view = glm::lookAt(eye, eye+dirvec, upwards); //view matrix
        glm::mat4 mvp = projection*view*model; //Model View Projection matrix
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        //Use shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
		    // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        int numtriangles = sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0])/9;
        //cout << "Number of triangles: "<<numtriangles<<endl;
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
           0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           (void*)0            // array buffer offset
        );
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, numtriangles*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        //Player movement
        float eyey = eye.y;
        dirvec.y = 0.0;
        dirvec = glm::normalize(dirvec);
        if(goForward) eye+=dirvec*cameraSpeed;
        if(goBackward) eye-=dirvec*cameraSpeed;
        glm::vec3 rightVec = glm::normalize(glm::cross(dirvec, upwards));
        if(goLeft) eye-=rightVec*cameraSpeed;
        if(goRight) eye+=rightVec*cameraSpeed;
        eye.y = eyey; //don't fly

        //drawWorld();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup VBO and shader
  	glDeleteBuffers(1, &vertexbuffer);
  	glDeleteProgram(programID);
  	glDeleteVertexArrays(1, &VertexArrayID);

    glfwTerminate();
    return 0;
}
