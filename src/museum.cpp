#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "camera.h"

using namespace std;

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);

	int main(int argc, char **argv) {
	//Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	//Initialize OpenGL context
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		puts("GLAD failed to init");
		return -1;
	}

	//Configure OpenGL state
	glEnable(GL_DEPTH_TEST);

	//Load everything
	Model model("resource/museum.obj");
	puts("Model created!");
	Camera camera(0, 0, 0, 0, 0, 0, 0, 0);
	puts("Camera created!");
	Shader shader("src/shader.vert", "src/shader.frag");
	puts("Shader created!");

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		model.Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

