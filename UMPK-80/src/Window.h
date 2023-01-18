#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <ImGuiFileBrowser.h>

#include "Emulator.h"

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

private:
	friend void GLFWErrorCallback(int error, const char* description);
	friend void GLFWWindowResizeCallback(GLFWwindow* window, int width, int height);

private:
	bool Init();
	void InitImGui();
	void InitImGuiStyle();
	void ImGuiNewFrame();
	void ImGuiEndFrame();

	void RenderMainMenubar(KR580VM80A* emu);
	void RenderViews(KR580VM80A* emu);

private:
	GLFWwindow* m_GLFWWindow;
	int32_t m_Width, m_Height;
	std::string m_Title;

	imgui_addons::ImGuiFileBrowser m_FileDialog;
};