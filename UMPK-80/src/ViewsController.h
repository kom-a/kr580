#pragma once

#include "Emulator.h"
#include "Views.h"

class ViewsController
{
public:
	static ViewsController& GetInstance();

private:
	ViewsController();
	~ViewsController();

public:
	ViewsController(const ViewsController&) = delete;
	void operator=(const ViewsController&) = delete;

public:
	// Don't use these twice :)
	inline void SetEditorView(EditorView* view)			{ m_Views.push_back(view); m_EditorView = view; }
	inline void SetMemoryView(MemoryView* view)			{ m_Views.push_back(view); m_MemoryView = view; }
	inline void SetRegistersView(RegistersView* view)	{ m_Views.push_back(view); m_RegistersView = view; }
	inline void SetStackView(StackView* view)			{ m_Views.push_back(view); m_StackView = view; }
	inline void SetInPortView(InPortView* view)			{ m_Views.push_back(view); m_InPortView = view; }
	inline void SetOutPortView(OutPortView* view)		{ m_Views.push_back(view); m_OutPortView = view; }
	inline void SetStandToolsView(StandToolsView* view) { m_Views.push_back(view); m_StandToolsView = view; }
	inline void SetToolbarView(ToolbarView* view)		{ m_Views.push_back(view); m_ToolbarView = view; }

	inline EditorView* GetEditorView() const { return m_EditorView; }
	inline MemoryView* GetMemoryView() const { return m_MemoryView; }
	inline RegistersView* GetRegisterView() const { return m_RegistersView; }
	inline StackView* GetStackView() const { return m_StackView; }
	inline InPortView* GetInPortsView() const { return m_InPortView; }
	inline OutPortView* GetOutPortsView() const { return m_OutPortView; }
	inline StandToolsView* GetStandToolsView() const { return m_StandToolsView; }

	void Render(KR580VM80A* emu);
private: 
	EditorView* m_EditorView = nullptr;
	MemoryView* m_MemoryView = nullptr;
	RegistersView* m_RegistersView = nullptr;
	StackView* m_StackView = nullptr;
	InPortView* m_InPortView = nullptr;
	OutPortView* m_OutPortView = nullptr;
	StandToolsView* m_StandToolsView = nullptr;
	ToolbarView* m_ToolbarView = nullptr;
	std::vector<View*> m_Views;
};