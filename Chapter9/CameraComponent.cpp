#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioSystem.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{

}

void CameraComponent::SetViewMatrix(const Matrix4& view) {
	// pass view matrix to renderer and audio system
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
	game->GetAudioSystem()->SetListener(view);
}