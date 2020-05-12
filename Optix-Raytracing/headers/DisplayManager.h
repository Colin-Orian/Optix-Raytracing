#pragma once
#define GLFW_DLL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>
struct float3 {
	float x;
	float y;
	float z;
};

class DisplayManager {
private:
	GLFWwindow* window;
	int width;
	int height;

	/*
	//Used https://learnopengl.com/Advanced-OpenGL/Framebuffers
	These two shaders will create a plane right in front of the camera and display a texture on it.
	This is commonly used when you're using OpenGL to display results of another rendering API
	*/
	const char* vertexSource =
		"#version 330 core\n" 
		"in vec2 vPosition;\n" 
		"in vec2 vTex;\n" 
		"out vec2 fTexCoords;\n" 
		"void main() {\n" 
			"fTexCoords = vTex;\n" 
			"gl_Position = vec4(vPosition.x, vPosition.y, 0.0f, 1.0f);\n" 
		"}\n";
	const char* fragSource =
		"#version 330 core\n"
		"uniform sampler2D tex;\n"
		"in vec2 fTexCoords;\n"
		"out vec4 finalColour;"
		"void main() {\n"
			"	vec3 color = texture(tex, fTexCoords).xyz;\n"
			"	finalColour = vec4(color, 1.0f);\n"
		"}\n";
	//
	int program;

	GLfloat* vertices;
	GLfloat* texturesCoords;

	GLuint vBuffer;
	GLuint tBuffer;
	GLuint iBuffer;
	GLuint objVAO;

	bool checkShader(int shader);
	bool checkProgram();
	void initializeProgams();
	void initializePlane();
	void createTexture(GLuint* texLoc, std::vector<float3> texture);
public:
	DisplayManager(int width, int height, std::string windowName);

	void display(std::vector<float3> texture);

	bool shouldClose();

	void cleanUp();
};
