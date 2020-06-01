#pragma once
#include <optix.h>
#include <optixu.h>
#include <optixpp_namespace.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
class ProgramCreator {
private:
	optix::Context context;
	std::map<std::string, optix::Program> programs;
	std::string readFile(std::string fileName);
public:
	ProgramCreator();
	ProgramCreator(optix::Context);
	optix::Program getProgram(std::string programName);
	optix::Program createProgram(std::string fileName, std::string programName);
	void createRaygenProgram(std::string fileName, std::string programName, int entryPoint);
	void createProgramVariable1f(std::string varName, float data);

};