#include "Text.h"

/*---------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------TextTexture Class--------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------TextTexture Constructor--------------------------------------*/
/// <summary>
/// Constructor
/// </summary>
/// <param name="file_path"> : File path of texture</param>
/// <param name="size"> : Size of each text</param>
/// <param name="wpr"> : Words per row</param>
TextTexture::TextTexture(std::wstring file_path, Vector2 size, int wpr) : font_path(file_path), text_size(size), words_per_row(wpr)
{
	
}

/*--------------------------------------------------TextTexture Initialize()--------------------------------------*/
/// <summary>
/// Called when constructed
/// </summary>
/// <returns></returns>
HRESULT TextTexture::Initialize()
{
	// Create the font texture from the file path
	font_texture = std::make_shared<SPRITE>(font_path.c_str());
	return font_texture ? S_OK : E_FAIL;
}

/*--------------------------------------------------TextTexture PrintOut()--------------------------------------*/
/// <summary>
/// Prints out the text
/// </summary>
/// <param name="text"> : Text to print</param>
/// <param name="position"> : Position to print</param>
/// <param name="scale"> : Scale of text</param>
void TextTexture::PrintOut(std::string text, Vector2 position, Vector2 scale)
{
	int x{}, y{};		// For offsetting
	int index{};
	for (auto& character : text)
	{
		x = (int)character % words_per_row;
		y = (int)character / words_per_row;
		Vector2 texture_position{ text_size.x * x, text_size.y * y };
		Vector2 nextPosition = {position.x + (text_size.x * scale.x * 0.5f) * index, position.y};
		font_texture->Render(nextPosition, scale, texture_position, text_size);
		++index;
	}
}

/// <summary>
/// Prints out the text
/// </summary>
/// <param name="text"> : Text to print</param>
/// <param name="position"> : Position to print</param>
/// <param name="scale"> : Scale of text</param>
void TextTexture::PrintOut(std::wstring text, Vector2 position, Vector2 scale)
{
	int x{}, y{};		// For offsetting
	for (auto& character : text)
	{
		x = (int)character % words_per_row;
		y = (int)character / words_per_row;
		Vector2 texture_position{ text_size.x * x, text_size.y * y };
		Vector2 nextPosition = { position.x + text_size.x * scale.x * 0.5f, position.y };
		font_texture->Render(nextPosition, scale, texture_position, text_size);
	}
}



/*---------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------TextManager Class--------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------TextManager Insert()-----------------------------------------*/
/// <summary>
/// Inserts the TextTexture into the manager
/// </summary>
/// <param name="name"> : Name of text, will be mapped to the texture</param>
/// <param name="file_path"> : File path of texture</param>
/// <param name="size_of_text"> : Size of each individual text</param>
/// <param name="wpr"> : Words per row</param>
/// <returns></returns>
HRESULT TextManager::Insert(std::string name, std::wstring file_path, Vector2 size_of_text, int wpr)
{
	fonts.emplace(name, std::make_unique<TextTexture>(file_path, size_of_text, wpr));
	return fonts.find(name)->second->Initialize();
}

/*--------------------------------------------------TextManager Initialize()-----------------------------------------*/

HRESULT TextManager::Initialize()
{
	// Insert font textures into the map
	Insert("Font0", L"./Data/Font/font0.png", { 16, 16 }, 16);
	Insert("Font1", L"./Data/Font/font1.png", { 32, 32 }, 16);
	return S_OK;
}

/*--------------------------------------------------TextManager PrintOut()-----------------------------------------*/
/// <summary>
/// Prints out the text
/// </summary>
/// <param name="text"> : Text to print</param>
/// <param name="position"> : Position to print</param>
/// <param name="scale"> : Scale of text</param>
void TextManager::PrintOut(std::string string_of_text, Vector2 position, Vector2 scale)
{
	current_font->PrintOut(string_of_text, position, scale);
}

/// <summary>
/// Prints out the text
/// </summary>
/// <param name="text"> : Text to print</param>
/// <param name="position"> : Position to print</param>
/// <param name="scale"> : Scale of text</param>
void TextManager::PrintOut(std::wstring string_of_text, Vector2 position, Vector2 scale)
{
	current_font->PrintOut(string_of_text, position, scale);
}
void TextManager::PrintOutSequence(std::string string_of_text, Vector2 position, Vector2 scale)
{
	static int cur_index = 0;
	std::string cur_text;
	for (int a = 0; a < cur_index; ++a)
		cur_text.push_back(string_of_text[a]);

	current_font->PrintOut(cur_text, position, scale);

	if(cur_index < string_of_text.size())
	++cur_index;


}

/*--------------------------------------------------TextManager SetCurrentFont()-----------------------------------------*/
/// <summary>
/// Sets the desired font to be used
/// </summary>
/// <param name="name"></param>
void TextManager::SetCurrentFont(std::string name)
{
	current_font = fonts.find(name)->second.get();
}

