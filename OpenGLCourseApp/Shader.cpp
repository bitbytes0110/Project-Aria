#include "pch.h"
#include "Shader.h"


Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
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

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");

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
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

void Shader::ClearShader()
{
	if (shaderID)
	{
		glDeleteProgram(shaderID);
	}
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}
