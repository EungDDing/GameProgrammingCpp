#pragma once
#include "GL/glew.h"
#include <string>
#include "Math.h"

class Shader {
public:
	Shader();
	~Shader();

	// load the vertex/fragment shaders with the given names
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	// set this as the active shader program
	void SetActive();

	// set a matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& amtrix);
	// set a Vector3 uniform
	void SetVectorUniform(const char* name, const Vector3& vector);
	// set a SetFloatUniform
	void SetFloatUniform(const char* name, float value);
private:
	// try to compile the specified shader
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	// test whether shader compiled successfully
	bool IsCompiled(GLuint shader);
	// test whether vertex/fragment programs link
	bool IsValidProgram();
private:
	// store the shader object IDs
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};