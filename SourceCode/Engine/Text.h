#pragma once
#include <map>
#include "Sprite.h"
#include "DXMath.h"
using namespace Math;
class TextManager;
class TextTexture
{
	std::shared_ptr<SPRITE>font_texture;
	std::wstring font_path{ L"" };
	Vector2 text_size;			// Size of each individual text
	int words_per_row{ 16 };
	friend class TextManager;
	/// <summary>
	/// Called when constructed
	/// </summary>
	/// <returns></returns>
	HRESULT Initialize();

	/// <summary>
	/// Prints out the text
	/// </summary>
	/// <param name="text"> : Text to print</param>
	/// <param name="position"> : Position to print</param>
	/// <param name="scale"> : Scale of text</param>
	void PrintOut(std::string text, Vector2 position, Vector2 scale);
	/// <summary>
	/// Prints out the text
	/// </summary>
	/// <param name="text"> : Text to print</param>
	/// <param name="position"> : Position to print</param>
	/// <param name="scale"> : Scale of text</param>
	void PrintOut(std::wstring text, Vector2 position, Vector2 scale);
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="file_path"> : File path of texture</param>
	/// <param name="size"> : Size of each text</param>
	/// <param name="wpr"> : Words per row</param>
	TextTexture(std::wstring file_path, Vector2 size, int wpr = 16);

};


class TextManager : public Singleton<TextManager>
{
	friend class TextTexture;
	std::map<std::string, std::unique_ptr<TextTexture>>fonts;
	TextTexture* current_font;
	/// <summary>
	/// Inserts the TextTexture into the manager
	/// </summary>
	/// <param name="name"> : Name of text, will be mapped to the texture</param>
	/// <param name="file_path"> : File path of texture</param>
	/// <param name="size_of_text"> : Size of each individual text</param>
	/// <param name="wpr"> : Words per row</param>
	/// <returns></returns>
	HRESULT Insert(std::string name, std::wstring file_path, Vector2 size_of_text, int wpr);
public:
	/// <summary>
	/// Called at the start of the program
	/// </summary>
	/// <returns></returns>
	HRESULT Initialize();

	/// <summary>
	/// Prints out the text
	/// </summary>
	/// <param name="text"> : Text to print</param>
	/// <param name="position"> : Position to print</param>
	/// <param name="scale"> : Scale of text</param>
	void PrintOut(std::string string_of_text, Vector2 position, Vector2 scale);

	/// <summary>
	/// Prints out the text
	/// </summary>
	/// <param name="text"> : Text to print</param>
	/// <param name="position"> : Position to print</param>
	/// <param name="scale"> : Scale of text</param>
	void PrintOut(std::wstring string_of_text, Vector2 position, Vector2 scale);

	void PrintOutSequence(std::string string_of_text, Vector2 position, Vector2 scale);

	/// <summary>
	/// Sets the desired font to be used
	/// </summary>
	/// <param name="name"></param>
	void SetCurrentFont(std::string name);

};