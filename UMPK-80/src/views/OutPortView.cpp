#include "OutPortView.h"

OutPortView::OutPortView()
{
	m_Editor.OptShowAscii = false;
	m_Editor.OptShowOptions = false;
	m_Editor.Cols = 16;
}

OutPortView::~OutPortView()
{

}

void OutPortView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Out port", &m_Open);

	m_Editor.DrawContents(emu->Out, PORTS_SIZE);

	ImGui::End();
}
