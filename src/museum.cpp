#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//LearnOpenGL libs
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "camera.h"

//Constants, namespace setup
using namespace std;
#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

//Function declarations
void mouseCallback(GLFWwindow *window, double x, double y);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);

//Globals
double mouseLastX = SCR_WIDTH / 2.0f;
double mouseLastY = SCR_HEIGHT / 2.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char **argv) {
	//Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialize OpenGL context
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		puts("GLAD failed to init");
		return -1;
	}

	//Configure OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	//Load everything
	Model model("resource/museum.obj");
	puts("Model created!");
	Shader shader("src/shader.vs", "src/shader.fs");
	puts("Shader created!");

	//Pre-render logic
	camera.Position.x = -0.11f;
	camera.Position.y = -1.33f;
	camera.Position.z = 2.3f;

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//Get time to render frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Process user input
		processInput(window);

		//Clear the buffer
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the current shader
		shader.use();

		//Camera perspective projection
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		//Model transform setup
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, -1.75, 0.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));

		//Draw model
		shader.setMat4("model", modelMatrix);
		model.Draw(shader);

		//Ending stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//GLFW stop
	glfwTerminate();
}

void processInput(GLFWwindow *window) {
	//Escape key to exit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//Movement keys
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	//Change viewport size to match buffer size
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow *window, double x, double y) {
	//Track mouse position
	float xoffset = x - mouseLastX;
	float yoffset = mouseLastY - y;
	mouseLastX = x;
	mouseLastY = y;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	//Process mouse scrolling to zoom
	camera.ProcessMouseScroll(yoffset);
}
