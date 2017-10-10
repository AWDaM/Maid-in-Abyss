#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SDL/include/SDL_rect.h"
#endif

class Animation
{
	//Variables
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[10];

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

	//Functions
public:

	void PushBack(const SDL_Rect& rect)
	{
		if (last_frame < 10)
			frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
		}

		return frames[(int)current_frame];
	}
};