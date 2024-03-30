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

// Active window
GLFWwindow *window;

// Properties
GLuint sWidth = 1800, sHeight = 800;

// Camera
                     //x-comp,y-comp,z-comp
Camera camera(glm::vec3(0.0f, 0.0f, 100.f));

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

// The MAIN function, from here we start our application and run the loop
int main()
{
  init_Resources();

  Shader droneShader("Vertex.glsl", "Fragment.glsl");

  Shader asteroidsShader("VertexAsteroids.glsl", "FragmentAsteroids.glsl");
  
  Model drone((GLchar *)"assets/objects/Drone.obj");

  Model asteroids((GLchar*)"assets/objects/asteroids.obj");

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)sWidth / (GLfloat)sHeight,
                                          0.1f, 1000.0f);
  asteroidsShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(asteroidsShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  droneShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  // Iterate this block while the window is open
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();

    // Clear buffers
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Add transformation matrices

    // =======================================================================
    // Create the View matrix
    // =======================================================================
    asteroidsShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(asteroidsShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    asteroidsShader.Use();

    // =======================================================================
    // Create the model matrix
    // =======================================================================
    glm::mat4 asteroidModel = glm::mat4(1);
    asteroidModel = glm::rotate(asteroidModel, glm::radians(-100.f), glm::vec3(.0f, 1.0f, .0f));
    asteroidModel = glm::translate(asteroidModel,glm::vec3(glfwGetTime(), 5.f, -10.f));
    
    // =======================================================================
    // Pass the Model matrix, to the shader as "model"
    // =======================================================================
    glUniformMatrix4fv(glGetUniformLocation(asteroidsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(asteroidModel));

    // =======================================================================
    // Draw the object.
    // =======================================================================
    asteroids.Draw(asteroidsShader);

    droneShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

    // =======================================================================
    // Create the model matrix
    // =======================================================================
    
    droneShader.Use();

    glm::mat4 droneModel = glm::mat4(1);
    droneModel = glm::translate(droneModel, glm::vec3(-2.f, -2.0f, 90.0f));
    droneModel = glm::rotate(droneModel, glm::radians(165.f), glm::vec3(.0f, 1.0f, .0f));
    droneModel = glm::rotate(droneModel, glm::radians(-14.f), glm::vec3(1.0f, .0f, .0f));

    // =======================================================================
    // Pass the Model matrix, to the shader as "model"
    // =======================================================================
    glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(droneModel));


    // =======================================================================
    // Draw the object.
    // =======================================================================
    drone.Draw(droneShader);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}