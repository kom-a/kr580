#include "ToolbarView.h"

ToolbarView::ToolbarView()
{

}

ToolbarView::~ToolbarView()
{

}

void ToolbarView::Render(KR580VM80A* emu)
{
#if 1
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar 
											| ImGuiDockNodeFlags_NoDockingSplitMe 
											| ImGuiDockNodeFlags_NoDockingOverMe;
	ImGui::SetNextWindowClass(&window_class);

	ImGui::Begin("##toolbar");

	if (ImGui::Button("Load"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Step"))
	{
		emu->Step();
	}
	ImGui::SameLine();
	if (ImGui::Button("Run"))
	{
		emu->Running = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		emu->Running = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		emu->PC = USER_MEMORY_OFFSET;
		emu->SP = STACK_OFFSET + STACK_SIZE;
	}

	ImGui::End();
#else
	ImGui::Begin("Test window");

	if (ImGui::Button("Test"))
	{
		// Do something
	}

	ImGui::End();
#endif

}
