#pragma once

#include "Emulator.h"

class View
{
public:
	View(KR580VM80A* emulator);

	virtual void Render() = 0;

protected:
	KR580VM80A* m_Emulator;
};
