#pragma once

#include "View.h"
#include "src/Compiler.h"
#include "src/Disassembler/Disassembler.h"

#include <imgui/imgui.h>
#include <ImGuiColorTextEdit/TextEditor.h>

class EditorView : public View
{
public:
	EditorView();
	~EditorView();

	void Render(KR580VM80A* emu);

public:
	inline std::string GetText() const { return m_Editor.GetText(); }
	inline void SetErrors(TextEditor::ErrorMarkers markers) { m_Editor.SetErrorMarkers(markers); }

private:
	TextEditor m_Editor;
	Compiler m_Compiler;
	Disassembler m_Disassembler;
};
