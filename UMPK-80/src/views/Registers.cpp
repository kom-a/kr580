#include "Registers.h"

#include <string>

#include <imgui/imgui.h>

Registers::Registers(KR580VM80A* emu)
	: View(emu)
{
}

void Registers::Render()
{
	ImGui::Begin("Registers");

	ImGui::LabelText(std::to_string(m_Emulator->A).c_str(), "A");
	ImGui::LabelText(std::to_string(m_Emulator->F).c_str(), "F");
										 
	ImGui::LabelText(std::to_string(m_Emulator->B).c_str(), "B");
	ImGui::LabelText(std::to_string(m_Emulator->C).c_str(), "C");
										 
	ImGui::LabelText(std::to_string(m_Emulator->D).c_str(), "D");
	ImGui::LabelText(std::to_string(m_Emulator->E).c_str(), "E");
										 
	ImGui::LabelText(std::to_string(m_Emulator->H).c_str(), "H");
	ImGui::LabelText(std::to_string(m_Emulator->L).c_str(), "L");

	ImGui::LabelText(std::to_string(m_Emulator->PC).c_str(), "PC");
	ImGui::LabelText(std::to_string(m_Emulator->SP).c_str(), "SP");

	ImGui::End();
}

