#pragma once
#include "Base Classes/COMPONENT.h"


class SoundObstruction_Data : public ComponentData
{
public:
	SoundObstruction_Data() { type = COMPONENT_TYPE::SOUND_OBSTRUCTION; }
	int mesh_index{-1};
	float obstruction_rate{ 0.5f };
	
	template <class T>
	void serialize(T& t)
	{
		t(mesh_index, obstruction_rate);
	}
};


class SoundObstruction_Component : public Component
{
	SoundObstruction_Data* data{};
public:
	SoundObstruction_Component() {};
	SoundObstruction_Component(GameObject* parent, ComponentData* d);

	HRESULT Initialize() override;
	void Execute() override;
	void UI() override;

	COMPONENT_TYPE GetComponentType() override;

};


CEREAL_REGISTER_TYPE(SoundObstruction_Component);