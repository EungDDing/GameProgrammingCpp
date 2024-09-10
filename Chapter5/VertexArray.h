#pragma once

class VertexArray {
public:
	VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// activate this vertex array
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	unsigned int mNumVerts;		// how many vertices in the vertex buffer
	unsigned int mNumIndices;	// how many indices in the index buffer
	unsigned int mVertexBuffer;	// OpenGL ID of vertex buffer
	unsigned int mIndexBuffer;	// OpenGL ID of the index buffer
	unsigned int mVertexArray;	// OpenGL ID of the vertex array object
};