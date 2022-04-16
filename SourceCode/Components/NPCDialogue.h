#pragma once
#include "Base Classes/Component.h"

enum class NPC_Type
{
	Normal_Npc, 
	Key_Npc
};

class NPCDialogue_Data : public ComponentData
{
public:
	std::vector<std::list<std::string>>dialogue_list;
	NPC_Type npc_type{ NPC_Type::Normal_Npc };

	NPCDialogue_Data();
	template <class T>
	void serialize(T& t)
	{
		t(cereal::base_class<ComponentData>(this), dialogue_list, npc_type);
	}
};

class NPCDialogue : public Component
{
	NPCDialogue_Data* data;
	bool triggered;
	void GetDialogue();
public:
	NPCDialogue(GameObject* p, ComponentData* d);
	HRESULT Initialize() override;
	void Execute() override;
	void Render() override;
	void UI() override;
	void Trigger();

	NPCDialogue_Data* GetData();
};


CEREAL_REGISTER_TYPE(NPCDialogue_Data)