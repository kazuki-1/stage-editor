#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
class Mesh_Component;
struct SoundWall_Parameters
{
	Mesh_Component* mesh{};
	int mesh_index{};
};

class SoundObstructionManager : public Singleton<SoundObstructionManager>
{
public:
	SoundObstructionManager() {};
	std::vector<SoundWall_Parameters>sound_walls;
	
	void Insert(Mesh_Component* mesh, int mesh_index);
	void PerformCalculation(std::vector<float>& channel_volumes);

};