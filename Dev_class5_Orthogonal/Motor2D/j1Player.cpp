#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Player.LoadPushbacks();

	Player.speed = { 0,0 };
	Player.accel = { 0,0 };
	Player.current_animation = &Player.idle;
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{
			
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start;objdata;objdata = objdata->next)
			{
				if (objdata->data->name == ("Player"))
				{
					Player.collider.h = objdata->data->height;
					Player.collider.w = objdata->data->width;
					Player.collider.x = objdata->data->x;
					Player.collider.y = objdata->data->y;
				}
				else if (objdata->data->name == ("Start"))
				{
					Player.position = { objdata->data->x, objdata->data->y };
				}
			}
		}
	}
	Player.Marisa = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	
	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

void j1Player::Draw()
{
	App->render->Blit(Player.Marisa, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()));
}


// Called before quitting
bool j1Player::CleanUp()
{
	
	return true;
}

// Load Game State
bool j1Player::Load(pugi::xml_node& data)
{
	/*camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();*/

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	/*pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;
*/
	return true;
}

iPoint j1Player::Overlay_avoid(iPoint originalvec)
{
	SDL_Rect CastCollider;
	CastCollider = Player.collider;
	CastCollider.x += Player.speed.x;
	CastCollider.y += Player.speed.y;

	SDL_Rect result;

	iPoint newvec = originalvec;
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{

			for(p2List_item<ObjectsData*>* objdata = obj->data->objects.start;objdata;objdata = objdata->next)
			{
				if(objdata->data->name == ("Floor"))
				{
					if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
					{
						newvec.x -= result.x - 1;
						newvec.y -= result.y - 1;
					}
				}
			}
		}
	}
	
	return newvec;
}

SDL_Rect j1Player::CreateRect_FromObjData(ObjectsData* data)
{
	SDL_Rect ret;
	ret.x = data->x;
	ret.y = data->y;
	ret.h = data->height;
	ret.w = data->width;
	return ret;
}

void PlayerData::LoadPushbacks()
{
	idle.PushBack({ 5, 18, 51, 72 });
}