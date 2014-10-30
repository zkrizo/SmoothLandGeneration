#version 330

in vec2 texCoord;
smooth in vec3 normalVector;
out vec4 outputColor;

uniform sampler2D textureSampler;

struct DirectionalLight
{
	vec3 lightColor;
	vec3 lightDirection;
	float lightIntensity;
};

uniform DirectionalLight sun;

void main()
{
	vec4 textureColor =  texture(textureSampler, texCoord);
	float diffuse  = max(0.0, dot(normalize(normalVector), -sun.lightDirection));
	outputColor= textureColor*vec4(sun.lightColor*(sun.lightIntensity+diffuse),1.0);
}