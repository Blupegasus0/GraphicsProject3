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
GLuint sWidth = 800, sHeight = 800;

// Camera
Camera camera(glm::vec3(7.0f, 14.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -160.0f, -60.0f);

GLfloat droneOffsetY = 0.0;

GLboolean droneDirectionForward = true;

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

  Model drone((GLchar *)"Drone.obj");

  glm::mat4 projection = glm::perspective(1.0f, (GLfloat)sWidth / (GLfloat)sHeight,
                                          1.0f, 100.0f);
  droneShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "projection"),
                     1, GL_FALSE, glm::value_ptr(projection));

  // Iterate this block while the window is open
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();

    // Clear buffers
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Add transformation matrices

    // =======================================================================
    // Step 4. create the View matrix
    // =======================================================================
    droneShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

    // =======================================================================
    // Step 5. Create the model matrix (We'll call it "planetModel" in this case)
    // =======================================================================
    droneShader.Use();
    glm::mat4 droneModel = glm::mat4(1);

    // Modify the model matrix with scaling, translation, rotation, etc
    droneModel = glm::scale(droneModel, glm::vec3(1.1f));
    if (droneOffsetY >= 15.f)
    {
      droneDirectionForward = false;
    }
    else if (droneOffsetY <= -15.f)
    {
      droneDirectionForward = true;
    }
    if (droneDirectionForward)
    {
      droneOffsetY += 0.1;
    }
    else
    {
      droneOffsetY -= 0.1;
    }
    // Adjust the phase to ensure the cosine wave starts at the correct position
    droneModel = glm::translate(droneModel, glm::vec3(0.0f, 0.0f, droneOffsetY));

    // =======================================================================
    // Step 6. Pass the Model matrix, "planetModel", to the shader as "model"
    // =======================================================================
    glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(droneModel));

    // =======================================================================
    // Step 7.  Draw the object.
    // =======================================================================
    drone.Draw(droneShader);
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}