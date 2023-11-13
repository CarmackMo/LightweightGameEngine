	#version 420
layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;
	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	vec2 g_padding;
};
layout( std140, binding = 2 ) uniform g_constantBuffer_drawCall
{
	mat4 g_transform_localToWorld;
};
layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec4 i_vertexColor;
layout( location = 0 ) out vec4 vertexColor;
void main()
{
	vec4 vertexPosition_world;
	{
		vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
		vertexPosition_world = g_transform_localToWorld * vertexPosition_local;
	}
	{
		vec4 vertexPosition_camera = g_transform_worldToCamera * vertexPosition_world;
		gl_Position = g_transform_cameraToProjected * vertexPosition_camera;
		vertexColor = i_vertexColor;
	}
}
