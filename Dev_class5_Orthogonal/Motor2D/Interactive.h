#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

#include "UIElement.h"
#include "SDL/include/SDL.h"

class Interactive : virtual public UIElement
{
public:
	Interactive();
	Interactive(SDL_Rect& pos);
	Interactive(SDL_Rect& pos, iPoint posOffset, j1Module* callback);
	~Interactive();

public:
	 bool InteractivePreUpdate();
	 bool InteractivePostUpdate();

	 bool InteractiveDraw();

	 void MoveCollider();


public:
	iPoint Interactiverelativepos;
	SDL_Rect 	collider;
	bool		isMouseInside = false;
	j1Module* callback = nullptr;
	SDL_Texture* fontTexture;
	p2SString text = "Default Text";

};

#endif