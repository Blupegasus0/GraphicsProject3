
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

#define numAstroids 150
#define numSlug 15
const GLfloat shipYLimit = 25;
const GLfloat shipRadiusMin = 130;
const GLfloat shipRadiusMax = 145;

// Active window
GLFWwindow* window;

// Properties
GLuint sWidth = 1300, sHeight = 720;

GLfloat torusScale = 130.0f;
//GLfloat torusScale = 1230.0f; // scale for torus2

// Camera
					  //x-comp,y-comp,z-comp
Camera camera(glm::vec3(torusScale * 1.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 0.0f, -20);

GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLfloat cameraZ = 0.0f;

GLfloat cameraOffSet = 0.0f;

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
GLfloat asteroidSpeed = .001f;
GLfloat asteroidXRotation = 0.0f;
GLfloat asteroidYRotation = 0.0f;
GLfloat asteroidZRotation = 0.0f;

GLfloat cameraSpeed = 0.5f;
GLboolean collision = false;

GLfloat cameraAngle = 0.0f;
GLfloat spaceShipAngle = 0.0f;
GLfloat asteroidAngle = 0.0f;

GLfloat shipRadius = torusScale + 10 * 1.0f;
GLfloat cameraRadius = shipRadius - 2;
GLfloat asteroidRadius = torusScale * 1.0f;

GLfloat nextShipRadius;
GLfloat maxShipRadius;
GLfloat minShipRadius;

GLfloat spaceTime = 0.0f;
GLfloat collisionTime = -99999.0f;
GLboolean immunity = false;
GLfloat spaceShipAngleInPlane = 30.0f;
GLfloat cameraAngleInPlane = 1.0f;
glm::vec3 forwardDirection = glm::vec3(1.0f, 0.0f, 1.0f);

GLfloat torusOuterRadius = torusScale;
GLfloat torusInnerRadius = 0.5;
GLfloat torusOuterRingRadius;
GLfloat torusInnerRingRadius;

vector<vector <glm::vec3>> asteroidsData(numAstroids);
Model spaceShip;

vector <Model> asteroids;

vector <Model> smokeParticles(numSlug);
vector <glm::vec4> smokeParticlesData(numSlug);

GLint lives = 3;

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

static void render_Space(Shader& shader, Model& model, Camera& camera, GLuint texture) {
	GLuint TextureID = glGetUniformLocation(shader.Program, "spaceTexture");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	glm::mat4 spaceModel = glm::mat4(1.0f);
	spaceModel = glm::scale(spaceModel, glm::vec3(torusScale));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceModel));

	model.Draw(shader);
}

static void render_SpaceShip(Shader& shader, Model& model, Camera& camera, GLuint texture[])
{
	srand(glfwGetTime());

	glm::mat4 spaceShipModel = glm::mat4(1.f);

	glm::vec3 forwardDirection = glm::normalize(glm::vec3(-sin(glm::radians(spaceShipAngleInPlane - 90.0f)), 0.0f, cos(glm::radians(spaceShipAngleInPlane + 90.0f))));

	if ((collision) && (!immunity)) {
		collisionTime = spaceTime;
		/*
		// temporary until collision animation is implemented
		if (shipY < (shipYLimit + 10)) {
			shipY += 10;
			cameraY += 10;
		} else {
			shipY -= 10;
			cameraY -= 10;
		}

		GLfloat movementSpeed = 0.5f; // Adjust as needed
		shipX += movementSpeed * 10 * forwardDirection.x;
		shipZ += movementSpeed * 10 * forwardDirection.z;
		// ==========

		// Update the camera to follow the ship
		cameraX += movementSpeed * 10 * forwardDirection.x;
		cameraZ += movementSpeed * 10 * forwardDirection.z;
		*/
		lives--;
		cout << "lives: " << lives << endl;
		collision = false;
	}

	//cout << "Space Time: " << spaceTime << endl;
	//cout << "Collision Time: " << collisionTime << endl;


	GLuint TextureID[2] = {};
	TextureID[0] = glGetUniformLocation(shader.Program, "shipTexture");
	TextureID[1] = glGetUniformLocation(shader.Program, "immunityTexture");

	if (((spaceTime - collisionTime) <= 50.0f) && (lives > 0)) {
		immunity = true;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}
	else
	{
		immunity = false;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}

	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	if (spaceShipAngle != 360) {
		spaceShipAngle += 0.1;
		cameraAngle += 0.1;
		camera.Yaw -= 0.1;
	}
	else {
		spaceShipAngle = 0.0;
		cameraAngle = 0.0;
		camera.Yaw = 0.0;
	}

	shipX = shipRadius * sin(glm::radians(spaceShipAngle + 5));
	shipZ = shipRadius * cos(glm::radians(spaceShipAngle + 5));

	cameraX = cameraRadius * sin(glm::radians(cameraAngle));
	cameraZ = cameraRadius * cos(glm::radians(cameraAngle));

	spaceShipModel = glm::translate(spaceShipModel, glm::vec3(shipX, shipY, shipZ));
	model.center = glm::vec3(shipX, shipY, shipZ);

	spaceShipModel = glm::rotate(spaceShipModel, glm::radians(spaceShipAngle + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spaceShipModel));

	model.Draw(shader);
}

