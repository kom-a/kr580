#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "views/View.h"
#include <imgui/imgui.h>

class Window
{
public:
	Window(int32_t width, int32_t height, const std::string& title);
	~Window();

public:
	void Update();
	void Render(KR580VM80A* emu);
	void Add(View* view);

public:
	inline bool Closed() const { return glfwWindowShouldClose(m_GLFWWindow); }

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

	std::vector<View*> m_Views;
};