#include <optix.h>
#include <optixu.h>
#include <optixpp_namespace.h>


#include <iostream>

#define GLFW_DLL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
int main(int argc, char **argv) {
	optix::Context context = optix::Context();
	int devices = context->getDeviceCount();
	std::cout << devices << std::endl;

	GLFWwindow* window;
	if (!glfwInit()) {
		return -1;
	}
	window = glfwCreateWindow(512, 512, "Test", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 1, 1);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{	
		
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}