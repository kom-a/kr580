#include "InPortView.h"

InPortView::InPortView()
{
	m_Editor.OptShowAscii = false;
	m_Editor.OptShowOptions = false;
	m_Editor.Cols = 16;
}

InPortView::~InPortView()
{

}

void InPortView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("In ports", &m_Open);

	m_Editor.DrawContents(emu->In, KR580VM80A::PORTS_SIZE);

	ImGui::End();
}
