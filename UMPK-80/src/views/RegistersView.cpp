#include "RegistersView.h"

RegistersView::RegistersView()
{

}

RegistersView::~RegistersView()
{

}

void RegistersView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Registers", &m_Open);

	ImGui::Text("A: %02X", emu->A);
	ImGui::SameLine();
	ImGui::Text("F: %02X", emu->F);

	ImGui::Text("B: %02X", emu->B);
	ImGui::SameLine();
	ImGui::Text("C: %02X", emu->C);

	ImGui::Text("D: %02X", emu->D);
	ImGui::SameLine();
	ImGui::Text("E: %02X", emu->E);

	ImGui::Text("H: %02X", emu->H);
	ImGui::SameLine();
	ImGui::Text("L: %02X", emu->L);

	ImGui::Text("SP: %04X", emu->SP);
	ImGui::Text("PC: %04X", emu->PC);
	
	ImGui::End();
}
