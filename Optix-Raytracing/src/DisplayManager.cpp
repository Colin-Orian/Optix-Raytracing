#include "../headers/DisplayManager.h"

DisplayManager::DisplayManager(int width, int height, std::string windowName) {
	this->width = width;
	this->height = height;
	if (!glfwInit()) {
		throw -1;
	}
	window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
	
	if (!window) {
		throw - 1;
	}
	glfwMakeContextCurrent(window);


	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("Error starting GLEW: %s\n", glewGetErrorString(error));
	}

	initializeProgams();
	initializePlane();
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 0, 1);
	glViewport(0, 0, width, height);

	glUseProgram(program);

	glfwSwapInterval(1); 

}

void DisplayManager::initializePlane() {
	//positions and tex coords taken from https://learnopengl.com/code_viewer.php?code=advanced/framebuffers_quad_vertices
	float quadVertices[] = {
		// positions 
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f,

		-1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f,  1.0f
	};
	// texCoords
	float quadTex[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	int numVert = 12;
	int numTex = 12;
	vertices = new GLfloat[12];
	texturesCoords = new GLfloat[12];
	for (int i = 0; i < 12; i++) {
		vertices[i] = quadVertices[i];
		texturesCoords[i] = quadTex[i];
	}

	int numIndices = 6;
	float quadIndex[] = {
		0, 1, 2, //bot left triangle
		3, 4, 5
	};


	GLuint* indices = new GLuint[numIndices];
	for (int i = 0; i < numIndices; i++) {
		indices[i] = quadIndex[i];
	}

	glGenVertexArrays(1, &objVAO);
	glBindVertexArray(objVAO);

	//Bind vertex buffer
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVert * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//bind texture coords buffer
	glGenBuffers(1, &tBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tBuffer);
	glBufferData(GL_ARRAY_BUFFER, numTex * sizeof(GLfloat), texturesCoords, GL_STATIC_DRAW);

	//Bind index buffer
	glGenBuffers(1, &iBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);


}
//Check if the shader succesfully compiled
bool DisplayManager::checkShader(int shader) {
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	return result == GL_TRUE;
}

//Check if the program succesfully linked
bool DisplayManager::checkProgram() {
	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	return result == GL_TRUE;
}
//Compile and link the vertex and fragment shader
void DisplayManager::initializeProgams() {
	//Vertex Shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar **)&vertexSource, 0);
	glCompileShader(vertexShader);
	if (!checkShader(vertexShader)) {
		std::cout << "Error compiling Vertex Shader! " << std::endl;
	}
	
	//Fragment shader
	int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar **)&fragSource, 0);
	glCompileShader(fragShader);
	if (!checkShader(fragShader)) {
		std::cout << "Error compling Frag Shader! " << std::endl;
	}

	//Link shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	if (!checkProgram()) {
		std::cout << "Error linking program! " << std::endl;
	}	
}
/*
Create and bind a texture from a given set of pixels, the texture parameter. 
*/
void DisplayManager::createTexture(GLuint* texLoc, std::vector<float3> texture) {
	glGenTextures(1, texLoc);
	glBindTexture(GL_TEXTURE_2D, *texLoc);
	glActiveTexture(GL_TEXTURE0);
	
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB,
		GL_FLOAT, texture.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
/*Display the rendered texture onto a plane right in front of the camera. 
	By rendering it right in front, it will appear that the rendered texture is the scene itself.
*/
void DisplayManager::display(std::vector<float3> data) {
	//Normal OpenGL rendering steps
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objVAO);

	//Bind the vertex buffer to the vPosition uniforn
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	int vPosition = glGetAttribLocation(program, "vPosition");
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	//Same as above but for texture coords
	glBindBuffer(GL_ARRAY_BUFFER, tBuffer);
	int vTex = glGetAttribLocation(program, "vTex");
	glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vTex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
	
	//Bind the rendered texture
	GLuint texLoc;
	createTexture(&texLoc, data);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texLoc);


	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, NULL);
	glFinish();
	

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}

bool DisplayManager::shouldClose() {
	return glfwWindowShouldClose(window);
}

void DisplayManager::cleanUp() {
	glfwTerminate();
	delete[] texturesCoords;
	delete[] vertices;
}