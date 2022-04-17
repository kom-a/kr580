#include "ViewsController.h"

ViewsController& ViewsController::GetInstance()
{
	static ViewsController instance;
	return instance;
}

ViewsController::ViewsController()
	: m_Views()
{

}

ViewsController::~ViewsController()
{
	for (View* view : m_Views)
		delete view;
}

void ViewsController::Render(KR580VM80A* emu)
{
	for (auto& view : m_Views)
		if (view)
			view->Render(emu);
}
