#include "pch.h"
#include "Shader.h"


Shader::Shader()
{
	shaderID = 0;
	uniformModelID = 0;
	uniformViewID = 0;
	uniformProjectionID = 0;

	uniformDirectionalLight.colourID = 0;
	uniformDirectionalLight.aIntensityID = 0;
	uniformDirectionalLight.dIntensityID = 0;
	uniformDirectionalLight.directionID = 0;

	pointLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char * vertexLocation, const char * fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	CompileShader(vertexString.c_str(), fragmentString.c_str());
}

std::string Shader::ReadFile(const char * fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{

		cout << "Failed to read " << fileLocation << "! File doesn't exist" << endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;

}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		cout << "Error creating shader Program!\n" << endl;
		return;
	}
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		cout << "Error Linking Program : " << eLog << endl;
		return;
	}

	glValidateProgram(shaderID);	// Checking if the shader program is valid in the current context OpenGL is working in
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		cout << "Error Validating Shader Program : " << eLog << endl;
		return;
	}

	uniformModelID = glGetUniformLocation(shaderID, "model");
	uniformProjectionID = glGetUniformLocation(shaderID, "projection");
	uniformViewID = glGetUniformLocation(shaderID, "view");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformDirectionalLight.colourID = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.aIntensityID = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.dIntensityID = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.directionID = glGetUniformLocation(shaderID, "directionalLight.direction");

	uniformSpecularIntensityID = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformSpecularShininessID = glGetUniformLocation(shaderID, "material.specularShininess");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char LocBuffer[100] = { '\0' };	//	Setting all the values to null character
		
		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].base.colour", i);
		uniformPointLights[i].colourID = glGetUniformLocation(shaderID, LocBuffer);
		
		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLights[i].aIntensityID = glGetUniformLocation(shaderID, LocBuffer);
		
		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLights[i].dIntensityID = glGetUniformLocation(shaderID, LocBuffer);
		
		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].position", i);
		uniformPointLights[i].positionID = glGetUniformLocation(shaderID, LocBuffer);

		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].constant", i);
		uniformPointLights[i].aConstantID = glGetUniformLocation(shaderID, LocBuffer);

		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].linear", i);
		uniformPointLights[i].aLinearID = glGetUniformLocation(shaderID, LocBuffer);

		snprintf(LocBuffer, sizeof(LocBuffer), "pointLights[%d].exponent", i);
		uniformPointLights[i].aExponentID = glGetUniformLocation(shaderID, LocBuffer);
	}
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		string shaderTypeString = ((shaderType == 35632) ? "Fragment" : "Vertex");
		cout << "Error Compiling the " << shaderTypeString << " shader : " << endl;
		cout << eLog;
		return;
	}

	glAttachShader(theProgram, theShader);

}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjectionID;
}

GLuint Shader::GetModelLocation()
{
	return uniformModelID;
}

GLuint Shader::GetViewLocation()
{
	return uniformViewID;
}
/*		TO REMOVE ???
GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.aIntensityID;
}

GLuint Shader::GetAmbientColourLocation()
{
	return uniformDirectionalLight.colourID;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.dIntensityID;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.directionID;
}*/

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensityID;
}

GLuint Shader::GetSpecularShininessLocation()
{
	return uniformSpecularShininessID;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}


void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(	uniformDirectionalLight.colourID, uniformDirectionalLight.aIntensityID, uniformDirectionalLight.dIntensityID,
						uniformDirectionalLight.directionID);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
	{
		lightCount = MAX_POINT_LIGHTS;	//	Instructing the shader not to use more than MAX_POINT_LIGHTS!
	}
	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(	uniformPointLights[i].colourID, uniformPointLights[i].aIntensityID, uniformPointLights[i].dIntensityID, 
							uniformPointLights[i].positionID,
							uniformPointLights[i].aConstantID, uniformPointLights[i].aLinearID, uniformPointLights[i].aExponentID);
	}
}


void Shader::ClearShader()
{
	if (shaderID)
	{
		glDeleteProgram(shaderID);
	}
	shaderID = 0;
	uniformModelID = 0;
	uniformViewID = 0;
	uniformProjectionID = 0;

	uniformDirectionalLight.colourID = 0;
	uniformDirectionalLight.aIntensityID = 0;
	uniformDirectionalLight.dIntensityID = 0;
	uniformDirectionalLight.directionID = 0;

	pointLightCount = 0;
}

Shader::~Shader()
{
	ClearShader();
}