#pragma once
#include <optix.h>
#include <optixu.h>
#include <optixpp_namespace.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>
class Renderer {
private:
	int windowWidth;
	int windowHeight;
	optix::Context context;
	
	std::map<std::string, optix::Buffer> buffers;

	//Given the ith and jth entry of an array, return the index of the entry in a 1D array
	static int arrayConvert(int i, int j, int width) {
		return width * j + i;
	}
public:
	Renderer(int windowWidth, int windowHeight, int rayCount, int entryPoints, int stackSize);

	void render(int entryPoint);

	/*
	Create a 2D output buffer of type T. Has the given width and height. 
	*/
	template<typename T>
	void createOutputBuffer(std::string bufferName, int width, int height) {

		//Create the buffer
		optix::Buffer buffer = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_USER, width, height);
		buffer->setElementSize(sizeof(T));
		buffer->setSize(width, height);

		//Set the buffer into the context
		context[bufferName]->setBuffer(buffer);
		buffers[bufferName] = buffer;
		
	}

	/*
	Read in the data from a output buffer, formating the data to type T, and return the data as a vector.
	*/
	template<typename T>
	std::vector<T> readOutputBuffer(std::string bufferName, int width, int height) {

		std::vector<T> result;
		const void* bufferLoc = context[bufferName]->getBuffer()->map(0, RT_BUFFER_MAP_READ);

		T* data = new T[width * height];
		std::memmove(data, bufferLoc, sizeof(T) * width * height);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int index = arrayConvert(i, j, width);
				result.push_back(data[index]);
			}
		}
		context[bufferName]->getBuffer()->unmap(); //We need to unmap all of our data each time we're finished with it.
		delete[] data; //We have to delete otherwise we will leak memory each frame. 
		return result;

		
	}

	optix::Context getContext();
	void cleanUp();
};