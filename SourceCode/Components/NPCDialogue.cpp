#include "NPCDialogue.h"



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
		ImGui::InputText("Dialogue Name", text_name, 256);
		ImGui::InputText("Insert Text", text, 256);
		if (ImGui::Button("Insert dialogue"))
		{
			std::string name(text_name), dialogue(text);
			data->dialogue.emplace(text_name, dialogue);
		}
		bool isEmpty{};
		if (data->dialogue.size() < 1)
			isEmpty = true;

		if (!isEmpty)
		{
			ImGui::ListBoxHeader("Dialogue list");
			static std::string selected{ data->dialogue.begin()->first };
			for (auto& t : data->dialogue)
			{
				if (ImGui::Selectable(t.first.c_str()))
				{
					selected = t.first;
					break;
				}
			}
			ImGui::ListBoxFooter();

			ImGui::Text(data->dialogue.find(selected)->second.c_str());
		}
		ImGui::TreePop();
	}
}