#include "InteractiveLabelledImage.h"



InteractiveLabelledImage::InteractiveLabelledImage(SDL_Rect & pos, iPoint posOffsetA, iPoint posOffsetB, iPoint posOffsetC, SDL_Rect image_section, p2SString fontPath, SDL_Color textColor, p2SString label, int size, InteractiveType type, j1Module* callback, bool draggable) :
	Interactive(pos, posOffsetA, type, callback), Label(pos, posOffsetB, fontPath, textColor, label, size), Image(pos, posOffsetC, image_section), UIElement(pos, draggable)
{
	UItype = UIType::INTERACTIVE_LABELLED_IMAGE;
	this->image_section = image_section;
	current = &this->image_section;
}


InteractiveLabelledImage::~InteractiveLabelledImage()
{
}

bool InteractiveLabelledImage::PreUpdate()
{
	bool ret = true;
	ret = InteractivePreUpdate();

	if(ret)
	ret = LabelPreUpdate();

	if(ret)
	ret = ImagePreUpdate();

	return ret;
}

bool InteractiveLabelledImage::PostUpdate()
{
	InteractivePostUpdate();
	LabelPostUpdate();
	ImagePostUpdate();
	return true;
}

bool InteractiveLabelledImage::Draw()
{
	InteractiveDraw();

	ImageDraw(*current);
	LabelDraw();
	return true;
}

bool InteractiveLabelledImage::HandleAnimation(int eventType)
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
