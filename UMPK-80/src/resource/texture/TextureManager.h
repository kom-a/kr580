#pragma once

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
	
	
private:

	EditorView* m_EditorView = nullptr;
	MemoryView* m_MemoryView = nullptr;
	RegistersView* m_RegistersView = nullptr;
	StackView* m_StackView = nullptr;
	InPortView* m_InPortView = nullptr;
	OutPortView* m_OutPortView = nullptr;
	StandToolsView* m_StandToolsView = nullptr;
	ToolbarView* m_ToolbarView = nullptr;
};