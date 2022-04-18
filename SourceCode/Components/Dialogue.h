#pragma once
#include "Base Classes/Component.h"

enum class NPC_Type
{
	Normal_Npc, 
	Key_Npc
};

class Dialogue_Data : public ComponentData
{
public:
	std::vector<std::list<std::string>>dialogue_list;
	NPC_Type npc_type{ NPC_Type::Normal_Npc };

	Dialogue_Data();
	template <class T>
	void serialize(T& t)
	{
		t(cereal::base_class<ComponentData>(this), dialogue_list, npc_type);
	}
};

class Dialogue_Component : public Component
{
	Dialogue_Data* data;
	bool triggered;
	void GetDialogue();
public:
	Dialogue_Component(GameObject* p, ComponentData* d);
	HRESULT Initialize() override;
	void Execute() override;
	void Render() override;
	void UI() override;
	void Trigger();

	Dialogue_Data* GetData();
};


CEREAL_REGISTER_TYPE(Dialogue_Data)