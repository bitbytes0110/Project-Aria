#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight	// Since an ambient light is a very direct value and doesn't need any interpolation it is directly passed on to the fragment shader
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float specularShininess;
};

uniform sampler2D theTexture;	//The sampler accesses our texture through a texture unit. Default texture unit is 0, so we do not have to set anything
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePosition;

void main()
{
	vec4 ambientColor = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.specularShininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	color = texture(theTexture,TexCoord) * ( ambientColor + diffuseColor + specularColour );
}