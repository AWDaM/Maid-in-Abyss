#include "Window.h"

#include "j1App.h"
#include "p2log.h"
#include "j1Input.h"


Window::Window()
{
}

Window::Window(SDL_Rect & collider, bool draggable) : collider(collider), draggable(draggable)
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

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		HandleFocus();


	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (SDL_PointInRect(&mousePos, &collider))
		{
			if (App->gui->BecomeFocus(this))
				hasFocus = true;
		}
		else hasFocus = false;
	}
	if (hasFocus)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && draggable)
			HandleMovement();
	}
	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
	return true;
}


WinElement*  Window::AddElementToWindow(UIElement* element, iPoint relativePosition)
{
	element->window = this;
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
		item->data->element->MoveElement(difference.Negate());
		difference.Negate();
	}
}

void Window::HandleMovement()
{
	iPoint mouseCurrentpos;
	App->input->GetMousePosition(mouseCurrentpos.x, mouseCurrentpos.y);
	if (mouseCurrentpos != mouseLastFrame)
	{
		iPoint difference = mouseLastFrame - mouseCurrentpos;
		moveElements(difference);
	}
}

void Window::HandleFocus()
{
	bool focus = false;

	for (p2List_item<WinElement*>* item = children_list.start; item; item = item->next)
	{
		if (item->data->element->hasFocus)
		{
			focus = true;
			break;
		}
	}
	{
		if (!focus)			
		{
			FocusOnFirstInteractiveElement();
		}
		else
		{
			for (p2List_item<WinElement*>* item = children_list.start; item; item = item->next)
			{
				if (item->data->element->hasFocus && (item->data->element->UItype == UIType::INTERACTIVE || item->data->element->UItype == UIType::INTERACTIVE || item->data->element->UItype == UIType::INTERACTIVE_LABEL || item->data->element->UItype == UIType::INTERACTIVE_IMAGE || item->data->element->UItype == UIType::INTERACTIVE_LABELLED_IMAGE))
				{
					item->data->element->hasFocus = false;
					if(item->next)
						item->next->data->element->hasFocus = true;
					else 
					{ 
						FocusOnFirstInteractiveElement();
					}
					break;
				}
			}
		}
	}
}

void Window::FocusOnFirstInteractiveElement()
{
	for (p2List_item<WinElement*>* item = children_list.start; item; item = item->next)
	{
		if (item->data->element->UItype == UIType::INTERACTIVE || item->data->element->UItype == UIType::INTERACTIVE_LABEL || item->data->element->UItype == UIType::INTERACTIVE_IMAGE || item->data->element->UItype == UIType::INTERACTIVE_LABELLED_IMAGE)
		{
			item->data->element->hasFocus = true;
			break;
		}
	}
}

WinElement::WinElement(UIElement* element, iPoint relativePosition) : element(element), relativePosition(relativePosition)
{
}
