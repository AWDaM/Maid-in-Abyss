#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include "Interactive.h"
#include "Image.h"
class Scrollbar : public Image
{
public:
	Scrollbar();
	Scrollbar(SDL_Rect & scroller_image, bool moves_vertically, int min,SDL_Rect & pos, iPoint Sliderrelativepos, SDL_Rect image_section , bool draggable);
	~Scrollbar();

	bool PreUpdate();
	bool Draw();
private:
public:
	SDL_Rect scroller_image;
	SDL_Rect collider;
	bool moves_vertically;
	int min;
	int max;
	uint current_value;
	iPoint mouseLastFrame = { 0,0 };
	bool beingSlided = false;
	iPoint sliderPos;
private:

};

#endif