#include "pch.h"
#include "Shader.h"


Shader::Shader()
{
	shaderID = 0;
	uniformModelID = 0;
	uniformViewID = 0;
	uniformProjectionID = 0;
	uniformAmbientIntensityID = 0;
	uniformAmbientColourID = 0;
	uniformDiffuseIntensityID = 0;
	uniformDirectionID = 0;
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

	uniformAmbientColourID = glGetUniformLocation(shaderID, "directionalLight.colour");
	uniformAmbientIntensityID = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");

	uniformDiffuseIntensityID = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
	uniformDirectionID = glGetUniformLocation(shaderID, "directionalLight.direction");
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
		cout << "Error Compiling the " << shaderType << " shader : " << eLog << endl;
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

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformAmbientIntensityID;
}

GLuint Shader::GetAmbientColourLocation()
{
	return uniformAmbientColourID;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDiffuseIntensityID;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionID;
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
}

Shader::~Shader()
{
	ClearShader();
}
