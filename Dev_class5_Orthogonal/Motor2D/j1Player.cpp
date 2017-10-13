#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
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

	Player.direction_x = 1;

	LOG("Gone throught that");
	Player.maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	Player.maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();
	LOG("Trough that too");
	Player.accel.x = config.child("accel").attribute("x").as_int();
	Player.accel.y = config.child("accel").attribute("y").as_int();

	Player.colOffset.x = config.child("colOffset").attribute("x").as_int();
	Player.colOffset.y = config.child("colOffset").attribute("y").as_int();

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Player.LoadPushbacks();

	Player.speed = { 0,0 };
	
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
					Player.collider.x = Player.position.x + Player.colOffset.x;
					Player.collider.y = Player.position.y + Player.colOffset.y;
				}
			}
		}
	}
	Player.Player_tex = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	FlipImage();

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Player.direction_x = 1, AddSpeed();
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Player.direction_x = -1, AddSpeed();
	else
		ReduceSpeed();	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		Player.speed.y = -20, Player.grounded = false;


	Player.speed = ApplyGravity(Player.speed);
	Player.speed = Overlay_avoid(Player.speed);

	ChangeAnimation();

	PlayerMovement();
	return true;
}

bool j1Player::PostUpdate()
{
	App->render->camera.x = Player.position.x - App->render->camera.w / 2;
	App->render->camera.y = Player.position.y - App->render->camera.h / 2;
	return true;
}

void j1Player::Draw()
{
	if(Player.flip)
		App->render->Blit(Player.Player_tex, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
	else
		App->render->Blit(Player.Player_tex, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, -1.0);

}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Deleting player");
	App->tex->UnLoad(Player.Player_tex);
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
	CastCollider.x += originalvec.x;
	CastCollider.y += originalvec.y;
	 
	SDL_Rect result;

	iPoint newvec = originalvec;
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
		if (obj->data->name == ("Collisions"))
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
				if (objdata->data->name == ("Floor"))
				{
					if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
					{
						/*int module = newvec.GetModule();
						module -= 1;
						newvec.GetXYfrom_Module_Angle();
						CastCollider.x = Player.collider.x + newvec.x;
						CastCollider.x += Player.collider.x + newvec.y;*/

						
						if (Player.speed.y > 0)
						{
							if (Player.position.y + Player.collider.h + Player.colOffset.y <= objdata->data->y)
							{
								if (Player.speed.x > 0)
								{
									if (result.h <= result.w || Player.position.x + Player.collider.w + Player.colOffset.x <= objdata->data->x)
										newvec.y -= result.h;
									else
										newvec.x -= result.w;
								}
								else if (Player.speed.x < 0)
								{
									if (result.h <= result.w || Player.position.x >= objdata->data->x + objdata->data->width)
										newvec.y -= result.h;
									else
										newvec.x += result.w;
								}
								else
									newvec.y -= result.h;
							}
							else
							{
								if (Player.speed.x > 0)
									newvec.x -= result.w;
								else
									newvec.x += result.w;
							}

						}
					else if (Player.speed.y < 0)
						{
							if (Player.position.y <= objdata->data->y + objdata->data->height)
							{
								if (result.h <= result.w)
									newvec.y += result.h;
								else
								{
									if (Player.speed.x < 0)
										newvec.x += result.w;
									else if (CastCollider.x > 0)
										newvec.x -= result.w;
								}
							}
							else
							{
								if (Player.speed.x > 0)
									newvec.x -= result.w;
								else if (Player.speed.x < 0)
									newvec.x += result.w;
							}
						}
					else
						{
							if (Player.speed.x > 0)
								newvec.x -= result.w;
							else if (Player.speed.x < 0)
								newvec.x += result.w;
						}
					}
				}
				else if (objdata->data->name == ("BGFloor"))
					if (Player.position.y + Player.collider.y + Player.colOffset.y <= objdata->data->y)
						if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
							newvec.y -= result.h;

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

void j1Player::FlipImage()
{
	if (Player.speed.x < 0)
		Player.flip = true;
	else if(Player.speed.x > 0)
		Player.flip = false;
}

void j1Player::AddSpeed()
{
	Player.speed.x += Player.accel.x * Player.direction_x;


	if (Player.direction_x > 0)
	{
		if (Player.speed.x > Player.maxSpeed.x)
			Player.speed.x = Player.maxSpeed.x;
	}

	else
	{
		if (Player.speed.x < Player.direction_x*Player.maxSpeed.x)
			Player.speed.x = Player.direction_x*Player.maxSpeed.x;
	}
}

void j1Player::ReduceSpeed()
{
	if(Player.speed.x != 0)
	Player.speed.x -= Player.accel.x * Player.direction_x;
}

void j1Player::ChangeAnimation()
{
	if (Player.speed.y == 0)
		if (Player.speed.x == 0)
			Player.current_animation = &Player.idle;
		else
			Player.current_animation = &Player.running;
	else
		Player.current_animation = &Player.jumping;
}

void j1Player::PlayerMovement()
{
	Player.position += Player.speed;

	Player.collider.x = Player.position.x + Player.colOffset.x;
	Player.collider.y = Player.position.y + Player.colOffset.y;
}


void j1Player::PositionCameraOnPlayer(SDL_Rect& camera)
{
	/*camera.x = Player.position.x - 100;
	if (camera.x < 0)camera.x = 0;
	camera.y = Player.position.y - 50;
	if (camera.y > 2048)camera.y = 2048 - 768;*/
}
iPoint j1Player::ApplyGravity(iPoint originalvec)
{
		originalvec.y += Player.accel.y;
	return originalvec;
}

void PlayerData::LoadPushbacks()
{
	idle.PushBack({ 5, 17, 56, 73 });

	running.PushBack({ 89, 17, 60, 73 });
	running.PushBack({ 180, 17, 60, 73 });
	running.PushBack({ 277, 17, 60, 73 });
	running.PushBack({ 375, 17, 60, 73 });
	running.PushBack({ 470, 17, 60, 73 });
	running.PushBack({ 565, 17, 60, 73 });
	running.loop = true;
	running.speed = 0.1f;

	jumping.PushBack({ 5, 17, 56, 73 });
}
