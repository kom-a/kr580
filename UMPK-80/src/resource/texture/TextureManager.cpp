#include "TextureManager.h"

Texture2D* TextureManager::Empty;
Texture2D* TextureManager::Assemble;
Texture2D* TextureManager::Disassemble;

TextureManager& TextureManager::GetInstance()
{
	static TextureManager instance;
	return instance;
}

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	delete Empty;
	delete Assemble;
	delete Disassemble;
}

void TextureManager::Init()
{
	uint8_t* emptyTextureData = new uint8_t[4 * 4 * 4];
	memset(emptyTextureData, 0xFF, 4 * 4 * 4);

	Empty = new Texture2D(4, 4, 4, emptyTextureData);
	Assemble = new Texture2D("res/Icons/icons8-code-64.png");
	Disassemble = new Texture2D("res/Icons/Disassemble.png");

	delete[] emptyTextureData;
}