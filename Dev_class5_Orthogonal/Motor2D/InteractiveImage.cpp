#include "InteractiveImage.h"
#include "j1Render.h"




InteractiveImage::InteractiveImage(SDL_Rect & pos) : Interactive(pos)
{
}


InteractiveImage::InteractiveImage(SDL_Rect & pos, iPoint Interactiverelativepos, iPoint Imagerelativepos, SDL_Rect image_section, j1Module* callback, bool draggable) :
	Interactive(pos,Interactiverelativepos,callback), Image(pos,Imagerelativepos,image_section) , UIElement(pos, draggable)
{
	type = INTERACTIVE_IMAGE;
	current = &this->image_section;
}


InteractiveImage::~InteractiveImage()
{
}

bool InteractiveImage::PreUpdate()
{
	InteractivePreUpdate();
	ImagePreUpdate();
	return true;
}

bool InteractiveImage::Update(float dt)
{
	return true;
}

bool InteractiveImage::PostUpdate()
{
	InteractivePostUpdate();
	ImagePostUpdate();
	return true;
}

bool InteractiveImage::Draw()
{
	InteractiveDraw();
	ImageDraw(*current);
	return true;
}

bool InteractiveImage::HandleAnimation(int eventType)
{
	if (eventType == 4)
	{
		current = &hover;
	}
	else if (eventType == 5 && !SDL_RectEquals(current, &click))
	{
		current = &image_section;
	}
	else if (eventType == 0 || eventType == 2)
	{
		current = &click;
	}
	else if (eventType == 1 || eventType == 3)
	{
		current = &image_section;
	}
	return true;
}
