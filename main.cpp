#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <iostream>

#include "Object.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLfloat theta[3] = {0, 0, 0};
GLfloat scale[2] = {0.5, 0.5};
const GLsizei MODELS = 4;
GLuint VAO[MODELS];
GLint modelSelection = 0;
double mousex = 0;
double mousey = 0;
bool leftClick = false;
bool rightClick = false;
double mousedx = 0;
double mousedy = 0;
double scroll = 1;

char *vertexShaderSource;
char *fragmentShaderSource;


int main(){
	//Initialize GLFW
	glfwInit();

    //read in from file
    FILE *vertexShaderFile = fopen("shader.vert", "r");
    fseek(vertexShaderFile, 0, SEEK_END);
    long fsize = ftell(vertexShaderFile);
    fseek(vertexShaderFile, 0, SEEK_SET);  //same as rewind(f);

    vertexShaderSource = (char *) malloc(fsize + 1);
    fread(vertexShaderSource, fsize, 1, vertexShaderFile);
    fclose(vertexShaderFile);

    FILE *fragmentShaderFile = fopen("shader.frag", "r");
    fseek(fragmentShaderFile, 0, SEEK_END);
    fsize = ftell(fragmentShaderFile);
    fseek(fragmentShaderFile, 0, SEEK_SET);  //same as rewind(f);

    fragmentShaderSource = (char *) malloc(fsize + 1);
    fread(fragmentShaderSource, fsize, 1, fragmentShaderFile);
    fclose(fragmentShaderFile);

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
    GLint scaleUniform = glGetUniformLocation(shaderProgram, "scale");

	//bind callbacks
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
	//Rendering
	while(!glfwWindowShouldClose(window)){

		// Process window input
		processInput(window);
		mousedx = mousex;
		mousedy = mousey;
		glfwGetCursorPos(window, &mousex, &mousey);
		mousedx -= mousex;
		mousedy -= mousey;

		// Clear buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Try to draw the cube
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[modelSelection]);
		glUniform3fv(thetaUniform, 1, theta);
        glUniform2fv(scaleUniform, 1, scale);
		models[modelSelection].draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

		if(leftClick){
			theta[0] -= mousedy / 4.0;
			theta[1] += mousedx / 4.0;
		}

        scale[0] = (scale[1] = scroll);
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

void mouseCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			leftClick = true;
		}
		if (action == GLFW_RELEASE) {
			leftClick = false;
		}
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    scroll += yoffset/10.0;
}