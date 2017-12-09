#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include"j1IntroScene.h"
#include "j1Scene.h"
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
		if(item->data->active)
			 ret = item->data->PreUpdate();

		if (!ret)
			break;
	}

	if (ret)
	{

		for (p2List_item<Window*>* item = window_list.start; item; item = item->next)
		{
			if (item->data->active)
				ret = item->data->PreUpdate();

			if (!ret)
				break;
		}
	}
	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;
	for (p2List_item<UIElement*>* item = elements.start; item; item = item->next)
	{
		if (item->data->active)
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
				if (item->data->active &&SDL_PointInRect(&currentMousepos, &item->data->position) && item->data->draggable || item->data->being_dragged)
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
		if (item->data->active)
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



InheritedImage* j1Gui::AddImage(SDL_Rect& position, iPoint positionOffset, SDL_Rect & section, bool draggable)
{
	InheritedImage* ret = new InheritedImage(position, positionOffset, section, draggable);
	elements.add(ret);
	return ret;
}



InheritedInteractive * j1Gui::AddInteractive(SDL_Rect & position, iPoint positionOffset, SDL_Rect & size, InteractiveType type, j1Module * callback, bool draggable)
{
	InheritedInteractive* ret = new InheritedInteractive(position, positionOffset, size, type, callback, draggable);
	elements.add(ret);
	focusList.add(ret);
	return ret;
}

InheritedLabel* j1Gui::AddLabel(SDL_Rect& position, iPoint positionOffset, p2SString fontPath, SDL_Color textColor, p2SString label, int size, bool draggable)
{
	InheritedLabel* ret = new InheritedLabel(position, positionOffset, fontPath, textColor, label, size, draggable);
	elements.add(ret);

	return ret;
}



InteractiveImage * j1Gui::AddInteractiveImage(SDL_Rect & position, iPoint positionOffsetA, iPoint positionOffsetB, SDL_Rect image_section,InteractiveType type, j1Module * callback, bool draggable)
{
	InteractiveImage* ret = new InteractiveImage(position, positionOffsetA, positionOffsetB, image_section, type, callback, draggable);
	elements.add(ret);
	focusList.add(ret);
	return ret;
}

InteractiveLabel * j1Gui::AddInteractiveLabel(SDL_Rect & position, iPoint positionOffsetA, iPoint positionOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, InteractiveType type, j1Module * callback, bool draggable)
{
	InteractiveLabel* ret = new InteractiveLabel(position, positionOffsetA, positionOffsetB, fontPath, textColor, label, size, type, callback, draggable);
	elements.add(ret);
	focusList.add(ret);
	return ret;
}



InteractiveLabelledImage * j1Gui::AddInteractiveLabelledImage(SDL_Rect & position, iPoint positionOffsetA, iPoint positionOffsetB, iPoint positionOffsetC, SDL_Rect & image_section, p2SString & fontPath, SDL_Color & textColor, p2SString & label, int size, InteractiveType type, j1Module * callback, bool draggable)
{
	InteractiveLabelledImage* ret = new InteractiveLabelledImage(position, positionOffsetA, positionOffsetB, positionOffsetC, image_section, fontPath, textColor, label, size, type, callback, draggable);
	elements.add(ret);
	focusList.add(ret);
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
	Window* window = new Window(windowrect,draggable);

	window_list.add(window);

	return window;
}

void j1Gui::Load_UIElements(pugi::xml_node node, j1Module* callback)
{

	pugi::xml_node tmp;

	tmp = node.child("alternate_image");
	if (tmp)
	{
		Load_AlterantiveImage_fromXML(tmp);
		while (tmp = tmp.next_sibling("alternate_image"))
		{
			Load_AlterantiveImage_fromXML(tmp);
		}
	}
	 tmp = node.child("interactivelabelledimage");

 node.child("image");

	if (tmp)
	{
		App->gui->Load_Image_fromXML(tmp);
		while (tmp = tmp.next_sibling("image"))
		{
			App->gui->Load_Image_fromXML(tmp);
		}
	}

	tmp = node.child("interactivelabelledimage");
	if (tmp)
	{
		App->gui->Load_InteractiveLabelledImage_fromXML(tmp,callback);
		while (tmp = tmp.next_sibling("interactivelabelledimage"))
		{
			App->gui->Load_InteractiveLabelledImage_fromXML(tmp,callback);
		}
	}

}

void j1Gui::Load_SceneWindows(pugi::xml_node node,j1Module* callback)
{
	pugi::xml_node tmp = node.child("window");

	if (tmp)
	{
		App->gui->Load_Window_fromXML(tmp,callback);
		while (tmp = tmp.next_sibling("window"))
		{
			App->gui->Load_Window_fromXML(tmp,callback);
		}
	}
}

UIElement * j1Gui::Load_InteractiveLabelledImage_fromXML(pugi::xml_node tmp, j1Module* callback)
{
	InteractiveLabelledImage* added = nullptr;
	
	SDL_Rect pos = { tmp.child("pos").attribute("x").as_int(), tmp.child("pos").attribute("y").as_int(), tmp.child("pos").attribute("w").as_int(), tmp.child("pos").attribute("h").as_int() };
	iPoint relativeposA = { tmp.child("relativeposA").attribute("x").as_int(),tmp.child("relativeposA").attribute("y").as_int() };
	iPoint relativeposB = { tmp.child("relativeposB").attribute("x").as_int(),tmp.child("relativeposB").attribute("y").as_int() };
	iPoint relativeposC = { tmp.child("relativeposC").attribute("x").as_int(),tmp.child("relativeposC").attribute("y").as_int() };
	SDL_Rect section = { tmp.child("imagesection").attribute("x").as_int(), tmp.child("imagesection").attribute("y").as_int(), tmp.child("imagesection").attribute("w").as_int(), tmp.child("imagesection").attribute("h").as_int() };
	p2SString path = (tmp.child("fontpath").attribute("path").as_string());
	SDL_Color color = { tmp.child("color").attribute("r").as_int(), tmp.child("color").attribute("g").as_int(), tmp.child("color").attribute("b").as_int(), tmp.child("color").attribute("a").as_int() };
	p2SString label = (tmp.child("label").attribute("string").as_string());
	int size = tmp.child("size").attribute("value").as_int();
	InteractiveType type = InteractiveType_from_int(tmp.child("type").attribute("value").as_int());
	bool draggable = tmp.child("draggable").attribute("value").as_bool();

	added = AddInteractiveLabelledImage(pos, relativeposA, relativeposB, relativeposC, section, path, color, label, size, type, callback, draggable);
	
	added->hover = { tmp.child("hover").attribute("x").as_int(), tmp.child("hover").attribute("y").as_int(), tmp.child("hover").attribute("w").as_int(), tmp.child("hover").attribute("h").as_int() };
	added->click = { tmp.child("click").attribute("x").as_int(), tmp.child("click").attribute("y").as_int(), tmp.child("click").attribute("w").as_int(), tmp.child("click").attribute("h").as_int() };
	added->inactive = { tmp.child("inactive").attribute("x").as_int(), tmp.child("inactive").attribute("y").as_int(), tmp.child("inactive").attribute("w").as_int(), tmp.child("inactive").attribute("h").as_int() };

	return added;
}

Window * j1Gui::Load_Window_fromXML(pugi::xml_node node, j1Module* callback)
{
	SDL_Rect collider = { node.child("collider").attribute("x").as_int(), node.child("collider").attribute("y").as_int(), node.child("collider").attribute("w").as_int(), node.child("collider").attribute("h").as_int() };

	bool draggable =  node.child("draggable").attribute("value").as_bool(false);
	Window* added = AddWindow(collider,draggable);
	if (node.child("elements"))
	{
		Load_WindowElements_fromXML(node.child("elements"), added,callback);
	}
	return added;
}

void j1Gui::Load_WindowElements_fromXML(pugi::xml_node node, Window * window, j1Module* callback)
{
	pugi::xml_node tmp = node.child("interactivelabelledimage");

	UIElement* child;
	if (tmp)
	{
		child = App->gui->Load_InteractiveLabelledImage_fromXML(tmp,callback);
		window->AddElementToWindow(child, { tmp.child("winRelativePos").attribute("x").as_int(),tmp.child("winRelativePos").attribute("y").as_int() });
		while (tmp = tmp.next_sibling("interactivelabelledimage"))
		{
			child = App->gui->Load_InteractiveLabelledImage_fromXML(tmp,callback);
			window->AddElementToWindow(child, { tmp.child("winRelativePos").attribute("x").as_int(),tmp.child("winRelativePos").attribute("y").as_int()});
		}
	}

	tmp = node.child("image");
	if (tmp)
	{
		child = App->gui->Load_Image_fromXML(tmp);
		window->AddElementToWindow(child, { tmp.child("winRelativePos").attribute("x").as_int(),tmp.child("winRelativePos").attribute("y").as_int() });
		while (tmp = tmp.next_sibling("image"))
		{
			App->gui->Load_Image_fromXML(tmp);
			window->AddElementToWindow(child, { tmp.child("winRelativePos").attribute("x").as_int(),tmp.child("winRelativePos").attribute("y").as_int() });
		}
	}
}

UIElement * j1Gui::Load_Image_fromXML(pugi::xml_node node)
{
	SDL_Rect position = { node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int(), node.child("position").attribute("w").as_int(), node.child("position").attribute("h").as_int() };
	iPoint relativePos = { node.child("relativePosition").attribute("x").as_int(),node.child("relativePosition").attribute("y").as_int() };
	SDL_Rect image_section = { node.child("image_section").attribute("x").as_int(), node.child("image_section").attribute("y").as_int(), node.child("image_section").attribute("w").as_int(), node.child("image_section").attribute("h").as_int() };
	bool draggable = node.child("draggable").attribute("value").as_bool();
	Image* added = AddImage(position, relativePos, image_section, draggable);
	return added;
}

UIElement * j1Gui::Load_AlterantiveImage_fromXML(pugi::xml_node node)
{
	p2SString path = node.child("path").attribute("string").as_string();
	SDL_Rect position = { node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int(), node.child("position").attribute("w").as_int(), node.child("position").attribute("h").as_int() };
	iPoint relativePos = { node.child("relativePosition").attribute("x").as_int(),node.child("relativePosition").attribute("y").as_int() };
	bool draggable = node.child("draggable").attribute("value").as_bool();
	UIElement* added = AddImage_From_otherFile(position, relativePos, path, draggable);
	return added;
}

InteractiveType j1Gui::InteractiveType_from_int(int type)
{
	InteractiveType ret;
	switch (type)
	{
	case(0):
		ret= DEFAULT;
		break;
	case(1):
		 ret = QUIT;
		break;
	case(2):
		 ret = CONTINUE;
		break;
	case(3):
		ret = NEWGAME;
		break;
	case(4):
		ret = OPEN_SETTINGS;
		break;
	case(5):
		ret = OPEN_CREDITS;
		break;
	};
	return ret;
}

bool j1Gui::CreateSceneIntroGUI()
{
	//SDL_Rect backgroundrect = { 0,0,0,0 };
	////AddImage_From_otherFile(backgroundrect, { 0,0 }, background);
	////{0, 0, 122, 74};
	////{132, 19, 311, 131};
	//SDL_Rect rect1 = { 960 - 61,800,122,74 };
	//SDL_Rect rect2 = { 0,0,311,131 };
	//SDL_Rect rect3 = { 0,0,130,32 };
	///*AddInteractiveImage(rect1, { 0,0 }, { 0,0 }, { 960 - 61,800, 122, 74 }, { 0, 0, 122, 74 }, this);
	//AddInteractiveImage(rect2, { 0,0 }, { 0,0 }, { 0, 0, 311, 131 }, { 132, 19, 311, 131 }, this);*/
	//InteractiveImage* tmp = AddInteractiveImage(rect3, { 0,0 }, { 0,0 },  { 0, 74, 130, 32 }, (j1Module*)App->scene, true);
	//tmp->click = { 0,105,130,32 };
	//tmp->hover = { 0,150,145,43 };

	//SDL_Rect window_rect = { 0,0,500,500 };
	//Window* window = AddWindow(window_rect, true);
	//window->AddElementToWindow(tmp, { 50,50 });
	///*{0, 74, 130, 32}
	//{0,105,130,32}
	//{0,150,145,43}*/

	
	return true;
}

// const getter for atlas
 SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

 bool j1Gui::OnEvent(UIElement * element, int eventType)
 {
	 if(currentFocus > -1)
		if (element == focusList[currentFocus])
		{
			LOG("we did it reddit %i", currentFocus);
		}
	 element->HandleAnimation(eventType);
	 return true;
 }
