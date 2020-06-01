#include "../headers/ProgramCreator.h"

ProgramCreator::ProgramCreator(optix::Context context) {
	this->context = context;
	programs = std::map<std::string, optix::Program>();
}

ProgramCreator::ProgramCreator() {

}

//Read a file and return the contents of the file
std::string ProgramCreator::readFile(std::string fileName) {
		std::string fileContents;
		std::ifstream file(fileName);

		std::string line;
		if (file.is_open()) {
			std::cout << "Accesed file: " << fileName << std::endl;
			while (std::getline(file, line)) {
				fileContents += line;
				fileContents += '\n';
			}
			file.close();
		}
		else {
			std::cout << "ERROR: Cannot open file! File Name: " << fileName << std::endl;
		}
		return fileContents;
}

//Create a program and store it in a map so we can use it later.
optix::Program ProgramCreator::createProgram(std::string fileName, std::string programName) {

	std::string ptxString = readFile(fileName);
	optix::Program program = context->createProgramFromPTXString(ptxString.c_str(), programName);
	programs[programName] = program;
	return program;
}

//Create a Ray Generation Program
void ProgramCreator::createRaygenProgram(std::string fileName, std::string programName, int entryPoint) {
	optix::Program program = createProgram(fileName, programName);
	context->setRayGenerationProgram(entryPoint, program);
}

//Set a program variable
void ProgramCreator::createProgramVariable1f(std::string varName, float data) {
	context[varName]->setFloat(data);
}