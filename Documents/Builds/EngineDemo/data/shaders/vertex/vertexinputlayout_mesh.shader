	#version 420
layout( location = 0 ) in vec3 i_position;
void main()
{
	gl_Position = vec4( i_position, 1.0 );
}
