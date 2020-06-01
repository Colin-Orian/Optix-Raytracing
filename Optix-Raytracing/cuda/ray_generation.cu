#include <optix.h>
#include <optixu/optixu_math_namespace.h>


rtBuffer<optix::float3, 2> result_buffer;

rtDeclareVariable(uint2, theLaunchIndex, rtLaunchIndex, );
rtDeclareVariable(float, redValue, ,);
RT_PROGRAM void rayGeneration() {
	optix::float3 red = make_float3(0.0f);
	red.x = redValue;
	result_buffer[theLaunchIndex] = red;
}