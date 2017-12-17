
#include "j1EntityController.h"
#include "Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1SceneChange.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"


Entity::Entity()
{
}

Entity::Entity(entityType type) : type(type)
{
}

Entity::~Entity()
{
}

void Entity::Draw(float dt)
{
		if (flip)
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL, -1.0);
		else
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, -1.0);
}

fPoint Entity::SpeedBoundaries(fPoint originalvec)
{
	if (direction_x > 0)
	{
		if (originalvec.x > maxSpeed.x)
			originalvec.x = maxSpeed.x;
	}

	else
	{
		if (originalvec.x < direction_x*maxSpeed.x)
			originalvec.x = direction_x*maxSpeed.x;
	}

	if (originalvec.y > maxSpeed.y)
	{
		originalvec.y = maxSpeed.y;
	}
	return originalvec;
}

fPoint Entity::Collider_Overlay(fPoint originalvec)
{

	SDL_Rect CastCollider;
	CastCollider = Collider;
	CastCollider.x += originalvec.x;
	CastCollider.y += originalvec.y;

	SDL_Rect result;



	fPoint newvec = originalvec;
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
				{
					if (objdata->data->name == 1)
					{
						newvec = AvoidCollision(newvec, result, objdata);
					}
					else if (objdata->data->name == 2) //Only collides if the player is above the platform
					{
						if (position.y + Collider.h <= objdata->data->y)
							if (result.h <= result.w || position.x + Collider.w + colOffset.x >= objdata->data->x)
								newvec.y -= result.h, BecomeGrounded();
					}
					else if (objdata->data->name == 3 && !App->scenechange->IsChanging()) //Detects when the player falls
					{
							alive = false;
					}
					else if (objdata->data->name == 5 && App->scene->to_end == false && type == Entity::entityType::PLAYER && !isDying) //Detects when the player has finished the level
					{
							App->scene->to_end = true;
					}
					//The new trajectory of the player is adjousted for the next collision check
					CastCollider.x -= (originalvec.x - newvec.x);
					CastCollider.y -= (originalvec.y - newvec.y);
					//LOG("Newvec x: %i y: %i", newvec.x, newvec.y);

				}
			}
		}
	}


	return newvec;
}

fPoint Entity::AvoidCollision(fPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata)
{
	if (type == PLAYER)
	{
		type = PLAYER;
	}
	if (newvec.y > 0)
	{
		if (position.y + Collider.h + colOffset.y <= objdata->data->y)
		{
			if (newvec.x > 0)
			{
				if (result.h <= result.w || position.x + Collider.w + colOffset.x > objdata->data->x)
					newvec.y -= result.h, BecomeGrounded();
				else
					newvec.x -= result.w;
			}
			else if (newvec.x < 0)
			{
				if (result.h <= result.w || position.x + colOffset.x > objdata->data->x + objdata->data->width)
					newvec.y -= result.h, BecomeGrounded();
				else
					newvec.x += result.w;
			}
			else
				newvec.y -= result.h, BecomeGrounded();
		}
		else
		{
			if (newvec.x > 0)
				newvec.x -= result.w;
			else
				newvec.x += result.w;
		}

	}
	else if (newvec.y < 0)
	{
		if (position.y + colOffset.y >= objdata->data->y + objdata->data->height)
		{
			if (newvec.x > 0)
			{
				if (result.h <= result.w ||	position.x + Collider.w + colOffset.x >= objdata->data->x)
					newvec.y += result.h;
				else
					newvec.x -= result.w;
			}
			else if (newvec.x < 0)
			{
				if (result.h <= result.w || position.x + colOffset.x <= objdata->data->x + objdata->data->width)
					newvec.y += result.h;
				else
					newvec.x += result.w;
			}
			else
				newvec.y += result.h;
		}
		else
		{
			if (newvec.x > 0)
				newvec.x -= result.w;
			else if (newvec.x < 0)
				newvec.x += result.w;
			else
				newvec.y += result.h;
		}
	}
	else
	{
		if (newvec.x > 0)
			newvec.x -= result.w;
		else if (newvec.x < 0)
			newvec.x += result.w;
	}

	if (newvec.y < 1 && newvec.y > -1)
	{
		newvec.y = 0;
	}
	if (newvec.x < 1 && newvec.x > -1)
	{
		newvec.x = 0;
	}

	return newvec;
}

SDL_Rect Entity::CreateRect_FromObjData(ObjectsData * data)
{
	SDL_Rect ret;
	ret.x = data->x;
	ret.y = data->y;
	ret.h = data->height;
	ret.w = data->width;
	return ret;
}

void Entity::AddSFX(int channel, int repeat, uint volume)
{
	App->audio->PlayFx(channel, repeat, volume);
}

void Entity::NormalizeAnimationSpeed(float dt)
{
//	Current_Animation->speed = animationSpeed*dt;
}

void Entity::FlipImage()
{

	if (type == PLAYER)
	{
		if (speed.x < 0)
			flip = true;
		else if (speed.x > 0)
			flip = false;
	}
	else
	{
		if (direction_x == 1)
		{
			flip = false;
		}
		else if (direction_x == -1)
		{
			flip = true;
		}

	}
}

void Entity::PositionCollider()
{
	Collider.x = position.x + colOffset.x;
	Collider.y = position.y + colOffset.y;
	if (type != PLAYER)
	{
		if (direction_x == 1)
		{
			SightCollider.x = position.x - sightOffset.x;
			SightCollider.y = position.y - sightOffset.y;
		}
		else
		{
			SightCollider.x = position.x - SightCollider.w + sightOffset.x;
			SightCollider.y = position.y - sightOffset.y;
		}
		
	}
}
