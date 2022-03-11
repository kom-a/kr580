#include "StandToolsView.h"

#include <string>

#define BIT(value, index) ((value >> index) & 0x01)

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

void StandToolsView::DrawDisplay()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const float spacing = 10.0f;
	static float cell_width = 36.0f;
	static float cell_height = cell_width * 2;

	static int ngon_sides = 6;
	static bool circle_segments_override = false;
	static int circle_segments_override_v = 12;
	static bool curve_segments_override = false;
	static int curve_segments_override_v = 8;

	const int color_enabled = IM_COL32(200, 50, 50, 255);
	const int color_disabled = IM_COL32(20, 5, 5, 255);

	const ImDrawFlags corners_tl_br = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight;

	const int circle_segments = circle_segments_override ? circle_segments_override_v : 0;
	const int curve_segments = curve_segments_override ? curve_segments_override_v : 0;
	float x = p.x + 4.0f;
	float y = p.y + 4.0f;

	uint8_t symbols[6] = {
		0x76, 0x79, 0x38, 0x38, 0x3f, 0x0
	};

	for (int n = 0; n < 6; n++)
	{
		DrawCell(x, y, cell_width, cell_height, symbols[n]);

		x += cell_width + spacing;
	}
}

void StandToolsView::DrawCell(float x, float y, float width, float height, uint8_t bitmap)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const int color_enabled = IM_COL32(200, 50, 50, 255);
	const int color_disabled = IM_COL32(20, 5, 5, 255);
	static float rounding = 0.0f;
	static float thickness = 3.0f;
	static float offset = 4;	

	int color;
	color = BIT(bitmap, 0) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x + thickness + offset, y), ImVec2(x + width - thickness - offset, y + thickness), color, rounding);
	
	color = BIT(bitmap, 1) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x + width - thickness, y + thickness + offset), ImVec2(x + width, y + height * 0.5f - thickness * 0.5f - offset), color, rounding);

	color = BIT(bitmap, 2) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x + width - thickness, y + height * 0.5f + thickness * 0.5f + offset), ImVec2(x + width, y + height - thickness - offset), color, rounding);

	color = BIT(bitmap, 3) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x + thickness + offset, y + height - thickness), ImVec2(x + width - thickness - offset, y + height), color, rounding);

	color = BIT(bitmap, 4) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x, y + height * 0.5f + thickness * 0.5f + offset), ImVec2(x + thickness, y + height - thickness - offset), color, rounding);

	color = BIT(bitmap, 5) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x, y + thickness + offset), ImVec2(x + thickness, y + height * 0.5f - thickness * 0.5f - offset), color, rounding);

	color = BIT(bitmap, 6) ? color_enabled : color_disabled;
	draw_list->AddRectFilled(ImVec2(x + thickness + offset, y + height * 0.5f - thickness * 0.5f), ImVec2(x + width - thickness - offset, y + height * 0.5f + thickness * 0.5f), color, rounding);

}

void StandToolsView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Stand tools", &m_Open);
	ImGui::Text("Out: ");

	const int color_enabled = IM_COL32(100, 255, 100, 255);
	const int color_disabled = IM_COL32(10, 20, 10, 255);

	PortToBits(emu->Out[0x05], m_OutPort5);
	for (int i = 7; i >= 0; i--)
	{
		if(i < 7)
			ImGui::SameLine();
		ImGui::PushID(i);

		const int color = m_OutPort5[i] ? color_enabled : color_disabled;
		ImGui::PushStyleColor(ImGuiCol_CheckMark, color);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, color);

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
		if (i < 7)
			ImGui::SameLine();

		ImGui::PushID(i);
		ImGui::VSliderInt("##in_bit", ImVec2(20, 45), &m_InPort5[i], 0, 1);
		ImGui::PopID();
	}
	BitsToPort(emu->In[0x05], m_InPort5);

	DrawDisplay();

	ImGui::End();
}

