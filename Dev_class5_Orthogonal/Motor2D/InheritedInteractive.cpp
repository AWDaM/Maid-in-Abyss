#include "InheritedInteractive.h"



InheritedInteractive::InheritedInteractive()
{
}

InheritedInteractive::InheritedInteractive(SDL_Rect& pos) : Interactive(pos)
{
	type = UIType::INTERACTIVE;
}

InheritedInteractive::InheritedInteractive(SDL_Rect& pos, iPoint posOffset, SDL_Rect & size, j1Module * callback, bool draggable) : Interactive(pos, posOffset, callback), UIElement(pos, draggable)
{
	type = UIType::INTERACTIVE;
}


InheritedInteractive::~InheritedInteractive()
{
}

bool InheritedInteractive::PreUpdate()
{
	InteractivePreUpdate();
	return true;
}

bool InheritedInteractive::PostUpdate()
{
	InteractivePostUpdate();
	return true;
}

bool InheritedInteractive::Draw()
{
	InteractiveDraw();
	return true;
}
