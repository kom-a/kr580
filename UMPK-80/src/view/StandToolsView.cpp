#include "StandToolsView.h"

#include <string>

#define BIT(value, index) ((value >> index) & 0x01)

StandToolsView::StandToolsView()
{
	memset(m_InPort5, 0, sizeof(int) * 8);
	memset(m_OutPort5, 0, sizeof(int) * 8);
	memset(m_Display, 0, sizeof(uint8_t) * 6);

	m_Display[0] = 0x76;
	m_Display[1] = 0x77;
	m_Display[2] = 0x66;
	m_Display[3] = 0x77;
	m_Display[4] = 0x54;
	m_Display[5] = 0x5c;
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
	float x = p.x + 4.0f;
	float y = p.y + 4.0f;

	for (int n = 0; n < 6; n++)
	{
		DrawCell(x, y, m_DisplayStyle.Width, m_DisplayStyle.Height, m_Display[n]);

		x += m_DisplayStyle.Width + m_DisplayStyle.Spacing;
		if(n == 3)
			x += m_DisplayStyle.Spacing;
	}

	ImVec2& cursor = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + m_DisplayStyle.Height + m_DisplayStyle.Spacing));
}

void StandToolsView::DrawCell(float x, float y, float width, float height, uint8_t bitmap)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();
	int color;
	const float& thickness = m_DisplayStyle.Thickness;
	const float& offset = m_DisplayStyle.Offset;
	const float& rounding = m_DisplayStyle.Rounding;

	color = BIT(bitmap, 0) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x + thickness + offset, y), ImVec2(x + width - thickness - offset, y + thickness), color, rounding);
	
	color = BIT(bitmap, 1) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x + width - thickness, y + thickness + offset), ImVec2(x + width, y + height * 0.5f - thickness * 0.5f - offset), color, rounding);

	color = BIT(bitmap, 2) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x + width - thickness, y + height * 0.5f + thickness * 0.5f + offset), ImVec2(x + width, y + height - thickness - offset), color, rounding);

	color = BIT(bitmap, 3) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x + thickness + offset, y + height - thickness), ImVec2(x + width - thickness - offset, y + height), color, rounding);

	color = BIT(bitmap, 4) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x, y + height * 0.5f + thickness * 0.5f + offset), ImVec2(x + thickness, y + height - thickness - offset), color, rounding);

	color = BIT(bitmap, 5) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x, y + thickness + offset), ImVec2(x + thickness, y + height * 0.5f - thickness * 0.5f - offset), color, rounding);

	color = BIT(bitmap, 6) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	draw_list->AddRectFilled(ImVec2(x + thickness + offset, y + height * 0.5f - thickness * 0.5f), ImVec2(x + width - thickness - offset, y + height * 0.5f + thickness * 0.5f), color, rounding);

	color = BIT(bitmap, 7) ? m_DisplayStyle.ColorEnabled : m_DisplayStyle.ColorDisabled;
	ImVec2 points[4] = {
		ImVec2(x + width + thickness, y + height - thickness),
		ImVec2(x + width + thickness * 2, y + height - thickness),
		ImVec2(x + width + thickness, y + height + thickness),
		ImVec2(x + width, y + height + thickness),
	};
	draw_list->AddConvexPolyFilled(points, 4, color);
}

void StandToolsView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Stand tools", &m_Open);

	DrawDisplay();

	ImGui::Separator();
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

	ImGui::Separator();

	//const int rows = 5;
	//const int columns = 7;
	//if (ImGui::BeginTable("keyboardTable", 7))
	//{
	//	for (int row = 0; row < rows; row++)
	//	{
	//		ImGui::TableNextRow();
	//		for (int column = 0; column < columns; column++)
	//		{
	//			ImGui::TableSetColumnIndex(column);
	//			ImGui::Button("BUtton");
	//		}
	//	}
	//	ImGui::EndTable();
	//}

	ImGui::End();
}

void StandToolsView::Display(uint8_t* display_data)
{
	for (int i = 0; i < 6; i++)
		m_Display[i] = display_data[i];
}

