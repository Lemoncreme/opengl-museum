#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <iostream>

#include "Object.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLfloat theta[3] = {0, 0, 0};
const GLsizei MODELS = 4;
GLuint VAO[MODELS];
GLint modelSelection = 0;

const char *vertexShaderSource = "#version 150\n"
		"in vec4 vPosition;\n"
		"in vec4 vColor;\n"
		"out vec4 color;\n"
		"uniform vec3 theta;\n"
		"void main()\n"
		"{\n"
		"   //Compute the sines and cosines of theta for each of\n"
		"   //the three axes in one computation.\n"
		"   vec3 angles = radians(theta);\n"
		"   vec3 c = cos(angles);\n"
		"   vec3 s = sin(angles);\n"
		"   //Remember: these matrices are column major.\n"
		"   mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,\n"
		"                  0.0, c.x, s.x, 0.0,\n"
		"                  0.0, -s.x, c.x, 0.0,\n"
		"                  0.0, 0.0, 0.0, 1.0);\n"
		"   mat4 ry = mat4(c.y, 0.0, -s.y, 0.0,\n"
		"                  0.0, 1.0, 0.0, 0.0,\n"
		"                  s.y, 0.0, c.y, 0.0,\n"
		"                  0.0, 0.0, 0.0, 1.0);\n"
		"   mat4 rz = mat4(c.z, -s.z, 0.0, 0.0,\n"
		"                  s.z, c.z, 0.0, 0.0,\n"
		"                  0.0, 0.0, 1.0, 0.0,\n"
		"                  0.0, 0.0, 0.0, 1.0);\n"
		"   color = vColor;\n"
		"   gl_Position = rz * ry * rx * vPosition;\n"
		"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";


int main(){
	//Initialize GLFW
	glfwInit();

	//OpenGL version hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Museum", NULL, NULL);
	if(window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	//Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		puts("Failed to initialize GLEW");
		glfwTerminate();
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//VAOs
	glGenVertexArrays(MODELS, VAO);

	//Load cube object
	Object models[MODELS];
	glBindVertexArray(VAO[0]);
	models[0] = Object("cube.obj");
	models[0].load(shaderProgram);
	glBindVertexArray(VAO[1]);
	models[1] = Object("Pikachu.obj");
	models[1].load(shaderProgram);
	glBindVertexArray(VAO[2]);
	models[2] = Object("thinker.obj");
	models[2].load(shaderProgram);
	glBindVertexArray(VAO[3]);
	models[3] = Object("museum.obj");
	models[3].load(shaderProgram);

	//uniforms
	GLint thetaUniform = glGetUniformLocation(shaderProgram, "theta");

	//bind callbacks
	glfwSetKeyCallback(window, keyCallback);

	//Rendering
	while(!glfwWindowShouldClose(window)){

		// Process window input
		processInput(window);

		// Clear buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Try to draw the cube
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[modelSelection]);
		glUniform3fv(thetaUniform, 1, theta);
		models[modelSelection].draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

		theta[0] += 0.5;
		theta[1] += 0.5;
		theta[2] += 0.5;
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height){
	printf("FRAMEBUFFER SIZE %d %d\n", width, height);
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0,0,width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		exit(EXIT_SUCCESS);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		modelSelection = (modelSelection + 1) % MODELS;
		glBindVertexArray(VAO[modelSelection]);
	}
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	printf("WINDOW SIZE %d %d\n", width, height);
	glfwSetWindowSize(window, width, height);
	glViewport(0,0,width, height);
}
