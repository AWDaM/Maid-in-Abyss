#include "Window.h"

#include "j1App.h"
#include "j1Input.h"


Window::Window()
{
}

Window::Window(SDL_Rect & collider) : collider(collider)
{
	iPoint mouseLastFrame;
	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
}


Window::~Window()
{
}

bool Window::PreUpdate()
{
	SDL_Point mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && SDL_PointInRect(&mousePos,&collider))
	{
		iPoint mouseCurrentpos;
		App->input->GetMousePosition(mouseCurrentpos.x, mouseCurrentpos.y);
		if (mouseCurrentpos != mouseLastFrame)
		{
			iPoint difference = mouseLastFrame - mouseCurrentpos;
			moveElements(difference);
		}
	}

	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
	return true;
}


WinElement*  Window::AddElementToWindow(UIElement* element, iPoint relativePosition)
{
	element->In_window = true;
	
	WinElement* to_add = new WinElement(element, relativePosition);

	element->winElement = to_add;
	children_list.add(to_add);

	return to_add;
}

void Window::moveElements(iPoint difference)
{
	collider.x -= difference.x;
	collider.y -= difference.y;

	for (p2List_item<WinElement*>* item = children_list.start; item; item = item->next)
	{
		item->data->element->position.x -= difference.x;
		item->data->element->position.y -= difference.y;
	}
}

WinElement::WinElement(UIElement* element, iPoint relativePosition) : element(element), relativePosition(relativePosition)
{
}
