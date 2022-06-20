#pragma once
#include "Base Classes/COMPONENT.h"
#include "../Engine/DEBUG_PRIMITIVE.h"

class SoundObstructor_Data : public ComponentData
{
public:
	SoundObstructor_Data() { type = COMPONENT_TYPE::SOUND_OBSTRUCTOR; }
	Vector3 size{ 1, 1, 0 };		// Only the X and Y axis are used
	float obstruction_rate{ 0.5f };

	template <class T>
	void serialize(T& t)
	{
		t(cereal::base_class<ComponentData>(this), size, obstruction_rate);
	}
};


class SoundObstructor_Component : public Component
{
	SoundObstructor_Data* data{};
	std::shared_ptr<Dynamic_Plane> plane;
public:
	SoundObstructor_Component() {};
	SoundObstructor_Component(GameObject* parent, ComponentData* d);

	HRESULT Initialize() override;
	void Execute() override;
	void Render() override;
	void UI() override;

	COMPONENT_TYPE GetComponentType() override;

};


CEREAL_REGISTER_TYPE(SoundObstructor_Data);