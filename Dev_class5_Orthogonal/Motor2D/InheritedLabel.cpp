#include "InheritedLabel.h"



InheritedLabel::InheritedLabel()
{
}

InheritedLabel::InheritedLabel(SDL_Rect& position, iPoint positionOffset, p2SString fontPath, SDL_Color textColor, p2SString & label, int size, bool draggable) : Label(position, positionOffset, fontPath, textColor, label, size), UIElement(position, draggable)
{
}


InheritedLabel::~InheritedLabel()
{
}

bool InheritedLabel::PreUpdate()
{
	LabelPreUpdate();
	return true;
}

bool InheritedLabel::PostUpdate()
{
	LabelPostUpdate();
	return true;
}

bool InheritedLabel::Draw()
{
	LabelDraw();
	return true;
}
