#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Window.h"
#include "UIElement.h"
#include "Label.h"
#include "Image.h"
#include "Interactive.h"
#include "InteractiveImage.h"
#include "InteractiveLabel.h"
#include "InteractiveLabelledImage.h"
#include "LabelledImage.h"
#include "InheritedImage.h"
#include "InheritedInteractive.h"
#include "InheritedLabel.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	background = conf.child("background").attribute("file").as_string("");


	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	
	atlas = App->tex->Load(atlas_file_name.GetString());
	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
	CreateSceneIntroGUI();

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;
	for (p2List_item<UIElement*>* item = elements.start; item; item = item->next)
	{
		item->data->PreUpdate();
		if (!ret)
			break;
	}

	for (p2List_item<Window*>* item = window_list.start; item; item = item->next)
	{
		item->data->PreUpdate();
		if (!ret)
			break;
	}
	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;
	for (p2List_item<UIElement*>* item = elements.start; item; item = item->next)
	{
		ret = item->data->PostUpdate();
		
		if (!ret)
			break;
	}


		SDL_Point currentMousepos;
		App->input->GetMousePosition(currentMousepos.x, currentMousepos.y);
		for (p2List_item<UIElement*>* item = elements.start; item; item = item->next)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				if (SDL_PointInRect(&currentMousepos, &item->data->position) && item->data->draggable || item->data->being_dragged)
				{
					item->data->MoveElement({ currentMousepos.x - mouseLastFrame.x, currentMousepos.y - mouseLastFrame.y });
					item->data->being_dragged = true;
				}
				if (!ret)
					break;
			}
			else
				item->data->being_dragged = false;
		}
	
	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
	return ret;
}

bool j1Gui::Draw()
{
	bool ret = true;
	for (p2List_item<UIElement*>* item = elements.start; item; item = item->next)
	{
		item->data->Draw();
		if (!ret)
			break;
	}

	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	/*LOG("Freeing GUI");
	for (p2List_item<UIElement*>* item = elements.start; item; item = item->next)
	{
		RELEASE(item->data);
	}
	elements.clear();*/
	return true;
}

UIElement* j1Gui::AddElement(UIType type, SDL_Rect& position, iPoint positionOffset, bool draggable)
{
	UIElement* ret = nullptr;
	switch (type)
	{
	case INTERACTIVE:
		ret = new Interactive(position);
		break;
	case IMAGE:
		ret = new Image();
		break;
	case LABEL:
		ret = new Label();
		break;
	case INTERACTIVE_IMAGE:
		//
		break;
	case INTERACTIVE_LABELLED_IMAGE:
		//
		break;
	case INTERACTIVE_LABEL:
		//
		break;
	case LABELLED_IMAGE:
		//
		break;
	default:
		//
		break;
	}
	
	elements.add(ret);
	return ret;
}

InheritedImage* j1Gui::AddImage(SDL_Rect& position, iPoint positionOffset, SDL_Rect * section, bool draggable)
{
	InheritedImage* ret = new InheritedImage(position, positionOffset, *section, draggable);
	elements.add(ret);
	return ret;
}

InheritedInteractive* j1Gui::AddInteractive(SDL_Rect& position, iPoint positionOffset, SDL_Rect & size, j1Module * callback, bool draggable)
{
	InheritedInteractive* ret = new InheritedInteractive(position, positionOffset, size, callback, draggable);
	elements.add(ret);

	return ret;
}

InheritedLabel* j1Gui::AddLabel(SDL_Rect& position, iPoint positionOffset, p2SString fontPath, SDL_Color textColor, p2SString label, int size, bool draggable)
{
	InheritedLabel* ret = new InheritedLabel(position, positionOffset, fontPath, textColor, label, size, draggable);
	elements.add(ret);

	return ret;
}

