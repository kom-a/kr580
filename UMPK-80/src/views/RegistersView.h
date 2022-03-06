#pragma once

#include "View.h"

#include <imgui/imgui.h>

class RegistersView : public View
{
public:
	RegistersView();
	~RegistersView();

	void Render(KR580VM80A* emu);
};