
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

#define numAstroids 50

// Active window
GLFWwindow* window;

// Properties
GLuint sWidth = 1300, sHeight = 720;

GLfloat torusScale = 130.0f;

// Camera
					  //x-comp,y-comp,z-comp
Camera camera(glm::vec3(torusScale * 1.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 0.0f, -15);

GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLfloat cameraZ = 0.0f;

GLfloat asteroidX = 0.0f;
GLfloat asteroidY = 0.0f;
GLfloat asteroidZ = 0.0f;

GLfloat shipX = 0.0f;
GLfloat shipY = 0.0f;
GLfloat shipZ = 0.0f;
GLfloat nextShipX = 0.0f;
GLfloat nextShipY = 0.0f;
GLfloat nextShipZ = 0.0f;

GLfloat asteroidXOffset = 0.0f;
GLfloat asteroidYOffset = 0.0f;
GLfloat asteroidZOffset = 0.0f;
GLfloat asteroidScale = 0.0f;
GLfloat asteroidDisplacement = 0.0f;
GLfloat asteroidRotation = 0.0f;
GLfloat asteroidSpeed = 0.001f;
GLfloat asteroidXRotation = 0.0f;
GLfloat asteroidYRotation = 0.0f;
GLfloat asteroidZRotation = 0.0f;

GLfloat cameraSpeed = 0.5f;
GLboolean collision = false;

GLfloat cameraAngle = 0.0f;
GLfloat spaceShipAngle = 0.0f;
GLfloat asteroidAngle = 0.0f;

GLfloat cameraRadius = torusScale * 1.0f;
GLfloat shipRadius = torusScale * 1.0f;
GLfloat asteroidRadius = torusScale * 1.0f;

GLfloat spaceTime = 0.0f;
GLfloat spaceShipAngleInPlane = 10.0f;
GLfloat cameraAngleInPlane = 1.0f;
glm::vec3 forwardDirection = glm::vec3(1.0f, 0.0f, 1.0f);

GLfloat torusOuterRadius = torusScale;
GLfloat torusInnerRadius = 0.5;
GLfloat torusOuterRingRadius;
GLfloat torusInnerRingRadius;
vector<vector <GLfloat>> torusOuterRingX;
vector<vector <GLfloat>> torusOuterRingZ;
vector<vector <GLfloat>> torusInnerRingX;
vector<vector <GLfloat>> torusInnerRingZ;

Model spaceShip;

vector <Model> asteroids;

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

static void render_SpaceShip(Shader& shader, Model& model, Camera& camera, GLuint texture)
{
	srand(glfwGetTime());

	glm::vec3 forwardDirection = glm::normalize(glm::vec3(-sin(glm::radians(spaceShipAngleInPlane - 90.0f)), 0.0f, cos(glm::radians(spaceShipAngleInPlane + 90.0f))));

	if (collision) {

		// temporary until collision animation is implemented
		if (rand() % 2 == 1)
			shipRadius -= 10;
		else
			shipRadius += 10;

		GLfloat movementSpeed = 0.5f; // Adjust as needed
		shipX += movementSpeed * 10 * forwardDirection.x;
		shipZ += movementSpeed * 10 * forwardDirection.z;
		// ==========

		// Update the camera to follow the ship
		cameraX += movementSpeed * 10 * forwardDirection.x;
		cameraZ += movementSpeed * 10 * forwardDirection.z;

		collision = false;
	}

	GLuint TextureID = glGetUniformLocation(shader.Program, "shipTexture");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	glm::mat4 spaceShipModel = glm::mat4(1.f);

	// Translate ship long ship radius
	if (spaceShipAngle != 360) {
		spaceShipAngle += 0.01;
		cameraAngle += 0.01;
		camera.Yaw -= 0.01;
	}
	else {
		spaceShipAngle += 0.0;
		cameraAngle += 0.0;
		camera.Yaw -= 0.0;
	}

	shipX = shipRadius * sin(glm::radians(spaceShipAngle + 5));
	shipY = 0.0f;
	shipZ = shipRadius * cos(glm::radians(spaceShipAngle + 5));

	cameraX = cameraRadius * sin(glm::radians(cameraAngle));
	cameraY = 5.0f;
	cameraZ = cameraRadius * cos(glm::radians(cameraAngle));
	//

	spaceShipModel = glm::translate(spaceShipModel, glm::vec3(shipX, shipY, shipZ));
	model.center = glm::vec3(shipX, shipY, shipZ);

	//std::cout << "X: " << shipX << " Y: " << shipY << " Z: " << shipZ << endl;

	spaceShipModel = glm::rotate(spaceShipModel, glm::radians(spaceShipAngle + 5.0f + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	spaceShipModel = glm::rotate(spaceShipModel, glm::radians(spaceShipAngleInPlane), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceShipModel));

	model.Draw(shader);
}

static void render_Space(Shader& shader, Model& model, Camera& camera, GLuint texture) {
	GLuint TextureID = glGetUniformLocation(shader.Program, "spaceTexture");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	glm::mat4 spaceModel = glm::mat4(1.0f);
	spaceModel = glm::scale(spaceModel, glm::vec3(torusScale));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceModel));
	//std::cout << "Torus Radius: " << model.radius << endl;
	model.Draw(shader);
}

