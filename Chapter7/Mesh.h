#pragma once
#include <vector>
#include <string>

class Mesh {
public:
	Mesh();
	~Mesh();
	// load/unload mesh
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// get the vertex array associated with this mesh
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// get the texture from specified index
	class Texture* GetTexture(size_t index);
	// get name of shader
	const std::string& GetShaderName() const { return mShaderName; }
	// get object space bounding sphere radius
	float GetRadius() const { return mRadius; }
	// Get specular power of mesh
	float GetSpecPower() const { return mSpecPower; }
private:
	// textures associated with this mesh
	std::vector<class Texture*> mTextures;
	// vertex array associated with this mesh
	class VertexArray* mVertexArray;
	// name of shader specified by mesh
	std::string mShaderName;
	// stores object space bounding sphere radius
	float mRadius;
	// Specular power of surface
	float mSpecPower;
};