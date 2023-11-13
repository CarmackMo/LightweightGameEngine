	#version 420
layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;
	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	vec2 g_padding;
};
layout( location = 0 ) in vec4 vertexColor;
out vec4 o_color;
void main()
{
	o_color = vec4(
		1.0, 1.0, 1.0,
		1.0 );
	float x = abs(sin(g_elapsedSecondCount_simulationTime));
	float y = abs(tan(g_elapsedSecondCount_simulationTime));
	float z = abs(cos(g_elapsedSecondCount_simulationTime));
	o_color.r = x;
	o_color.g = y;
	o_color.b = z;
	o_color = o_color * vertexColor;
}
