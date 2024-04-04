
#pragma warning(disable : 4996)
#pragma comment(lib, "shell32.lib")
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "texture.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define numAstroids 500

// Active window
GLFWwindow *window;

// Properties
GLuint sWidth = 1300, sHeight = 720;

GLfloat torusScale = 130.0f;

// Camera
                      //x-comp,y-comp,z-comp
Camera camera(glm::vec3(torusScale*1.0f, .0f, 0.0f));

GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLfloat cameraZ = 0.0f;

GLfloat asteroidX = 0.0f;
GLfloat asteroidY = 0.0f;
GLfloat asteroidZ = 0.0f;

GLfloat cameraXOffset = 0.0f;
GLfloat cameraYOffset = 0.0f;
GLfloat cameraZOffset = 0.0f;

GLfloat asteroidXOffset = 0.0f;
GLfloat asteroidYOffset = 0.0f;
GLfloat asteroidZOffset = 0.0f;
GLfloat asteroidScale = 0.0f;
GLfloat asteroidDisplacement = 0.0f;
GLfloat asteroidRotation = 0.0f;
GLfloat asteroidSpeed = 0.01f;
GLfloat asteroidXRotation = 0.0f;
GLfloat asteroidYRotation = 0.0f;
GLfloat asteroidZRotation = 0.0f;

GLfloat cameraSpeed = 0.1f;

GLfloat cameraAngle = 0.0f;
GLfloat spaceShipAngle = 0.0f;
GLfloat asteroidAngle = 0.0f;

GLfloat cameraRadius = torusScale * 1.0f;
GLfloat shipRadius = torusScale * 1.0f;
GLfloat asteroidRadius = torusScale * 1.0f;

GLfloat spaceTime = 0.0f;

static void init_Resources()
{
  // Init GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Define the window
  window = glfwCreateWindow(sWidth, sHeight, "COMP3420 - Space Project", 0, 0);
  glfwMakeContextCurrent(window);

  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;

  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, sWidth, sHeight);

  // Setup OpenGL options
  glEnable(GL_DEPTH_TEST);
}

//GLfloat travelCoords(GLfloat angle, GLfloat y, GLfloat z, int choice) {
//    GLfloat ycoord, zcoord;
//    if (choice == 0) {
//        ycoord = ((y)*cos(glm::radians(angle)) - ((z)*sin(glm::radians(angle))));
//        return ycoord;
//    }
//    else {
//        zcoord = ((y)*sin(glm::radians(angle)) + ((z)*cos(glm::radians(angle))));
//        return zcoord;
//    }
//}
//static void render_SpaceAsteroid(Shader& shader, Model& model, Camera& camera)
//{
//
//  shader.Use();
//  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
//
//  // =======================================================================
//  // Create the model matrix
//  // =======================================================================
//  glm::mat4 asteroidModel = glm::mat4(1.f);
//  asteroidRadius = (rand() % (int)torusScale) + torusScale * .5f;
//  asteroidXOffset = rand() % 20;
//  asteroidYOffset = rand() % 20;
//  asteroidZOffset = rand() % 20;
//
//  asteroidModel = glm::translate(asteroidModel, glm::vec3(asteroidRadius * sin(glm::radians(asteroidAngle + 10)) + asteroidXOffset, 0.0f + asteroidYOffset, asteroidRadius * cos(glm::radians(asteroidAngle + 10)) + asteroidZOffset));
//  asteroidAngle += 0.01f;
//  // Rotate the asteroids around the axis with an angle of 1 degrees per second
//  // =======================================================================
//  // Pass the Model matrix, to the shader as "model"
//  // =======================================================================
//  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(asteroidModel));
//
//  // =======================================================================
//  // Draw the object.
//  // =======================================================================
//  model.Draw(shader);
//}


