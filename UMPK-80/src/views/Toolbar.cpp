#include "Toolbar.h"

Toolbar::Toolbar()
{

}

Toolbar::~Toolbar()
{

}

void Toolbar::Render(KR580VM80A* emu)
{
	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | 
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoScrollWithMouse);

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
}
