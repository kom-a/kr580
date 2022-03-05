#include "EditorView.h"


EditorView::EditorView()
	: m_Editor(),
	m_Open(true)
{

}

EditorView::~EditorView()
{

}

void EditorView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Editor", &m_Open);

	m_Editor.Render("Editor");

	ImGui::End();
}
