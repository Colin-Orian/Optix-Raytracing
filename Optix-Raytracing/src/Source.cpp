#include <optix.h>
#include <optixu.h>
#include <optixpp_namespace.h>

#include <iostream>
#include "../headers/DisplayManager.h"

int main(int argc, char **argv) {
	//Check if OptiX is actually linking properly
	optix::Context context = optix::Context();
	int devices = context->getDeviceCount();
	std::cout << devices << std::endl;

	//Iniatalize the display
	DisplayManager displayManager = DisplayManager(512, 512, "Test");
	
	float x = 0.0f;
	float step = 0.1f;
	bool goPos = true;
	while (!displayManager.shouldClose()) {
		std::vector<float3> data;

		//This code is just to check if the texture can change each frame. We won't be using it except for testing
		if (goPos) {
			x += step;
			goPos = !(x == 1.0f);
		}
		else {
			x -= step;
			goPos = x == 0.0f;
		}
		for (int i = 0; i < 512 * 512; i++) {
			float3 temp;
			temp.x = x;
			temp.y = 0.0f;
			temp.z = 0.0f;
			data.push_back(temp);

		}

		displayManager.display(data);
	}
	displayManager.cleanUp();
	return 0;

}