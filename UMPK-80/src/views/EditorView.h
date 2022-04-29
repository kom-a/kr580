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

public:
	inline std::string GetText() const { return m_Editor.GetText(); } 
	inline void SetText(const std::string& source) { m_Editor.SetText(source); }
	inline void SetErrors(TextEditor::ErrorMarkers markers) { m_Editor.SetErrorMarkers(markers); }
	inline bool CanUndo() const { return m_Editor.CanUndo(); }
	inline bool CanRedo() const { return m_Editor.CanRedo(); }
	inline void Undo() { m_Editor.Undo(); }
	inline void Redo() { m_Editor.Redo(); }
	inline void Cut() { m_Editor.Cut(); }
	inline void Copy() { m_Editor.Copy(); }
	inline void Paste() { m_Editor.Paste(); }

	void Render(KR580VM80A* emu);
	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename);

private:
	TextEditor m_Editor;
	Compiler m_Compiler;
	Disassembler m_Disassembler;
};
