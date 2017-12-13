#include "Scrollbar.h"
#include "j1Input.h"


Scrollbar::Scrollbar()
{
}

Scrollbar::Scrollbar(SDL_Rect & scroller_image, bool moves_vertically, int min, SDL_Rect & pos, iPoint Sliderrelativepos, SDL_Rect image_section, bool draggable) : Image(pos,{0,0},image_section)
{
	if (moves_vertically)
	{
		this->min = min - scroller_image.h;
		max = scroller_image.y;
	}
	else
	{
		this->min = min - scroller_image.w;
		max = scroller_image.x;
	}

	this->scroller_image = scroller_image;

}




Scrollbar::~Scrollbar()
{
}

bool Scrollbar::PreUpdate()
{
	SDL_Point currentMousepos;
	App->input->GetMousePosition(currentMousepos.x, currentMousepos.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if(SDL_PointInRect(&currentMousepos, &position) && draggable || beingSlided)
			{
			if (moves_vertically)
			{
				MoveElement({ 0, currentMousepos.y - mouseLastFrame.y });
				collider.y += (currentMousepos.y - mouseLastFrame.y);
			}

			else
			{
				MoveElement({ currentMousepos.x - mouseLastFrame.x ,0});
				collider.x += (currentMousepos.x - mouseLastFrame.x);
			}

			beingSlided = true;
			}
		}
	else
	beingSlided = false;
	

	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
	return true;
}

bool Scrollbar::Draw()
{
	ImageDraw(image_section);
	ImageDraw(scroller_image);
	return true;
}
