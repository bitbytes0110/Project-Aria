#include "pch.h"
#include "Mesh.h"


Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat * vertices, unsigned int *indices, unsigned int nVertices, unsigned int nIndices)
{
	indexCount = nIndices;


	glGenVertexArrays(1, &VAO);	// Creates memory in GPU for VAO and gives you an ID back to you in VAO
	glBindVertexArray(VAO);	// all futures operations will happen on this bound VAO

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);		//Storing Indices (IBO or EBO)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*nIndices, indices, GL_STATIC_DRAW);		// Just like VBO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*nVertices, vertices, GL_STATIC_DRAW);

	// first value is the layout location (the one we set in our shaders!),
	// Second value : the size (3 values x,y,z per vertex) at a time
	// Third value : datatype of the vertex
	// Fourth value :if we want to normalise 
	// Fifth value : stride value (say you are storing position and color data in the same vbo) we can give an offset
	// Sixth value : CAn give an offset from where you want to read (not necessary if you are using individually)
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);			// Unbinding VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// Unbinding IBO

	glBindVertexArray(0);						// Unbinding VAO

}
void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);	// Binding VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	// Binding IBO
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);	//  Draw using Elements/Indices
	//glDrawArrays(GL_TRIANGLES, 0, 3);	//  Draw using Vertex data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// UnBinding IBO
	glBindVertexArray(0);	// UnBinding VAO
}

void Mesh::ClearMesh()
{
	if (IBO)
	{
		glDeleteBuffers(1, &IBO);	//Make sure we delete it from the GPU
		IBO = 0;
	}

	if (VBO)
	{
		glDeleteBuffers(1, &VBO);	//Make sure we delete it from the GPU
		VBO = 0;
	}

	if (VAO)
	{
		glDeleteBuffers(1, &VAO);	//Make sure we delete it from the GPU
		VAO = 0;
	}

	indexCount = 0;
}
Mesh::~Mesh()
{
	ClearMesh();
}