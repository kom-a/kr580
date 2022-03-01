#include <iostream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Emulator.h>
#include "../Shared/Include/ISA.h"

#include <vector>
#include <string>


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	GLFWwindow* window = glfwCreateWindow(640, 480, "UMPK-80", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	KR580VM80A emu;

	std::vector<WORD> program = {
		MVI_C_d8, 0x08,
		LXI_H_d16, 0x00, 0x09,
		LXI_D_d16, 0x00, 0x0A,
		MOV_A_M,
		SUI_d8, 0x03,
		STAX_D,
		INX_H,
		INX_D,
		DCR_C,
		JNZ_a16, 0x08, 0x08,
		MVI_C_d8, 0x08,
		MOV_A_M,
		ADI_d8, 0x03,
		STAX_D,
		INX_H,
		INX_D,
		DCR_C,
		JNZ_a16, 0x14, 0x08,
		RST_1
	};

	emu.LoadProgram(program);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::ShowDemoWindow();
		ImGui::ShowAboutWindow();
		ImGui::ShowMetricsWindow();
		static MemoryEditor mem_edit;
		mem_edit.DrawWindow("Memory Editor", emu.Memory, MEMORY_SIZE);

		ImGui::Begin("Info");

		ImGui::LabelText(std::to_string(emu.PC).c_str(), "PC:");
		if (ImGui::Button("Step"))
		{
			emu.Step();
		}

		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}