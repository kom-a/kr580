#pragma once

#include "View.h"
#include "src/Compiler.h"

#include <imgui/imgui.h>
#include <ImGuiColorTextEdit/TextEditor.h>

class EditorView : public View
{
public:
	EditorView();
	~EditorView();

	void Render(KR580VM80A* emu);

private:
	TextEditor m_Editor;
	Compiler m_Compiler;
};
