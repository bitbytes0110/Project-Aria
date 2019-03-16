#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;

out vec4 color;

struct DirectionalLight	// Since an ambient light is a very direct value and doesn't need any interpolation it is directly passed on to the fragment shader
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;	//The sampler accesses our texture through a texture unit. Default texture unit is 0, so we do not have to set anything
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	float diffuseFactor= max(dot(normalize(Normal),normalize(directionalLight.direction)), 0.0f);
	
	vec4 diffuseColor = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	color = texture(theTexture,TexCoord) *( ambientColor + diffuseColor );
}