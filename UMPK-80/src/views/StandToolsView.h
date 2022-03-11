#pragma once

#include "View.h"

#include <imgui/imgui.h>

class StandToolsView : public View
{
public:
	StandToolsView();
	~StandToolsView();

	void Render(KR580VM80A* emu);

private:
	void PortToBits(uint8_t value, int bits[8]);
	void BitsToPort(uint8_t& value, int bits[8]);
	void DrawDisplay();
	void DrawCell(float x, float y, float width, float height, uint8_t bitmap);

private:
	int m_InPort5[8]; // In port 5 bits
	int m_OutPort5[8]; // Out port 5 bits
};
