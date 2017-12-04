#include "Interactive.h"
#include "p2Log.h"
#include "j1Fonts.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Window.h"


Interactive::Interactive()
{
}

Interactive::Interactive(SDL_Rect& pos)
{
	collider = pos;
}

Interactive::Interactive(SDL_Rect& pos, iPoint Interactiverelativepos, j1Module* callback) :  callback(callback)
{
	collider.x = pos.x + Interactiverelativepos.x;
	collider.y = pos.y + Interactiverelativepos.y;
	collider.w = pos.w;
	collider.h = pos.h;
}


Interactive::~Interactive()
{
}

bool Interactive::InteractivePreUpdate()
{
	bool ret = true;

	MoveCollider();
	SDL_Point mousePosition;

	App->input->GetMousePosition(mousePosition.x, mousePosition.y);

	if (SDL_PointInRect(&mousePosition, &collider))
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			callback->OnEvent(this, EventTypes::LEFT_MOUSE_PRESSED);
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			callback->OnEvent(this, EventTypes::LEFT_MOUSE_RELEASED);
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
			callback->OnEvent(this, EventTypes::RIGHT_MOUSE_PRESSED);
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
			callback->OnEvent(this, EventTypes::RIGHT_MOUSE_RELEASED);
		else if (!isMouseInside)
			callback->OnEvent(this, EventTypes::MOUSE_HOVER_IN);

		isMouseInside = true;
	}
	else
	{
		if (isMouseInside)
			callback->OnEvent(this, EventTypes::MOUSE_HOVER_OUT);

		isMouseInside = false;
	}

	return ret;
}

bool Interactive::InteractivePostUpdate()
{

	return true;
}

bool Interactive::InteractiveDraw()
{
	return true;
}

void Interactive::MoveCollider()
{
	if (!In_window)
	{
		collider.x = position.x + Interactiverelativepos.x;
		collider.y = position.y + Interactiverelativepos.y;
	}

	else if (In_window)
	{
		collider.x = position.x + Interactiverelativepos.x + winElement->relativePosition.x;
		collider.y = position.y + Interactiverelativepos.y + winElement->relativePosition.y;
	}
}


