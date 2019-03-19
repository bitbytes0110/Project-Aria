#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

const int MAX_POINT_LIGHT = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight	// Since an ambient light is a very direct value and doesn't need any interpolation it is directly passed on to the fragment shader
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float specularShininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHT];

uniform sampler2D theTexture;	//The sampler accesses our texture through a texture unit. Default texture unit is 0, so we do not have to set anything
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightbyDirection(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.colour, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.specularShininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	return ( ambientColor + diffuseColor + specularColour );
}

vec4 CalcDirectionalLight()
{
	return CalcLightbyDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLights[i].position;
		float distance  = length(direction);	//	GLSL utility function
		direction = normalize(direction);
		
		vec4 colour = CalcLightbyDirection(pointLights[i].base, direction);

		float attenuation = pointLights[i].exponent	* distance * distance	+
							pointLights[i].linear	* distance				+
							pointLights[i].constant;

		totalColour += colour / attenuation;
	}
	return totalColour;
}

void main()
{
	vec4 finalColour = CalcDirectionalLight() + CalcPointLights();
	color = texture(theTexture,TexCoord) * finalColour;
}