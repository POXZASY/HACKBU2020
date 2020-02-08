//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
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

void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_G && action == GLFW_PRESS){
    glClearColor(0.0, 1.0, 0.0, 0.0);
  }
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
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

    //initialize glew
    glewExperimental = GL_TRUE;
    glewInit();

    //making Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //verticies of a triangle
    static const GLfloat g_vertex_buffer_data[] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,
    };



    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    //CAMERA
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_width/(float)window_height, 0.1f, 100.0f); //projection matrix
    float eyex = 0.0;
    float eyey = 5.7;
    float eyez = 0.0;
    glm::vec3 eye = glm::vec3(eyex, eyey, eyez);
    glm::vec3 target = glm::vec3(eyex+20, eyey, eyez);
    glm::vec3 upwards = glm::vec3(0.0, 1.0, 0.0);
    glm::mat4 view = glm::lookAt(eye, target, upwards); //view matrix
    //glm::mat4 view = glm::lookAt(glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 model = glm::mat4(1.0); //model matrix as identity
    glm::mat4 mvp = projection*view*model; //Model View Projection matrix
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    //making the buffer
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    //GAME LOOP
    while (!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Use shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
		    // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
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
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        drawWorld();
        glfwSwapBuffers(window);
        glfwPollEvents();
        //glFlush();
    }
    // Cleanup VBO and shader
  	glDeleteBuffers(1, &vertexbuffer);
  	glDeleteProgram(programID);
  	glDeleteVertexArrays(1, &VertexArrayID);

    glfwTerminate();
    return 0;
}
