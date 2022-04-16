#pragma once

#include "View.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

class ToolbarView : public View
{
public:
	ToolbarView();
	~ToolbarView();

	void Render(KR580VM80A* emu);
};