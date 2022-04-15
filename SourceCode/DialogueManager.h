#pragma once
#include <string>
#include <map>
#include <list>
#include "Engine/Singleton.h"
#include "Engine/Sprite.h"


class DialogueController : public Singleton<DialogueController>
{
	std::list<std::string>dialogue_list;
	std::shared_ptr<SPRITE>text_box;
	struct TextBoxProperties
	{
		Vector2 position;
		Vector2 size;
	}properties;

	bool triggered{};
public:
	/// <summary>
	/// Called at the start of scene to initialize the text box
	/// </summary>
	/// <param name="textbox_path"> : Textbox sprite file path</param>
	/// <param name="textbox_pos"> : Where the Textbox should be rendered. Top left coords</param>
	/// <param name="textbox_size"> : Size of textbox</param>
	void Initialize(std::wstring textbox_path, Vector2 textbox_pos, Vector2 textbox_size);
	/// <summary>
	/// Pushes the dialogue list to be rendered 
	/// </summary>
	/// <param name="dialogue"> : Dialogue list to be rendered</param>
	void TriggerDialogue(std::list<std::string> dialogue);
	/// <summary>
	/// Called every frame to render any dialogue
	/// </summary>
	void Render();
	/// <summary>
	/// Returns true if dialogue is triggered
	/// </summary>
	/// <returns></returns>
	bool GetStatus();

};