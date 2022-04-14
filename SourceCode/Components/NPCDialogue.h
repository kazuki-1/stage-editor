#pragma once
#include "Base Classes/COMPONENT.h"


class NPCDialogue_Data : public COMPONENT_DATA
{
public:
	std::map<std::string, std::string>dialogue;
	NPCDialogue_Data();
	template <class T>
	void serialize(T& t)
	{
		t(cereal::base_class<COMPONENT_DATA>(this), dialogue);
	}
};

class NPCDialogue : public COMPONENT
{
	NPCDialogue_Data* data;
	bool activated;
public:
	NPCDialogue(GAMEOBJECT* p, COMPONENT_DATA* d);
	HRESULT Initialize() override;
	void Execute() override;
	void Render() override;
	void UI() override;
};


CEREAL_REGISTER_TYPE(NPCDialogue_Data)