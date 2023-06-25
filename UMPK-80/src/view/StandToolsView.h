#pragma once

#include "View.h"

#include <imgui/imgui.h>

class StandToolsView : public View
{
public:
	StandToolsView();
	~StandToolsView();

	void Render(KR580VM80A* emu);
	void Display(uint8_t* display_data);
private:
	void PortToBits(uint8_t value, int bits[8]);
	void BitsToPort(uint8_t& value, int bits[8]);
	void DrawDisplay();
	void DrawCell(float x, float y, float width, float height, uint8_t bitmap);

private:
	struct DisplayStyle
	{
		float Spacing = 20.0f;
		float Width = 36.0f;
		float Height = Width * 2;
		int ColorEnabled = IM_COL32(250, 250, 80, 255);
		int ColorDisabled = IM_COL32(50, 20, 20, 255);
		float Rounding = Width;
		float Thickness = 5.0f;
		float Offset = 0.5f;
	};

	int m_InPort5[8]; // In port 5 bits
	int m_OutPort5[8]; // Out port 5 bits
	uint8_t m_Display[6]; // Display bitmaps
	DisplayStyle m_DisplayStyle;
};
