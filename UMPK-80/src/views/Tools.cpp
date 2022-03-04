#pragma once

#include "Tools.h"

#include <imgui/imgui.h>

Tools::Tools(KR580VM80A* emu)
	: View(emu)
{

}

void Tools::Render()
{
	ImGui::Begin("Tools");

	if (ImGui::Button("Step"))
	{
		m_Emulator->Step();
	}

	if (ImGui::Button("Run"))
	{
		m_Emulator->Running = true;
	}

	ImGui::End();
}

