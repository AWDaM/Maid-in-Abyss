#include "InteractiveLabel.h"


InteractiveLabel::InteractiveLabel(SDL_Rect & pos, iPoint posOffsetA, iPoint posOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, j1Module * callback, bool draggable) :
	Interactive(pos, posOffsetA, callback), Label(pos, posOffsetB, fontPath, textColor, label, size), UIElement(pos, draggable)
{
	type = UIType::INTERACTIVE_LABEL;
}

InteractiveLabel::~InteractiveLabel()
{
}

bool InteractiveLabel::PreUpdate()
{
	InteractivePreUpdate();
	LabelPreUpdate();
	return true;
}

bool InteractiveLabel::PostUpdate()
{
	InteractivePostUpdate();
	LabelPostUpdate();
	return true;
}

bool InteractiveLabel::Draw()
{
	InteractiveDraw();
	LabelDraw();
	return true;
}