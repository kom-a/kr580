#include "Window.h"

#include <iostream>

#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <imgui/imgui_internal.h>

#include "ViewManager.h"

#define SOURCE_CODE_FILE_FORMAT ".asm"
#define MACHINE_CODE_FILE_FORMAT ".obj"
#define FILE_FORMATS (SOURCE_CODE_FILE_FORMAT "," MACHINE_CODE_FILE_FORMAT)

void GLFWErrorCallback(int error, const char* description);
void GLFWWindowResizeCallback(GLFWwindow* window, int width, int height);

static inline bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Window::Window(int32_t width, int32_t height, const std::string& title)
	: m_Width(width),
	m_Height(height),
	m_Title(title),
	m_GLFWWindow(nullptr),
	m_FileDialog()
{
	if (!Init())
	{
		std::cerr << "Failed to init window" << std::endl;
		return;
	}
}

Window::~Window()
{
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
	glfwSwapInterval(1);
	glfwSetWindowUserPointer(m_GLFWWindow, this);
	glfwSetWindowSizeCallback(m_GLFWWindow, GLFWWindowResizeCallback);

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
	auto font = io.Fonts->AddFontFromFileTTF("res/RobotoMono-Bold.ttf", 18, 0, io.Fonts->GetGlyphRangesCyrillic());
	io.FontDefault = font;

	InitImGuiStyle();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_GLFWWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::InitImGuiStyle()
{
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.FrameRounding = 3.0f;
	style.WindowTitleAlign.x = 0.5f;
	style.WindowMenuButtonPosition = ImGuiDir_None;

	// Colors
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.20f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.31f, 0.31f, 0.39f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.39f, 0.47f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.31f, 0.31f, 0.31f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.31f, 0.31f, 0.31f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.39f, 0.39f, 0.39f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.39f, 0.39f, 0.39f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.31f, 0.31f, 0.31f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.39f, 0.39f, 0.39f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.31f, 0.31f, 0.31f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.39f, 0.39f, 0.39f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.35f, 0.35f, 0.35f, 0.70f);
}

void Window::ImGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();
}

void Window::ImGuiEndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(m_GLFWWindow);
}

void Window::RenderMainMenubar(KR580VM80A* emu)
{
	ViewManager& viewManager = ViewManager::GetInstance();

	bool load_file = false, save_source_code = false, save_machine_code = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
				EditorView* editor = viewManager.GetEditorView();
				TextEditor::ErrorMarkers no_errors;
				std::string empty_source;

				// TODO: Ask if user wants to save previous source code or something
				editor->SetErrors(no_errors);
				editor->SetText(empty_source);
				emu->Init();
			}
			if (ImGui::MenuItem("Open"))
			{
				load_file = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save as *.asm"))
			{
				save_source_code = true;
			}
			if (ImGui::MenuItem("Save as *.obj"))
			{
				save_machine_code = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
			{
				glfwSetWindowShouldClose(m_GLFWWindow, true);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			EditorView* editor = viewManager.GetEditorView();
			bool can_undo = editor->CanUndo();
			bool can_redo = editor->CanRedo();

			if (!can_undo)
				ImGui::BeginDisabled();
			if (ImGui::MenuItem("Undo", "Ctrl+Z"))
			{
				editor->Undo();
			}
			if (!can_undo)
				ImGui::EndDisabled();

			if (!can_redo)
				ImGui::BeginDisabled();
			if (ImGui::MenuItem("Redo", "Ctrl+Y"))
			{

			}
			if (!can_redo)
				ImGui::EndDisabled();

			ImGui::Separator();

			if (ImGui::MenuItem("Cut", "Ctrl+X"))
			{
				editor->Cut();
			}
			if (ImGui::MenuItem("Copy", "Ctrl+C"))
			{
				editor->Copy();
			}
			if (ImGui::MenuItem("Paste", "Ctrl+V"))
			{
				editor->Paste();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Editor"))
				viewManager.GetEditorView()->Open();
			if (ImGui::MenuItem("Memory"))
				viewManager.GetMemoryView()->Open();
			if (ImGui::MenuItem("Registers"))
				viewManager.GetRegisterView()->Open();
			if (ImGui::MenuItem("Stack"))
				viewManager.GetStackView()->Open();
			if (ImGui::MenuItem("In ports"))
				viewManager.GetInPortsView()->Open();
			if (ImGui::MenuItem("Out ports"))
				viewManager.GetOutPortsView()->Open();
			if (ImGui::MenuItem("Stand tools"))
				viewManager.GetStandToolsView()->Open();

			ImGui::Separator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Built-in"))
			{
				//Do something
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (load_file)
		ImGui::OpenPopup("Open File");
	if (save_source_code)
		ImGui::OpenPopup("Save File#sourcecode");
	if (save_machine_code)
		ImGui::OpenPopup("Save File#machinecode");

	if (m_FileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(0, 0), FILE_FORMATS))
	{
		if (m_FileDialog.ext == SOURCE_CODE_FILE_FORMAT)
		{
			EditorView* editor_view = viewManager.GetEditorView();

			if (!editor_view->LoadFromFile(m_FileDialog.selected_path))
			{
				// Log an error during source code file loading
			}
		}
		else if (m_FileDialog.ext == MACHINE_CODE_FILE_FORMAT)
		{
			MemoryView* memory_view = viewManager.GetMemoryView();

			if (!memory_view->LoadFromFile(m_FileDialog.selected_path))
			{
				// Log an error during machine code file loading
			}
		}
		else
		{
			// Incorrect file type
		}
	}

	if (m_FileDialog.showFileDialog("Save File#sourcecode", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(0, 0), SOURCE_CODE_FILE_FORMAT))
	{
		std::string save_filename = ends_with(m_FileDialog.selected_path, SOURCE_CODE_FILE_FORMAT)
			? m_FileDialog.selected_path
			: m_FileDialog.selected_path.append(SOURCE_CODE_FILE_FORMAT);

		EditorView* editor_view = viewManager.GetEditorView();

		if (!editor_view->SaveToFile(save_filename))
		{
			// Log an error during source code saving to file
		}
	}

	if (m_FileDialog.showFileDialog("Save File#machinecode", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(0, 0), MACHINE_CODE_FILE_FORMAT))
	{
		std::string save_filename = ends_with(m_FileDialog.selected_path, MACHINE_CODE_FILE_FORMAT)
			? m_FileDialog.selected_path
			: m_FileDialog.selected_path.append(MACHINE_CODE_FILE_FORMAT);

		MemoryView* memory_view = viewManager.GetMemoryView();

		if (!memory_view->SaveToFile(save_filename))
		{
			// Log an error during machine code saving to file
		}
	}
}

void Window::RenderViews(KR580VM80A* emu)
{
	ViewManager& viewManager = ViewManager::GetInstance();
	viewManager.Render(emu);
}

void Window::Update()
{
	glfwPollEvents();

	ImGuiEndFrame();

	glfwSwapBuffers(m_GLFWWindow);
}

void Window::Render(KR580VM80A* emu)
{
	ImGuiNewFrame();

	RenderMainMenubar(emu);
	RenderViews(emu);

	ImGui::ShowDemoWindow();
	ImGui::ShowMetricsWindow();
}

void GLFWErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void GLFWWindowResizeCallback(GLFWwindow* window, int width, int height)
{
	Window* user_window = (Window*)glfwGetWindowUserPointer(window);
	user_window->m_Width = width;
	user_window->m_Height = height;
}
