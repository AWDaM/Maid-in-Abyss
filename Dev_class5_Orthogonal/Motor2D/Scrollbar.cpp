#include "Scrollbar.h"
#include "j1Input.h"
#include "j1Render.h"


Scrollbar::Scrollbar()
{
}

Scrollbar::Scrollbar(SDL_Rect & scroller_image, bool moves_vertically, int min, SDL_Rect & pos, iPoint Sliderrelativepos, SDL_Rect image_section, bool draggable) : Image(pos,{0,0},image_section)
{
	this->moves_vertically = moves_vertically;

	if (moves_vertically)
	{
		this->min = Sliderrelativepos.y + min - scroller_image.h;
		max = Sliderrelativepos.y;
	}
	else
	{
		this->min = Sliderrelativepos.x + min - scroller_image.w;
		max = Sliderrelativepos.x;
	}

	sliderPos.x = pos.x + Sliderrelativepos.x;
	sliderPos.y = pos.y + Sliderrelativepos.y;
	this->scroller_image = scroller_image;
	collider.x = pos.x + Sliderrelativepos.x;
	collider.y = pos.y + Sliderrelativepos.y;
	collider.w = scroller_image.w;
	collider.h = scroller_image.h;
	App->input->GetMousePosition(mouseLastFrame.x, mouseLastFrame.y);
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
		if(SDL_PointInRect(&currentMousepos, &collider) || beingSlided)
			{
			if (moves_vertically)
			{
				int movement = currentMousepos.y - mouseLastFrame.y;

					sliderPos.y += movement;
					if (sliderPos.y > min)
						sliderPos.y = min;
					if (sliderPos.y < max)
						sliderPos.y = max;
					collider.y += movement;
					if (collider.y < min)
						collider.y = min;
					if (collider.y > max)
						collider.y = max;

			}

			else
			{
				int movement = currentMousepos.x - mouseLastFrame.x;
			
					sliderPos.x += movement;
					if (sliderPos.x > min)
						sliderPos.x = min;
					if (sliderPos.x < max)
						sliderPos.x = max;
					collider.x += movement;
					if (collider.x < min)
						collider.x = min;
					if (collider.x > max)
						collider.x = max;
				
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
	App->render->Blit(image, sliderPos.x, sliderPos.y, &scroller_image);
	return true;
}
