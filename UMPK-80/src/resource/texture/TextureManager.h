#pragma once

#include "Texture2D.h"

class TextureManager
{
public:
	static TextureManager& GetInstance();

private:
	TextureManager();
	~TextureManager();

public:
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;

public:
	static void Init();

public:
	static Texture2D* Empty;
	static Texture2D* Assemble;
	static Texture2D* Disassemble;
};