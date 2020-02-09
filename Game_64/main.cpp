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
#include "object.h"


#define PURPLE 1.0f, 0.0f, 1.0f
#define RED 1.0f, 0.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f

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
//mouse movement
static const float mouseSensitivity = .05f;
static float yaw = -90.0f;
static float pitch = 0.0f;
static bool firstMouse = true;
static float lastX;
static float lastY;
static double mousex;
static double mousey;
//jumping
static bool inJumpSequence = false;
static float jumpMinX = -3.0f; //starting x in parabola y = -x^2 + b
static float jumpCurrentX = jumpMinX;
static float jumpSpeedMag = 10.0f; //speed of iteration through "x" of function
static float jumpMag = 8.0f; //magnitude of peak height
static bool currentlyJumping = false;
static float floorY = 0.0;
static bool isStanding = true;


void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods){
  //ESC
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  //WASD Movement
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
  //Jumping
  if(key == GLFW_KEY_SPACE){
    if(action == GLFW_PRESS){
      currentlyJumping = true;
    }
    if(action == GLFW_RELEASE) currentlyJumping = false;
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

//adding the derivative of f(x) = -x^2 + b every frame
float jumpHeight(float deltaTime){
  jumpCurrentX = jumpCurrentX+jumpSpeedMag*deltaTime; //iterating the "x"
  float newheight = -1.0f*jumpMag*deltaTime*(jumpCurrentX); //updating the "f(x)"
  return newheight;
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

    Object person = Object();
    person.updatePerson(eyex, eyey, eyez);

    vector<Cube> objectlist;
    //Making a Cube !
    vector<float> colorP{PURPLE};
    vector<float> colorR{RED};
    vector<float> colorB{BLUE};
    vector<float> position{10.0f, -9.0f, 10.0f};
    vector<float> position2{-5.0f, -80.0f, -5.0f};
    vector<float> dimensions{10.0f, 10.0f, 10.0f};
    Cube testCube = Cube(position, dimensions, colorP, colorP, colorR, colorR, colorB ,colorB);
    Cube testCube2 = Cube(position2, dimensions, colorP, colorP, colorR, colorR, colorB ,colorB);
    const int numfloats = 108;
    GLfloat g_vertex_buffer_data[2*numfloats];
    GLfloat g_color_buffer_data[2*numfloats];
    for(int i = 0; i < numfloats; i++){
        g_vertex_buffer_data[i] = testCube.all_triangles[i];
        g_color_buffer_data[i] = testCube.colors[i];
        g_vertex_buffer_data[i+108] = testCube2.all_triangles[i];
        g_color_buffer_data[i+108] = testCube2.colors[i];
    }
    objectlist.push_back(testCube);
    objectlist.push_back(testCube2);


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
    /*
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
    */
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

    //float peakHeight = -1;

    float fallCurrentX = 0.0;
    float fallSpeedMag = jumpSpeedMag;
    float fallMag = jumpMag;

    bool isFalling = false;

    //GAME LOOP
    while (!glfwWindowShouldClose(window)){
        //if(isFalling) cout << "falling: " << isFalling << endl;
        //if(inJumpSequence) cout << "jumping: " << inJumpSequence << endl;
        //normalizing movement to elapsed time
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cameraSpeed = cameraSpeedBase * deltaTime;
        //mouse movement
        glfwGetCursorPos(window, &mousex, &mousey);
        mouse_callback(window, mousex, mousey); //modifies dirvec with mouse

        //jumping
        if(currentlyJumping && inJumpSequence == false){
          inJumpSequence = true;

        }


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

        person.updatePerson(eye.x, eye.y, eye.z);



        //if currently jumping
        if(inJumpSequence){
          eye.y+=jumpHeight(deltaTime);
          //TODO: TEMPORARY CONDITION FOR ENDING JUMP
          if(eye.y < floorY+5.7){
            eye.y = floorY+5.7;
            jumpCurrentX = jumpMinX;
            inJumpSequence = false;
          }
        }
        //Collision checking
        for(long long unsigned int i = 0; i < objectlist.size(); i++){
          Cube obj = objectlist[i];
          //if the player collides with an object, adjust x, y, z individually and re-check the intersection until you find the correct overlap
          if(isCollision(obj, person)){
            //cout << "collision detected" << endl;
            float deltax = eyex - eye.x;
            float deltay = eyey - eye.y;
            float deltaz = eyez - eye.z;
            //check y intersection
            //cout << eye.y << endl;
            person.updatePerson(eye.x, eyey+deltay, eye.z);
            eye.y = eyey+deltay;
            //check x intersection
            if(isCollision(obj, person)){
              person.updatePerson(eye.x, eyey-deltay, eye.z);
              eye.y = eyey-deltay;
              person.updatePerson(eyex+deltax, eye.y, eye.z);
              eye.x = eyex+deltax;
              //check z intersection
              if(isCollision(obj, person)){
                person.updatePerson(eyex-deltax, eye.y, eye.z);
                eye.x = eyex-deltax;
                person.updatePerson(eye.x, eye.y, eyez+deltaz);
                eye.z = eyez+deltaz;
              }
            }
            //if y collision, set is standing
            else{
              floorY = obj.max_Y+.01;
              person.updatePerson(eye.x, floorY+5.7, eye.z);
              eye.y = floorY+5.7;
              inJumpSequence = false;
            }
          }
        }

        //update the old eye variables to this frame
        eyex = eye.x;
        eyey = eye.y;
        eyez = eye.z;

        //CHECK FOR STANDING
        isFalling = true;
        if(inJumpSequence) isFalling = false;
        person.updatePerson(eye.x, eye.y - .01, eye.z);
        eye.y = eye.y - .01;
        for(long long unsigned int i = 0; i < objectlist.size(); i++){
          Cube obj = objectlist[i];
          if(isCollision(obj, person)){
            isFalling = false;
          }
        }
        person.updatePerson(eye.x, eye.y + .01, eye.z);
        eye.y = eye.y + .01;

        if(isFalling){
          fallCurrentX = fallCurrentX+fallSpeedMag*deltaTime; //iterating the "x"
          eye.y = eye.y + -1.0f*fallMag*deltaTime*(fallCurrentX); //updating the "f(x)"
          person.updatePerson(eye.x, eye.y, eye.z);
        }
        else{
          fallCurrentX = 0.0f;
        }
        if(eye.y < 5.7 && glfwGetTime()<3){
          eye.y = 5.7;
          person.updatePerson(eye.x, eye.y, eye.z);
        }
        //cout << eye.y << endl;
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
