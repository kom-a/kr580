#include "Window.h"

#include <iostream>

#include <imgui/imgui.h>
#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

Window::Window(int32_t width, int32_t height, const std::string& title)
	: m_Width(width), 
	m_Height(height), 
	m_Title(title), 
	m_GLFWWindow(nullptr)
{
	if (!Init())
	{
		std::cerr << "Failed to init window" << std::endl;
		return;
	}
}

Window::~Window()
{
	for (int i = 0; i < m_Views.size(); i++)
		delete m_Views[i];

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_GLFWWindow);
	glfwTerminate();
}

bool Window::Init()
{
	glfwSetErrorCallback(GLFWErrorCallback);
	if (!glfwInit())
		return 1;

	m_GLFWWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
	if (m_GLFWWindow == NULL)
		return false;
	glfwMakeContextCurrent(m_GLFWWindow);
	glfwSwapInterval(0);

	InitImGui();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

void Window::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_GLFWWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::Update()
{
	glfwPollEvents();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(m_GLFWWindow);

	glfwSwapBuffers(m_GLFWWindow);
}

void Window::Render(KR580VM80A* emu)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
				//Do something
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Editor"))
			{
				//Do something
			}
			if (ImGui::MenuItem("Memory"))
			{
				//Do something
			}
			if (ImGui::MenuItem("Stack"))
			{
				//Do something
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::ShowDemoWindow();
	ImGui::ShowMetricsWindow();

	for (View* view : m_Views)
		view->Render(emu);
}

void Window::Add(View* view)
{
	m_Views.push_back(view);
}

void GLFWErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}