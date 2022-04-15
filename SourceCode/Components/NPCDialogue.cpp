#include "NPCDialogue.h"
#include "../DialogueManager.h"
std::string npc_types[] = { "Normal Npc", "Key Npc"};


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------NPCDialogue_Data Class-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------NPCDialogue_Data Contructor------------------------------------------------------------------------*/

NPCDialogue_Data::NPCDialogue_Data()
{
	type = COMPONENT_TYPE::NPC_DIALOGUE;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------NPCDialogue Class----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------NPCDialogue Constructor---------------------------------------------------------------------------*/

NPCDialogue::NPCDialogue(GAMEOBJECT* p, COMPONENT_DATA* data)
{
	parent = p;
	this->data = static_cast<NPCDialogue_Data*>(data);

}

/*--------------------------------------------------------NPCDialogue GetDialogue()---------------------------------------------------------------------------*/

void NPCDialogue::GetDialogue()
{
	std::list<std::string>dialogue = data->dialogue_list.at(rand() % (data->dialogue_list.size()));
	DialogueController::Instance()->TriggerDialogue(dialogue);
}


/*--------------------------------------------------------NPCDialogue Initialize()---------------------------------------------------------------------------*/

HRESULT NPCDialogue::Initialize()
{
	return S_OK;
}

/*--------------------------------------------------------NPCDialogue Execute()---------------------------------------------------------------------------*/

void NPCDialogue::Execute()
{
	
}

/*--------------------------------------------------------NPCDialogue Render()---------------------------------------------------------------------------*/

void NPCDialogue::Render()
{

}

/*--------------------------------------------------------NPCDialogue UI()---------------------------------------------------------------------------*/

void NPCDialogue::UI()
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

/*--------------------------------------------------------NPCDialogue Trigger()---------------------------------------------------------------------------*/

void NPCDialogue::Trigger()
{
	GetDialogue();
}

/*--------------------------------------------------------NPCDialogue GetData()-------------------------------------------------*/

NPCDialogue_Data* NPCDialogue::GetData()
{
	return data;
}

