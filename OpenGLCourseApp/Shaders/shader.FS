#version 330

in vec4 vCol;
in vec2 TexCoord;

out vec4 color;

struct DirectionalLight	// Since an ambient light is a very direct value and doesn't need any interpolation it is directly passed on to the fragment shader
{
	vec3 colour;
	float ambientIntensity;
};

uniform sampler2D theTexture;	//The sampler accesses our texture through a texture unit. Default texture unit is 0, so we do not have to set anything
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	color = texture(theTexture,TexCoord) * ambientColor;
}