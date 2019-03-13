#pragma once

#include<iostream>
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat * vertices, unsigned int *indices, unsigned int nVertices, unsigned int nIndices);
	void RenderMesh();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

