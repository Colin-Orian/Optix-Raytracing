#include "../headers/Renderer.h"

Renderer::Renderer(int windowWidth, int windowHeight, int rayCount, int entryPoints, int stackSize) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	context = optix::Context::create();
	context->setRayTypeCount(rayCount);
	context->setEntryPointCount(entryPoints);
	context->setStackSize(stackSize);
	
	//All of our rendering will have an output buffer with the data being a float3.
	this->createOutputBuffer<optix::float3>("result_buffer", windowWidth, windowHeight);
}

//Launch the rendering process for the OptiX scene
void Renderer::render(int entryPoint) {
	try {
		context->validate();
	}
	catch (optix::Exception e) {
		std::cout << "Validation Failed: " << e.getErrorString() << std::endl;
	}
	
	try {
		context->launch(entryPoint, windowWidth, windowHeight);
		
	}
	catch (optix::Exception e) {
		std::cout << "Launch Failed: " << e.getErrorString() << std::endl;
	}
}

void Renderer::cleanUp() {
	context->destroy();
}

optix::Context Renderer::getContext() {
	return context;
}