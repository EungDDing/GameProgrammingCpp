#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "Math.h"

class Renderer {
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	// unload texture and mesh
	void UnloadData();
	// draw frame
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	// map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// all the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;
	// all mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;

	class Game* mGame;

	// sprite shader
	class Shader* mSpriteShader;
	// sprite vertex array
	class VertexArray* mSpriteVerts;

	// mesh shader
	class Shader* mMeshShader;

	// view/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;
	// width/height of screen
	float mScreenWidth;
	float mScreenHeight;

	// window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
};