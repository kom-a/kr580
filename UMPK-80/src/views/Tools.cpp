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

	ImGui::End();
}

