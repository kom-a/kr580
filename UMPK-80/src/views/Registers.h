#pragma once

#include "Emulator.h"
#include "View.h"

class Registers : public View 
{
public:
	Registers(KR580VM80A* emu);

	void Render();
};