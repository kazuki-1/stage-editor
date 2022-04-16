#include "Scene.h"
#include "../GAMEOBJECT.h"
#include "../Engine/COLLISION.h"
#include "../Audio/AudioController.h"

void Scene::Finalize()
{
	GAMEOBJECTS->Finalize();
	COLLIDERS::RAYCAST_MANAGER::Instance()->Finalize();
	AudioController::Instance()->Finalize();
}