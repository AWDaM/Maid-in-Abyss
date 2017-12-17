#ifndef __UIANIMATION_H__
#define __UIANIMATION_H__

#include "UIElement.h"
#include "p2Animation.h"
#include "j1EntityController.h"

class UIClock : virtual public UIElement
{
public:
	UIClock();
	UIClock(SDL_Rect& pos, p2List<Animation>& animations, bool draggable);
	~UIClock();

	bool Draw(float dt);
	bool HandleAnimation(int newstate);
	void ResetAnimations();
	bool CleanUp();

	bool TimeIsModified();

private:
public:
	timeState current_time;
	SDL_Texture* image;
	Animation * current;
	Animation default;
	Animation slowing;
	Animation stopped;
	Animation fastening;
	Animation oncooldown;
private:
};

#endif