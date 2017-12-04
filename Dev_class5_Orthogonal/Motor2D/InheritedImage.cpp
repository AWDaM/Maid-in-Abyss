#include "InheritedImage.h"



InheritedImage::InheritedImage()
{
}

InheritedImage::InheritedImage(SDL_Rect& position, iPoint positionOffset, bool draggable) : Image(position, positionOffset), UIElement(position, draggable)
{
}

InheritedImage::InheritedImage(SDL_Rect& position, iPoint positionOffset, SDL_Rect & image_section, bool draggable) : Image(position, positionOffset, image_section), UIElement(position, draggable)
{
}

InheritedImage::InheritedImage(SDL_Rect& position, iPoint positionOffset, p2SString & path, bool draggable) : Image(path, position, positionOffset), UIElement(position, draggable)
{
}


InheritedImage::~InheritedImage()
{
}

bool InheritedImage::PreUpdate()
{
	ImagePreUpdate();
	return true;
}

bool InheritedImage::PostUpdate()
{
	ImagePostUpdate();
	return true;
}

bool InheritedImage::Draw()
{
	ImageDraw(image_section);
	return false;
}