static void update_Camera() {
	// Update the camera to follow the ship
	cameraX = cameraRadius * sin(glm::radians(cameraAngle));
	cameraZ = cameraRadius * cos(glm::radians(cameraAngle));

	camera = Camera(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0.0f, 1.0f, 0.0f), -cameraAngle + cameraOffSet, camera.Pitch);
}

static void minMax(Model& model) {
	GLfloat magnitude = glm::distance(model.center, camera.Position);

	GLfloat screenLimit = magnitude * tan(glm::radians(22.5));

	/*cout << "Magnitude: " << magnitude << endl;
	cout << "frontVectorX: " << frontVector.x << " frontVectorY: " << frontVector.y << " frontVectorZ: " << frontVector.z << endl;
	cout << "Front Magnitude: " << frontMagnitude << endl;
	cout << "Screen Limit: " << screenLimit << endl;*/

	maxShipRadius = floor(shipRadius + screenLimit - model.radius);
	minShipRadius = floor(shipRadius - screenLimit - (model.radius*2));

	//cout << "Max Ship Radius: " << maxShipRadius << endl;
	//cout << "Min Ship Radius: " << minShipRadius << endl;
	cout << endl;
}

static GLboolean outScreenBounds(int direction) {
	switch (direction) {
	case (1): {
		if (shipRadius <= minShipRadius)
			return true;
		else
			return false;
	}
	case (2): {
		if (shipRadius >= maxShipRadius)
			return true;
		else
			return false;
	}
	default: {
		cout << "Not an appropriate direction";
	}
	}

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Vertical movement
	if (key == GLFW_KEY_W)
	{
		nextShipX = shipX;
		nextShipY = shipY + 0.5f;
		nextShipZ = shipZ;

		if (shipY < shipYLimit) {
			shipY += 0.5f;
			cameraY += 0.5f;
		}
	}
	if (key == GLFW_KEY_S)
	{
		nextShipX = shipX;
		nextShipY = shipY - 0.5f;
		nextShipZ = shipZ;
		if (shipY > -shipYLimit) {
			shipY -= 0.5f;
			cameraY -= 0.5f;
		}
	}

	// Lateral movement
	if (key == GLFW_KEY_A) {
		nextShipRadius = shipRadius - 0.2;
		if (!outScreenBounds(1)) {
			shipRadius -= 0.2;
		}
	}

	if (key == GLFW_KEY_D) {
		nextShipRadius = shipRadius + 0.2;
		if (!outScreenBounds(2)) {
			shipRadius += 0.2;
		}
	}


	if (key == GLFW_KEY_E)
	{
		cameraOffSet += 0.5f;
	}
	if (key == GLFW_KEY_Q)
	{
		cameraOffSet -= 0.5f;
	}
	if (key == GLFW_KEY_C)
	{
		cameraOffSet = 0.0f;
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

	Shader smokeShader("SmokeVertex.glsl", "SmokeFragment.glsl");

	Model torus((GLchar*)"assets/objects/torus.obj");

	torusOuterRingRadius = ((((torus.radius) * torusScale * .75) - torusScale) * .75) + torusScale;
	torusInnerRadius = torusOuterRingRadius - torusOuterRadius;

	spaceShip = Model((GLchar*)"assets/objects/ship.obj");

	for (int i = 0; i < numAstroids; i++) {
		Model asteroid((GLchar*)"assets/objects/asteroid.obj");
		asteroids.push_back(asteroid);
	}

	glm::mat4 asteroidModels[numAstroids];

	srand(glfwGetTime());

	for (int i = 0; i < numAstroids; i++) {
		asteroidRadius = (rand() % (int)torusScale * .85) + torusScale * .55f;
		asteroidAngle = (rand() % 360);
		asteroidYOffset = (torusScale / 4) * glm::sin(rand() % 360);
		glm::vec3(.4, .6, .8);
		asteroidsData[i].push_back(glm::vec3(asteroidRadius, asteroidAngle, asteroidYOffset));
		asteroidsData[i].push_back(glm::vec3(rand() % 1000, rand() % 1000, rand() % 1000));
	}

	shipX = shipRadius * sin(glm::radians(spaceShipAngle + 5));
	shipY = 0.0f;
	shipZ = shipRadius * cos(glm::radians(spaceShipAngle + 5));

	cameraX = cameraRadius * sin(glm::radians(cameraAngle));
	cameraY = 5.0f;
	cameraZ = cameraRadius * cos(glm::radians(cameraAngle));

	GLuint shipTexture = loadBMP("assets/textures/ship.bmp");
	GLuint immunityTexture = loadBMP("assets/textures/immunity-shield.bmp");
	GLuint asteroidTexture = loadBMP("assets/textures/asteroid.bmp");
	GLuint spaceTexture = loadBMP("assets/images/env_stars.bmp");

	GLuint spaceShipTextures[] = { shipTexture, immunityTexture };

	glm::mat4 projection = glm::perspective(glm::radians(35.0f), (GLfloat)sWidth / (GLfloat)sHeight, 0.1f, 1000000.0f);

	spaceShipShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(spaceShipShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	asteroidShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	spaceShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(spaceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	smokeShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(smokeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Iterate this block while the window is open
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		// Clear buffers
		glClearColor(.8f, .8f, .8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_Space(spaceShader, torus, camera, spaceTexture);
		render_SpaceShip(spaceShipShader, spaceShip, camera, spaceShipTextures);

		update_Camera();

		// Render the asteroids
		asteroidShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		GLuint TextureID = glGetUniformLocation(asteroidShader.Program, "asteroidTexture");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroidTexture);

		for (int i = 0; i < numAstroids; i++) {
			asteroidModels[i] = glm::mat4(1.0f);
			GLdouble asteroidXRotation = rand() % 1000;
			GLdouble asteroidYRotation = rand() % 1000;
			GLdouble asteroidZRotation = rand() % 1000;
			asteroidRadius = asteroidsData[i][0].x;
			asteroidAngle = asteroidsData[i][0].y;
			asteroidYOffset = asteroidsData[i][0].z;
			GLfloat x = asteroidRadius * sin(glm::radians(asteroidAngle + 20));
			GLfloat z = asteroidRadius * cos(glm::radians(asteroidAngle + 20));

			asteroidModels[i] = glm::translate(asteroidModels[i], glm::vec3(-x, asteroidYOffset, z));
			asteroids[i].center = glm::vec3(-x, asteroidYOffset, z);

			asteroidAngle += 0.1f;
			if (asteroidAngle > 360.0f) {
				asteroidAngle = 0.0f;
			}
			asteroidsData[i][0].y = asteroidAngle;
			asteroidModels[i] = glm::rotate(asteroidModels[i], spaceTime / 10, glm::normalize(glm::vec3(asteroidsData[i][1].x, asteroidsData[i][1].y, asteroidsData[i][1].z)));
			glUniformMatrix4fv(glGetUniformLocation(asteroidShader.Program, "model"), 1,
				GL_FALSE, glm::value_ptr(asteroidModels[i]));
			asteroids[i].Draw(asteroidShader);
			asteroidSpeed += 0.0001f;
		}

		if (!immunity) {
			// Check for collision
			for (int i = 0; i < numAstroids; i++) {
				if (glm::distance(spaceShip.center, asteroids[i].center) <= spaceShip.radius + asteroids[i].radius) {
					std::cout << "Collision detected" << std::endl;
					collision = true;
					break;
				}
			}
		}

		if (smokeParticles.size() < numSlug) {
			smokeParticles.push_back(Model((GLchar*)"assets/objects/smoke.obj"));
			smokeParticlesData.push_back(glm::vec4(shipX-1, shipY, shipZ, 0.0));
		}
		else {
			smokeParticles.erase(smokeParticles.begin());
			smokeParticles.push_back(Model((GLchar*)"assets/objects/smoke.obj"));
			smokeParticlesData.erase(smokeParticlesData.begin());
			smokeParticlesData.push_back(glm::vec4(shipX-1, shipY, shipZ, 0.0));
		}

		smokeShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(smokeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		
		for (int i = 0; i < numSlug; i++) {
			glm::mat4 smokeModel = glm::mat4(1.0f);
			smokeModel = glm::translate(smokeModel, glm::vec3(smokeParticlesData[i].x, smokeParticlesData[i].y, smokeParticlesData[i].z));
			smokeModel = glm::scale(smokeModel, glm::vec3(.09f));
			glUniformMatrix4fv(glGetUniformLocation(smokeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(smokeModel));
			smokeParticles[i].Draw(smokeShader);
		}

		for (int i = 0; i < smokeParticlesData.size(); i++) {
			smokeParticlesData[i].w += 0.1f;
			smokeParticlesData[i].y += smokeParticlesData[i].w;
		}

		if (spaceTime < 0.2f)
			minMax(spaceShip);

		glfwSwapBuffers(window);

		spaceTime += 0.1f;

		spaceShipAngle += 0.1f;
		cameraAngle += 0.1f;

		// handle death condition
		if (lives == 0)
			break;

	} // running loop

	int score = (int)spaceTime;
	cout << "Score: " << score << endl;
	
	glfwTerminate();
	return 0;
}