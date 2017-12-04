#include "LabelledImage.h"



LabelledImage::LabelledImage(SDL_Rect & pos, iPoint posOffsetA, iPoint posOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, SDL_Rect image_section, bool draggable) :
	Label(pos, posOffsetA, fontPath, textColor, label, size), Image(pos, posOffsetB, image_section), UIElement(pos, draggable)
{
	type = UIType::LABELLED_IMAGE;
	this->image_section = image_section;
}


LabelledImage::~LabelledImage()
{
}

bool LabelledImage::PreUpdate()
{
	LabelPreUpdate();
	ImagePreUpdate();
	return true;
}

bool LabelledImage::PostUpdate()
{
	LabelPostUpdate();
	ImagePostUpdate();
	return true;
}

bool LabelledImage::Draw()
{
	LabelDraw();
	ImageDraw(image_section);
	return true;
}