InteractiveImage* j1Gui::AddInteractiveImage(SDL_Rect& position, iPoint Interactiverelativepos, iPoint Imagerelativepos, SDL_Rect image_section, j1Module * callback, bool draggable)
{
	InteractiveImage* ret = new InteractiveImage(position, Interactiverelativepos, Imagerelativepos, image_section, callback, draggable);
	elements.add(ret);
	return ret;
}

InteractiveLabel* j1Gui::AddInteractiveLabel(SDL_Rect & position, iPoint Interactiverelativepos, iPoint positionOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, j1Module * callback, bool draggable)
{
	InteractiveLabel* ret = new InteractiveLabel(position, Interactiverelativepos, positionOffsetB, fontPath, textColor, label, size, callback, draggable);
	elements.add(ret);
	return ret;
}

InteractiveLabelledImage* j1Gui::AddInteractiveLabelledImage(SDL_Rect & position, iPoint Interactiverelativepos, iPoint positionOffsetB, iPoint Imagerelativepos, SDL_Rect image_section, p2SString fontPath, SDL_Color textColor, p2SString label, int size, j1Module * callback, bool draggable)
{
	InteractiveLabelledImage* ret = new InteractiveLabelledImage(position, Interactiverelativepos, positionOffsetB, Imagerelativepos, image_section, fontPath, textColor, label, size, callback, draggable);
	elements.add(ret);
	return ret;
}

LabelledImage* j1Gui::AddLabelledImage(SDL_Rect & position, iPoint positionOffsetA, iPoint Imagerelativepos, p2SString fontPath, SDL_Color textColor, p2SString label, int size, SDL_Rect image_section, bool draggable)
{
	LabelledImage* ret = new LabelledImage(position, positionOffsetA, Imagerelativepos, fontPath, textColor, label, size, image_section, draggable);
	elements.add(ret);
	return ret;
}





UIElement* j1Gui::DeleteElement(UIElement* element)
{
	int index = elements.find(element);
	p2List_item<UIElement*>* item = nullptr;
	for (item = elements.start; item; item = item->next)
	{
		if (item->data == element)
		{
			break;
		}
	}
	elements.del(item);

	return item->data;
}

UIElement* j1Gui::AddImage_From_otherFile(SDL_Rect& position, iPoint positionOffset, p2SString &path, bool draggable)
{
	UIElement* element = new InheritedImage(position, positionOffset, path, draggable);

	elements.add(element);

	return element;
}

Window * j1Gui::AddWindow(SDL_Rect &windowrect, bool draggable)
{
	Window* window = new Window(windowrect);

	window_list.add(window);

	return window;
}

bool j1Gui::CreateSceneIntroGUI()
{
	SDL_Rect backgroundrect = { 0,0,0,0 };
	//AddImage_From_otherFile(backgroundrect, { 0,0 }, background);
	//{0, 0, 122, 74};
	//{132, 19, 311, 131};
	SDL_Rect rect1 = { 960 - 61,800,122,74 };
	SDL_Rect rect2 = { 0,0,311,131 };
	SDL_Rect rect3 = { 0,0,130,32 };
	/*AddInteractiveImage(rect1, { 0,0 }, { 0,0 }, { 960 - 61,800, 122, 74 }, { 0, 0, 122, 74 }, this);
	AddInteractiveImage(rect2, { 0,0 }, { 0,0 }, { 0, 0, 311, 131 }, { 132, 19, 311, 131 }, this);*/
	InteractiveImage* tmp = AddInteractiveImage(rect3, { 0,0 }, { 0,0 },  { 0, 74, 130, 32 }, (j1Module*)App->scene, true);
	tmp->click = { 0,105,130,32 };
	tmp->hover = { 0,150,145,43 };

	SDL_Rect window_rect = { 0,0,500,500 };
	/*Window* window = AddWindow(window_rect, true);
	window->AddElementToWindow(tmp, { 50,50 });*/
	/*{0, 74, 130, 32}
	{0,105,130,32}
	{0,150,145,43}*/

	
	return true;
}

// const getter for atlas
 SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

