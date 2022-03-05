#pragma once

#include "View.h"

#include <imgui/imgui.h>

class Toolbar : public View
{
public:
	Toolbar();
	~Toolbar();

	void Render(KR580VM80A* emu);
};