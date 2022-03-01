#pragma once

#include "Emulator.h"
#include "View.h"

class Tools : public View
{
public:
	Tools(KR580VM80A* emu);

	void Render();
};