static void update_Camera() {
	camera = Camera(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0.0f, 1.0f, 0.0f), camera.Yaw, camera.Pitch);
}


static GLfloat getAngle() {
	glm::vec3 shipPosition = glm::vec3(nextShipX, 0.0f, nextShipZ);
	GLfloat shipMagnitude = sqrt((nextShipX * nextShipX) + (nextShipZ * nextShipZ));
	//cout << "Ship Magnitude: " << shipMagnitude << endl;
	//cout << "Cosine of Angle: " << (nextShipX / shipMagnitude) << endl;
	return (acos(nextShipX / shipMagnitude) * 180 * 7) / 22;


}

static GLboolean outOfBounds(GLfloat angle) {
	GLfloat x, z, magnitude;
	/*cout << "Angle: " << angle << endl;
	cout << "X: " << shipX << endl;
	cout << "Y: " << shipY << endl;
	cout << "Z: " << shipZ << endl;
	cout << "Next X: " << nextShipX << endl;
	cout << "Next Y: " << nextShipY << endl;
	cout << "Next Z: " << nextShipZ << endl;*/
	if (nextShipZ >= 0) {
		x = torusScale * cos(angle * M_PI / 180);
		z = torusScale * sin(angle * M_PI / 180);
		magnitude = sqrt(pow((x - nextShipX), 2) + pow((nextShipY), 2) + pow((z - nextShipZ), 2));
		if (magnitude >= torusInnerRadius)
			return true;
		else
			return false;
	}
	if (nextShipZ < 0) {
		x = torusScale * cos(-angle * M_PI / 180);
		z = torusScale * sin(-angle * M_PI / 180);
		magnitude = sqrt(pow((x - nextShipX), 2) + pow((nextShipY), 2) + pow((z - nextShipZ), 2));
		if (magnitude >= torusInnerRadius)
			return true;
		else
			return false;
	}

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W)
	{
		glm::vec3 forwardDirection = glm::normalize(glm::vec3(-sin(glm::radians(spaceShipAngleInPlane - 90.0f)), 0.0f, cos(glm::radians(spaceShipAngleInPlane + 90.0f))));

		GLfloat movementSpeed = 0.5f; // Adjust as needed
		nextShipX = shipX + movementSpeed * forwardDirection.x;
		nextShipY = shipY;
		nextShipZ = shipZ + movementSpeed * forwardDirection.z;

		if (!outOfBounds(getAngle())) {
			GLfloat movementSpeed = 0.5f; // Adjust as needed
			shipX += movementSpeed * forwardDirection.x;
			shipZ += movementSpeed * forwardDirection.z;

			// Update the camera to follow the ship
			cameraX += movementSpeed * forwardDirection.x;
			cameraZ += movementSpeed * forwardDirection.z;

		}
	}

	if (key == GLFW_KEY_S)
	{
		// Calculate direction vector based on ship's rotation around y-axis
		glm::vec3 forwardDirection = glm::normalize(glm::vec3(-sin(glm::radians(spaceShipAngleInPlane - 90.0f)), 0.0f, cos(glm::radians(spaceShipAngleInPlane + 90.0f))));

		GLfloat movementSpeed = 0.5f; // Adjust as needed
		nextShipX = shipX - movementSpeed * forwardDirection.x;
		nextShipZ = shipZ - movementSpeed * forwardDirection.z;

		if (!outOfBounds(getAngle())) {
			GLfloat movementSpeed = 0.5f; // Adjust as needed
			shipX -= movementSpeed * forwardDirection.x;
			shipZ -= movementSpeed * forwardDirection.z;

			// Update the camera to follow the ship
			cameraX -= movementSpeed * forwardDirection.x;
			cameraZ -= movementSpeed * forwardDirection.z;

		}
	}

	// Lateral movement
	if (key == GLFW_KEY_A)
		shipRadius -= 0.2;
	//spaceShipAngleInPlane += 1.0f;
	if (key == GLFW_KEY_D)
		shipRadius += 0.2;
	//spaceShipAngleInPlane -= 1.0f;

	if (key == GLFW_KEY_Q)
	{
		// Rotate the camera around the ship.center
		camera.ProcessMouseMovement(4.0f, 0.0f);
		glm::mat2 rotationMatrix = glm::mat2(cos(glm::radians(cameraAngleInPlane)), sin(glm::radians(cameraAngleInPlane)), -sin(glm::radians(cameraAngleInPlane)), cos(glm::radians(cameraAngleInPlane)));
		glm::vec2 rotationVector = rotationMatrix * glm::vec2(cameraX - spaceShip.center.x, cameraZ - spaceShip.center.z) + glm::vec2(spaceShip.center.x, spaceShip.center.z);
		cameraX = rotationVector.x;
		cameraZ = rotationVector.y;

	}

	if (key == GLFW_KEY_E)
	{
		// Rotate the camera around the ship.center
		camera.ProcessMouseMovement(-4.0f, 0.0f);
		glm::mat2 rotationMatrix = glm::mat2(cos(glm::radians(cameraAngleInPlane)), -sin(glm::radians(cameraAngleInPlane)), sin(glm::radians(cameraAngleInPlane)), cos(glm::radians(cameraAngleInPlane)));
		glm::vec2 rotationVector = rotationMatrix * glm::vec2(cameraX - spaceShip.center.x, cameraZ - spaceShip.center.z) + glm::vec2(spaceShip.center.x, spaceShip.center.z);
		cameraX = rotationVector.x;
		cameraZ = rotationVector.y;
	}

	if (key == GLFW_KEY_U)
	{
		nextShipX = shipX;
		nextShipY = shipY + 0.5f;
		nextShipZ = shipZ;
		if (!outOfBounds(getAngle())) {
			shipY += 0.5f;
			cameraY += 0.5f;
		}
	}

	if (key == GLFW_KEY_J)
	{
		nextShipX = shipX;
		nextShipY = shipY - 0.5f;
		nextShipZ = shipZ;
		if (!outOfBounds(getAngle())) {
			shipY -= 0.5f;
			cameraY -= 0.5f;
		}
	}

	if (key == GLFW_KEY_R)
	{
		spaceShipAngle = 0.0f;
		spaceShipAngleInPlane = 0.0f;
		cameraAngle = 0.0f;
		shipY = 0.0f;

		shipX = shipRadius * sin(glm::radians(spaceShipAngle + 5));
		shipZ = shipRadius * cos(glm::radians(spaceShipAngle + 5));
		camera.Yaw = 0.0f;
		cameraX = cameraRadius * sin(glm::radians(cameraAngle));
		cameraY = 5.0f;
		cameraZ = cameraRadius * cos(glm::radians(cameraAngle));
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

	Model torus((GLchar*)"assets/objects/torus.obj");

	//torusOuterRing = torusScale * torus.radius * 2;
	//torusInnerRing = (4 * torusOuterRadius) - torusOuterRing;
	//torusInnerRadius = (torusOuterRing - torusInnerRing) / M_PI;

	torusOuterRingRadius = ((((torus.radius) * torusScale * .75) - torusScale) * .75) + torusScale;
	torusInnerRadius = torusOuterRingRadius - torusOuterRadius;

	//torusOuterBound();

	spaceShip = Model((GLchar*)"assets/objects/ship.obj");

	for (int i = 0; i < numAstroids; i++) {
		Model asteroid((GLchar*)"assets/objects/asteroid.obj");
		asteroids.push_back(asteroid);
	}

	glm::mat4 asteroidModels[numAstroids];

	srand(glfwGetTime());

	for (int i = 0; i < numAstroids; i++) {
		asteroidModels[i] = glm::mat4(1.0f);
		asteroidRadius = (rand() % (int)torusScale * .85) + torusScale * .55f;
		asteroidRadius = torusScale * 1.0f;
		asteroidAngle = (rand() % 360);
		asteroidYOffset = (rand() % 30) - 15;
		GLfloat x = asteroidRadius * sin(glm::radians(asteroidAngle + 20));
		GLfloat y = 0.0f + asteroidYOffset;
		GLfloat z = asteroidRadius * cos(glm::radians(asteroidAngle + 20));
		asteroidModels[i] = glm::translate(asteroidModels[i], glm::vec3(x, y, z));
		asteroids[i].center = glm::vec3(x, y, z);
	}

	shipX = shipRadius * sin(glm::radians(spaceShipAngle + 5));
	shipY = 0.0f;
	shipZ = shipRadius * cos(glm::radians(spaceShipAngle + 5));

	cameraX = cameraRadius * sin(glm::radians(cameraAngle));
	cameraY = 5.0f;
	cameraZ = cameraRadius * cos(glm::radians(cameraAngle));

	GLuint shipTexture = loadBMP("assets/textures/ship.bmp");
	GLuint spaceTexture = loadBMP("assets/images/env_stars.bmp");

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
		glClearColor(.8f, .8f, .8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_Space(spaceShader, torus, camera, spaceTexture);
		render_SpaceShip(spaceShipShader, spaceShip, camera, shipTexture);

		update_Camera();

		// Render the asteroids
		asteroidShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		for (int i = 0; i < numAstroids; i++) {
			asteroidModels[i] = glm::rotate(asteroidModels[i], asteroidSpeed, glm::vec3(0.2f, 0.8f, 0.6f));
			glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "model"), 1,
				GL_FALSE, glm::value_ptr(asteroidModels[i]));
			asteroids[i].Draw(asteroidShader);
		}

		for (int i = 0; i < numAstroids; i++) {
			// Check for collision
			if (glm::distance(spaceShip.center, asteroids[i].center) <= spaceShip.radius + asteroids[i].radius) {
				std::cout << "Collision detected" << std::endl;
				collision = true;
				break;
			}
		}
		// Swap the buffers
		glfwSwapBuffers(window);

		// Increment the time variable
		spaceTime += 0.1f;
	} // running loop

	glfwTerminate();
	return 0;
}