static void render_SpaceShip(Shader& shader, Model& model, Camera& camera, GLuint texture)
{
  GLuint TextureID = glGetUniformLocation(shader.Program, "shipTexture");
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  
  shader.Use();
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
  
  // =======================================================================
  // Create the model matrix
  // =======================================================================
  glm::mat4 spaceShipModel = glm::mat4(1.f);
    spaceShipModel = glm::translate(spaceShipModel, glm::vec3(shipRadius * sin(glm::radians(spaceShipAngle + 15)),0.0f, shipRadius * cos(glm::radians(spaceShipAngle + 15))));
    spaceShipAngle += 0.01f;
  // Rotate the spaceship around the axis with an angle of 1 degrees per second
  spaceShipModel = glm::rotate(spaceShipModel, glm::radians(min(spaceTime, 360.0f)), glm::vec3(1.0f, 0.f, 0.0f));
  spaceShipModel = glm::rotate(spaceShipModel, glm::radians(min(spaceTime, 270.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
  spaceShipModel = glm::rotate(spaceShipModel, glm::radians(spaceShipAngle + 15.0f - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  
  // =======================================================================
  // Pass the Model matrix, to the shader as "model"
  // =======================================================================
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceShipModel));
  
  // =======================================================================
  // Draw the object.
  // =======================================================================
  model.Draw(shader);
}

static void render_Space(Shader& shader, Model& model, Camera& camera, GLuint texture) {
  shader.Use();
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

  glm::mat4 spaceModel = glm::mat4(1.f);
  spaceModel = glm::scale(spaceModel, glm::vec3(torusScale));
  // rotate the torus around the y-axis at 1 degree per second
  spaceModel = glm::rotate(spaceModel, -glm::radians(spaceTime), glm::vec3(0.0f, 1.0f, 0.0f));

  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceModel));

  model.Draw(shader);
}

static void update_Camera() {
	cameraX = cameraRadius * sin(glm::radians(cameraAngle));
	cameraZ = cameraRadius * cos(glm::radians(cameraAngle));
  // rotate the camera around the origin
  camera = Camera(glm::vec3(cameraX + cameraXOffset, 0.0f + cameraYOffset, cameraZ + cameraZOffset), glm::vec3(0.0f,1.0f,0.0f), -cameraAngle,camera.Pitch);
	cameraAngle += 0.01f;
	if (cameraAngle > 360.0f) cameraAngle = 0.0f;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key == GLFW_KEY_W)
    cameraAngle += 1.0f;
  if (key == GLFW_KEY_S)
    cameraAngle -= 1.0f;
  if (key == GLFW_KEY_A)
    cameraRadius -= 0.1f;
  if (key == GLFW_KEY_D)
    cameraRadius += 0.1f;
  if (key == GLFW_KEY_Q)
    cameraYOffset += 0.1f;
  if (key == GLFW_KEY_E)
    cameraYOffset -= 0.1f;
  if (key == GLFW_KEY_R)
  {
    spaceTime = 0.0f;
    cameraXOffset = 0.0f;
    cameraYOffset = 0.0f;
    cameraZOffset = 0.0f;
    cameraRadius = torusScale * 1.0f;
    cameraAngle = 0.0f;
    spaceShipAngle = 0.0f;
    camera.Yaw = YAW;
    camera.Pitch = PITCH;
  }
}

// The MAIN function, from here we start our application and run the loop
int main()
{
  init_Resources();

  glfwSetKeyCallback(window, key_callback);

  Shader spaceShipShader("SpaceShipVertex.glsl", "SpaceShipFragment.glsl");

  Shader asteroidShader("AsteroidVertex.glsl", "AsteroidFragment.glsl");

  Shader spaceShader("SpaceVertex.glsl", "SpaceFragment.glsl");

  vector <Model> asteroids;

  Model spaceShip((GLchar *)"assets/objects/ship.obj");

  for (int i = 0; i < numAstroids; i++) {
		Model asteroid((GLchar*)"assets/objects/asteroid.obj");
		asteroids.push_back(asteroid);
	}

  glm::mat4 asteroidModels[numAstroids];

  srand(glfwGetTime());

  for (int i = 0; i < numAstroids; i++) {
    asteroidModels[i] = glm::mat4(1.0f);
    asteroidRadius = (rand() % (int)torusScale*.85) + torusScale * .55f;
    asteroidAngle = (rand() % 360);
    asteroidYOffset = (rand() % 30) - 15;
    asteroidModels[i] = glm::translate(asteroidModels[i], glm::vec3(asteroidRadius * sin(glm::radians(asteroidAngle + 20)), 0.0f + asteroidYOffset, asteroidRadius * cos(glm::radians(asteroidAngle + 20))));
  }

  Model torus((GLchar*)"assets/objects/torus.obj");

  GLuint shipTexture = loadBMP("assets/textures/ship.bmp");

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)sWidth / (GLfloat)sHeight, 0.1f, 1000000.0f);

  spaceShipShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(spaceShipShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  asteroidShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  spaceShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(spaceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  // Iterate this block while the window is open
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();

    // Clear buffers
    glClearColor(.8f, .8f , .8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_Space(spaceShader, torus, camera, 0);
    render_SpaceShip(spaceShipShader, spaceShip, camera, shipTexture);

    asteroidShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

    // Render the asteroids
    asteroidShader.Use();
    for (int i = 0; i < numAstroids; i++) {
      // Rotate the asteroids around a random axis
      asteroidXRotation = (rand() % 360);
      asteroidYRotation = (rand() % 360);
      asteroidZRotation = (rand() % 360);
      asteroidModels[i] = glm::rotate(asteroidModels[i], asteroidSpeed, glm::vec3(0.2f, 0.8f, 0.6f));
      glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "model"), 1,
        GL_FALSE, glm::value_ptr(asteroidModels[i]));
      asteroids[i].Draw(asteroidShader);
    }
      
    // Increment the time variable
    spaceTime += .1;
    // Update the camera to zoom out
    if (spaceTime > 360.0f && spaceTime < 390.0f)
    {
      camera = Camera(glm::vec3(cameraRadius * sin(glm::radians(cameraAngle)) + cameraXOffset, 0.0f + cameraYOffset, cameraRadius * cos(glm::radians(cameraAngle)) + cameraZOffset), glm::vec3(0.0f, 1.0f, 0.0f), -cameraAngle, camera.Pitch);
    }

    // Update the camera to the right and up of the spaceship and look at it from the bottom left corner
    else if (spaceTime > 390.0f && spaceTime < 420.0f)
    {
      cameraAngle += 0.05f;
      cameraRadius += 0.01f;
      camera = Camera(glm::vec3(cameraRadius * sin(glm::radians(cameraAngle) + cameraXOffset), 0.0f + cameraYOffset, cameraRadius * cos(glm::radians(cameraAngle)) + cameraZOffset), glm::vec3(0.0f, 1.0f, 0.0f), -cameraAngle, camera.Pitch);
    }
    else {
      update_Camera();
    }
    
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}