#include "Dialogue.h"
#include "../DialogueController.h"
std::string npc_types[] = { "Normal Npc", "Key Npc"};


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------Dialogue_Data Class-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------Dialogue_Data Contructor------------------------------------------------------------------------*/

Dialogue_Data::Dialogue_Data()
{
	type = COMPONENT_TYPE::NPC_DIALOGUE;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------Dialogue_Component Class----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------Dialogue_Component Constructor---------------------------------------------------------------------------*/

Dialogue_Component::Dialogue_Component(GameObject* p, ComponentData* data)
{
	parent = p;
	this->data = static_cast<Dialogue_Data*>(data);

}

/*--------------------------------------------------------Dialogue_Component GetDialogue()---------------------------------------------------------------------------*/

void Dialogue_Component::GetDialogue()
{
	std::list<std::string>dialogue = data->dialogue_list.at(rand() % (data->dialogue_list.size()));
	DialogueController::Instance()->TriggerDialogue(dialogue);
}


/*--------------------------------------------------------Dialogue_Component Initialize()---------------------------------------------------------------------------*/

HRESULT Dialogue_Component::Initialize()
{
	return S_OK;
}

/*--------------------------------------------------------Dialogue_Component Execute()---------------------------------------------------------------------------*/

void Dialogue_Component::Execute()
{
	
}

/*--------------------------------------------------------Dialogue_Component Render()---------------------------------------------------------------------------*/

void Dialogue_Component::Render()
{

}

/*--------------------------------------------------------Dialogue_Component UI()---------------------------------------------------------------------------*/

void Dialogue_Component::UI()
{
	if (ImGui::TreeNode("NPCDialogue"))
	{
		static char text[256], text_name[256];

		static int cur_type{};
		if (ImGui::BeginCombo("NPC Type", npc_types[cur_type].c_str()))
		{
			int index{};
			for (auto& s : npc_types)
			{
				if (ImGui::Selectable(npc_types[index].c_str()))
					cur_type = index;
				++index;
			}
			ImGui::EndCombo();
		}
		data->npc_type = (NPC_Type)cur_type;
		if (ImGui::Button("Create Dialogue List")) 
		{
			std::string name(text_name);
			data->dialogue_list.push_back(std::list<std::string>());
		}
		static int index{};
		bool isEmpty{ data->dialogue_list.size() < 1 };
		if (!isEmpty)
		{
			std::string header{ "Dialogue " + std::to_string(index) };
			if (ImGui::BeginCombo("Dialogue List", header.c_str()))
			{
				int ind{};
				header = { "Dialogue " + std::to_string(ind) };
				for (auto& d : data->dialogue_list)
				{
					if (ImGui::Selectable(header.c_str()))
					{
						index = ind;
						break;
					}
					ind++;
				}
				ImGui::EndCombo();
			}
			for (auto& d : data->dialogue_list.at(index))
				ImGui::Text(d.c_str());



			ImGui::InputText("Text", text, 256);
			if (ImGui::Button("Insert Dialogue"))
				data->dialogue_list.at(index).push_back(std::string(text));


		}
		ImGui::TreePop();
	}
}

/*--------------------------------------------------------Dialogue_Component Trigger()---------------------------------------------------------------------------*/

void Dialogue_Component::Trigger()
{
	GetDialogue();
}

/*--------------------------------------------------------Dialogue_Component GetData()-------------------------------------------------*/

Dialogue_Data* Dialogue_Component::GetData()
{
	return data;
}

