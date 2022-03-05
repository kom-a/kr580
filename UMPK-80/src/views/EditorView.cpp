#include "EditorView.h"


EditorView::EditorView()
{

}

EditorView::~EditorView()
{

}

void EditorView::Render(KR580VM80A* emu)
{
	ImGui::Begin("Editor");

	m_Editor.Render("Editor");

	ImGui::End();
}
