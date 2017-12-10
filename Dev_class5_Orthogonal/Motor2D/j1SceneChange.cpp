#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1SceneChange.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Map.h"
#include  "j1EntityController.h"
#include"j1Gui.h"
#include "UIClock.h"

#include "SDL\include\SDL_render.h"
#include "SDL\include\SDL_timer.h"

j1MapChange::j1MapChange()
{
	//TODO: screen en funcio de config
	name.create("mapchange");
}

j1MapChange::~j1MapChange()
{}

bool j1MapChange::Awake(pugi::xml_node&)
{
	screen = { 0, 0, App->win->width*App->win->scale, App->win->height*App->win->scale };
	return true;
}

bool j1MapChange::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool j1MapChange::Update(float dt)
{
	if (current_step == fade_step::none)
	{
		return true;
	}

	uint now = SDL_GetTicks() - start_time;
	float normalized = 1.0f < ((float)now / (float)total_time) ? 1.0f : ((float)now / (float)total_time);

		
	switch (current_step)
	{
			case fade_step::fade_to_black:
		{
			if (now >= total_time) //Point where the screen is totally black, and the new map is loaded.
			{
				App->scene->currentMap = nextMap;
				LOG("%i", App->scene->currentMap);

				App->map->SwitchMaps(App->scene->map_names[nextMap]);
				
				App->entitycontroller->Restart();
				App->gui->clock->ResetAnimations();
				total_time += total_time;
				start_time = SDL_GetTicks();
				fading = false;
				current_step = fade_step::fade_from_black;
			}
		}break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;

			if (now >= total_time)
			{
				current_step = fade_step::none;
				if (App->entitycontroller->Entities.count() <= 1)
				{
					App->scene->SpawnEnemies();
				}
			}

		}break;
	}
	
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

bool j1MapChange::ChangeMap(int newMap, float time)
{
 	bool ret = false;

	nextMap = newMap;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time*0.5f*1000.0f);
		fading = true;
		ret = true;
	}

	return ret;
}

bool j1MapChange::IsChanging() const
{
	return current_step != fade_step::none;
}