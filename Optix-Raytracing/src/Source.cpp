#include <optix.h>
#include <optixu.h>
#include <optixpp_namespace.h>

#include <iostream>
#include "../headers/DisplayManager.h"
#include "../headers/Renderer.h"
#include "../headers/ProgramCreator.h"
#include <cmath>
int main(int argc, char **argv) {
	int width = 512;
	int height = 512;

	int rayCount = 1;
	int entryPoints = 1;
	int stackSize = 800;

	//Iniatalize the display
	DisplayManager displayManager = DisplayManager(width, height, "Raytracing");

	Renderer renderer = Renderer(width, height, rayCount, entryPoints, stackSize);
	ProgramCreator programCreator = ProgramCreator(renderer.getContext());
	programCreator.createRaygenProgram("ray_generation.ptx", "rayGeneration", 0);
	
	float counter = 0.0f;
	while (!displayManager.shouldClose()) {
		float redValue = abs(sin(counter));
		counter += 0.1f;
		programCreator.createProgramVariable1f("redValue", redValue);

		renderer.render(0);
		//Even though the buffer is a optix::float3, the two data types have the same structure so this still works
		std::vector<float3> data = renderer.readOutputBuffer<float3>("result_buffer", width, height);

		displayManager.display(data);
		
	}
	displayManager.cleanUp();
	return 0;

}