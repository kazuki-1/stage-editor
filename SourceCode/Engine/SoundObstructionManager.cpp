#include "SoundObstructionManager.h"
#include "Audio.h"

/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------SoundObstructionManager Class--------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------SoundObstructionManager Insert()-----------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/

void SoundObstructionManager::Insert(Mesh_Component* mesh, int mesh_index)
{
	sound_walls.push_back({ mesh, mesh_index });
}

