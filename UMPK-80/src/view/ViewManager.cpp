#include "ViewManager.h"

ViewManager& ViewManager::GetInstance()
{
	static ViewManager instance;
	return instance;
}

ViewManager::ViewManager()
{

}

ViewManager::~ViewManager()
{
	delete m_EditorView;
	delete m_MemoryView;
	delete m_RegistersView;
	delete m_StackView;
	delete m_InPortView;
	delete m_OutPortView;
	delete m_StandToolsView;
	delete m_ToolbarView;
}

void ViewManager::Render(KR580VM80A* emu)
{
	if (m_EditorView)
		m_EditorView->Render(emu);
	if (m_MemoryView)
		m_MemoryView->Render(emu);
	if(m_RegistersView)
		m_RegistersView->Render(emu);
	if(m_StackView)
		m_StackView->Render(emu);
	if(m_InPortView)
		m_InPortView->Render(emu);
	if(m_OutPortView)
		m_OutPortView->Render(emu);
	if(m_StandToolsView)
		m_StandToolsView->Render(emu);
	if(m_ToolbarView)
		m_ToolbarView->Render(emu);
}
