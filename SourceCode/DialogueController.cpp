#include "DialogueController.h"
#include "Engine/Text.h"
#include "Engine/Input.h"
/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------DialogueController Class------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------DialogueController Initialize()-----------------------------------------------------------------*/
/// <summary>
/// Called at the start of scene to initialize the text box
/// </summary>
/// <param name="textbox_path"> : Textbox sprite file path</param>
/// <param name="textbox_pos"> : Where the Textbox should be rendered. Top left coords</param>
/// <param name="textbox_size"> : Size of textbox</param>
void DialogueController::Initialize(std::wstring textbox_path, Vector2 textbox_pos, Vector2 textbox_size)
{
	text_box = std::make_shared<SPRITE>();
	text_box->Initialize(textbox_path.c_str());
	properties.position = textbox_pos;
	properties.size = textbox_size;
}

///*-------------------------------------DialogueController Insert()---------------------------------------------------------------*/
//
//void DialogueController::Insert(std::string name, std::list<std::string> dialogue)
//{
//	dialogue_map.emplace(name, dialogue);
//}

/*-------------------------------------DialogueController SetCurrentDialogue()-----------------------------------------------------------------*/
/// <summary>
/// Pushes the dialogue list to be rendered 
/// </summary>
/// <param name="dialogue"> : Dialogue list to be rendered</param>
void DialogueController::TriggerDialogue(std::list<std::string> dialogue)
{
	dialogue_list = dialogue;
	triggered = true;
}

/*-------------------------------------DialogueController Render()-----------------------------------------------------------------*/
/// <summary>
/// Called every frame to render any dialogue
/// </summary>
void DialogueController::Render()
{
	if (!triggered)
		return;
	text_box->Render(properties.position, { 1, 1 }, {}, properties.size, {}, { 1.0f, 1.0f, 1.0f, 0.7f });			// Last Vector4 parameters is RGBA setting, A is set to .5 to allow transparency

	if (dialogue_list.size() < 1)
	{
		triggered = false;
		return;
	}

	
	TextManager::Instance()->PrintOut(dialogue_list.front(), properties.position + Vector2(50.0f, 50.0f), { 0.7f, 0.7f });
	if (KEYS::Triggered(KEYS::MBS::LB))
		dialogue_list.pop_front();

}

/*---------------------------------------DialogueController Render()-------------------------------------------------*/
/// <summary>
/// Returns true if dialogue is triggered
/// </summary>
/// <returns></returns>
bool DialogueController::GetStatus()
{
	return triggered;
}
