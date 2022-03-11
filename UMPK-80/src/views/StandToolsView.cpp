#include "StandToolsView.h"

#include <string>

StandToolsView::StandToolsView()
{
	memset(m_InPort5, 0, sizeof(int) * 8);
	memset(m_OutPort5, 0, sizeof(int) * 8);
}

StandToolsView::~StandToolsView()
{

}

void StandToolsView::PortToBits(uint8_t value, int bits[8])
{
	for (int i = 0; i < 8; i++)
	{
		bits[i] = value & 0x01;
		value >>= 1;
	}
}

void StandToolsView::BitsToPort(uint8_t& value, int bits[8])
{
	uint8_t tmp = 0;
	for (int i = 0; i < 8; i++)
	{
		tmp |= (bits[i] & 0x01) << i;
	}

	value = tmp;
}

void StandToolsView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Stand tools", &m_Open);
	ImGui::Text("Out: ");

	PortToBits(emu->Out[0x05], m_OutPort5);
	for (int i = 7; i >= 0; i--)
	{
		if(i < 7)
			ImGui::SameLine();
		ImGui::PushID(i);

		if (!m_OutPort5[i])
		{
			ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(10, 20, 10, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(10, 20, 10, 255));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(100, 255, 100, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(100, 255, 100, 255));
		}

		ImGui::RadioButton("##out_bit", (bool)m_OutPort5[i]);
		ImGui::PopStyleColor(2);
		ImGui::PopID();
	}
	BitsToPort(emu->Out[0x05], m_OutPort5);

	ImGui::Separator();
	ImGui::Text("In: ");

	PortToBits(emu->In[0x05], m_InPort5);
	for (int i = 7; i >= 0; i--)
	{
		ImGui::PushID(i);
		ImGui::VSliderInt("##in_bit", ImVec2(20, 45), &m_InPort5[i], 0, 1);
		ImGui::SameLine();
		ImGui::PopID();
	}
	BitsToPort(emu->In[0x05], m_InPort5);

	ImGui::End();
}

