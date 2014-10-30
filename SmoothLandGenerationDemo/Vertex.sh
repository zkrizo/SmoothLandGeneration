#version 330

uniform mat4 Camera;
uniform mat4 Projection;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 inNormal;

out vec2 texCoord;

smooth out vec3 normalVector;

void main()
{
	gl_Position = Projection*Camera*modelMatrix*vec4(inPosition,1.0);
	
	texCoord=vertexUV;
	
	vec4 normalRes = normalMatrix*vec4(inNormal,0.0);
	normalVector = normalRes.xyz;
}