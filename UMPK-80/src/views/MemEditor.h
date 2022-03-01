#pragma once

#include <cinttypes>

#include "View.h"

class MemEditor : public View
{
public:
	MemEditor(KR580VM80A* emu);

	void Render();
};