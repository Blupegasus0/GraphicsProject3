
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

// Active window
GLFWwindow *window;

// Properties
GLuint sWidth = 1300, sHeight = 720;

// Camera
                     //x-comp,y-comp,z-comp
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

GLfloat spaceTime = 350.0f; // Time variable

GLfloat translateX = 0.0f;

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
  window = glfwCreateWindow(sWidth, sHeight, "COMP3420 - Project 3", 0, 0);
  glfwMakeContextCurrent(window);

  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;

  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, sWidth, sHeight);

  // Setup OpenGL options
  glEnable(GL_DEPTH_TEST);
}

static void render_SpaceShip(Shader& shader, Model& model, Camera& camera)
{
  GLuint shipTexture = loadBMP("assets/textures/ship.bmp");
  GLuint TextureID = glGetUniformLocation(shader.Program, "shipTexture");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shipTexture);

	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	// =======================================================================
	// Create the model matrix
	// =======================================================================
	glm::mat4 spaceShipModel = glm::mat4(1.f);
  spaceShipModel = glm::scale(spaceShipModel, glm::vec3(1.5f,1.5f,1.5f));
  // Rotate the spaceship around the axis with an angle of 1 degrees per second
  // the ship starts at the origin, facing the negative x-axis, so we rotate it so that it faces the positive z-axis 1 degree per second
  spaceShipModel = glm::rotate(spaceShipModel, glm::radians(min(spaceTime, 360.0f)), glm::vec3(1.0f, 0.f, 0.0f));
  spaceShipModel = glm::rotate(spaceShipModel, glm::radians(min(spaceTime, 270.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
  
	// =======================================================================
	// Pass the Model matrix, to the shader as "model"
	// =======================================================================
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceShipModel));

	// =======================================================================
	// Draw the object.
	// =======================================================================
	model.Draw(shader);
}

static void render_SpaceAsteroids(Shader& shader, Model& model, Camera& camera)
{
  shader.Use();
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

  // =======================================================================
  // Create the model matrix
  // =======================================================================
  glm::mat4 asteroidsModel = glm::mat4(1.f);
  asteroidsModel = glm::scale(asteroidsModel, glm::vec3(1.5f, 1.5f, 1.5f));
  // Rotate the asteroids around the axis with an angle of 1 degrees per second
  translateX += .1f;
  asteroidsModel = glm::rotate(asteroidsModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  asteroidsModel = glm::translate(asteroidsModel, glm::vec3(-75.0f + translateX, 10.0f, 15.0f));
  // =======================================================================
  // Pass the Model matrix, to the shader as "model"
  // =======================================================================
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(asteroidsModel));

  // =======================================================================
  // Draw the object.
  // =======================================================================
  model.Draw(shader);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key == GLFW_KEY_W)
    camera.ProcessKeyboard(FORWARD, 0.1f);
  if (key == GLFW_KEY_S)
    camera.ProcessKeyboard(BACKWARD, 0.1f);
  if (key == GLFW_KEY_A)
    camera.ProcessKeyboard(LEFT, 0.1f);
  if (key == GLFW_KEY_D)
    camera.ProcessKeyboard(RIGHT, 0.1f);
  if (key == GLFW_KEY_Q)
    camera.ProcessKeyboard(UP, 0.1f);
  if (key == GLFW_KEY_E)
    camera.ProcessKeyboard(DOWN, 0.1f);
  if (key == GLFW_KEY_R)
  {
    spaceTime = 350.0f;
    translateX = 0.0f;
    camera.Position = glm::vec3(0.0f, 0.0f, 10.0f);
    camera.Yaw = YAW;
    camera.Pitch = PITCH;
  }
  if (key == GLFW_KEY_Z)
		camera.ProcessMouseMovement(-1.0f, 0.0f);
  if (key == GLFW_KEY_X)
    camera.ProcessMouseMovement(1.0f, 0.0f);
  if (key == GLFW_KEY_C)
		camera.ProcessMouseMovement(0.0f, -1.0f);
  if (key == GLFW_KEY_V)
		camera.ProcessMouseMovement(0.0f, 1.0f);
}

// The MAIN function, from here we start our application and run the loop
int main()
{
  init_Resources();

  glfwSetKeyCallback(window, key_callback);

  Shader spaceShipShader("SpaceShipVertex.glsl", "SpaceShipFragment.glsl");

  Shader asteroidsShader("AsteroidsVertex.glsl", "AsteroidsFragment.glsl");
  
  Model spaceShip((GLchar *)"assets/objects/ship.obj");

  Model asteroids((GLchar*)"assets/objects/asteroids.obj");

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)sWidth / (GLfloat)sHeight, 0.1f, 100.0f);

  spaceShipShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(spaceShipShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  asteroidsShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(asteroidsShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  // Iterate this block while the window is open
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();

    // Clear buffers
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Increment the time variable
    spaceTime += .1;
    // Update the camera to zoom out
    if (spaceTime > 360.0f && spaceTime < 363.0f)
      camera.Position = glm::vec3(camera.Position.x, camera.Position.y, spaceTime - 360.0f + camera.Position.z);
    // Update the camera to the right and up of the spaceship and look at it from the bottom left corner
    if (spaceTime > 390.0f && spaceTime < 393.0f)
      camera.Position = glm::vec3(camera.Position.x + .1f, camera.Position.y + .1f, camera.Position.z);
    // check if the spaceship and the asteroids have collided and reset the time variable

    // Render the asteroids
    if (spaceTime > 393.0f)
      render_SpaceAsteroids(asteroidsShader, asteroids, camera);

		// Render the spaceship
    render_SpaceShip(spaceShipShader, spaceShip, camera);
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}