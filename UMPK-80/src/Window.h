#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Views.h"
#include <imgui/imgui.h>

class Window
{
public:
	Window(int32_t width, int32_t height, const std::string& title);
	~Window();

public:
	void Update();
	void Render(KR580VM80A* emu);

public:
	inline bool Closed() const { return glfwWindowShouldClose(m_GLFWWindow); }

	// Don't use these twice :)
	inline void SetEditorView(EditorView* view)			{ m_Views.push_back(view); m_EditorView = view; }
	inline void SetMemoryView(MemoryView* view)			{ m_Views.push_back(view); m_MemoryView = view; }
	inline void SetRegistersView(RegistersView* view)	{ m_Views.push_back(view); m_RegistersView = view; }
	inline void SetStackView(StackView* view)			{ m_Views.push_back(view); m_StackView = view; }
	inline void SetInPortView(InPortView* view)			{ m_Views.push_back(view); m_InPortView = view; }
	inline void SetOutPortView(OutPortView* view)		{ m_Views.push_back(view); m_OutPortView = view; }
	inline void SetStandToolsView(StandToolsView* view) { m_Views.push_back(view); m_StandToolsView = view; }
	inline void SetToolbarView(ToolbarView* view)		{ m_Views.push_back(view); m_Toolbar = view; }

private:
	friend void GLFWErrorCallback(int error, const char* description);

private:
	bool Init();
	void InitImGui();
	void InitImGuiStyle();

private:
	GLFWwindow* m_GLFWWindow;
	int32_t m_Width, m_Height;
	std::string m_Title;

	// Views
	EditorView* m_EditorView = nullptr;
	MemoryView* m_MemoryView = nullptr;
	RegistersView* m_RegistersView = nullptr;
	StackView* m_StackView = nullptr;
	InPortView* m_InPortView = nullptr;
	OutPortView* m_OutPortView = nullptr;
	StandToolsView* m_StandToolsView = nullptr;
	ToolbarView* m_Toolbar = nullptr;
	std::vector<View*> m_Views;
};