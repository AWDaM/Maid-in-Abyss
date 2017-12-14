#include "j1EntityController.h"
#include "LandMaid.h"

LandMaid::LandMaid() : Enemy(entityType::LAND_ENEMY)
{
	canFly = false;
}

LandMaid::LandMaid(iPoint position) : Enemy(entityType::LAND_ENEMY, position)
{
	LoadPushbacks();
	Current_Animation = &idle;
	canFly = false;
	speed = { 0,0 };

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);
		
	config = config.child("entitycontroller").child("landmaid");

	maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();
	gravity = config.child("gravity").attribute("value").as_float();
	direction_x = -1;
	colOffset.x = config.child("colOffset").attribute("x").as_int();
	colOffset.y = config.child("colOffset").attribute("y").as_int();
	Collider.h = config.child("Collider").attribute("h").as_int();
	Collider.w = config.child("Collider").attribute("w").as_int();
	Collider.x = position.x;
	Collider.y = position.y;
	sightOffset.x = config.child("sightOffset").attribute("x").as_int();
	sightOffset.y = config.child("sightOffset").attribute("y").as_int();
	SightCollider.x = position.x - sightOffset.x;
	SightCollider.y = position.y - sightOffset.y;
	SightCollider.w = config.child("SightCollider").attribute("w").as_int();
	SightCollider.h = config.child("SightCollider").attribute("h").as_int();
}

LandMaid::~LandMaid()
{
}

bool LandMaid::Update(float dt)
{
	if (dt > 1) return true;

	if(target == nullptr)
	{
		target = GetTarget();
	}

	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
	{
		DoPathfinding = true;
		accumulated_time = 0.0f;
	}

	if (App->entitycontroller->time_state != STOPPED)
	{
		if (chasing_player)
		{
			Current_Animation = &moving;
			if (DoPathfinding)
			{
				DoPathfinding = false;
				if (App->pathfinding->CreatePath(App->map->WorldToMap(position.x + colOffset.x, position.y + colOffset.y), App->map->WorldToMap(target->position.x + target->colOffset.x, target->position.y + target->colOffset.y), canFly) > -1)
				{
					path = *App->pathfinding->GetLastPath();

					if (path.Count() > 0 && currentPathtile != *path.At(0))currentPathtile = *path.At(0);

					else if (path.Count() > 1)currentPathtile = *path.At(1);

					else speed.x = 0;

				}
				else
				{
					speed.x = 0;
				}
			}
		}
		else
		{
			Current_Animation = &idle;
		}
	}

	Move();
	speed.x = speed.x*dt;
	speed.y = gravity*dt;
	speed = Collider_Overlay(speed);

	speed.x = (int)speed.x;
	speed.y = (int)speed.y;

	FlipImage();
	position.x += speed.x;
	position.y += speed.y;
	PositionCollider();
	//Current_Animation->speed = animationSpeed*dt;


	return true;
}

bool LandMaid::Start()
{
	return true;
}

bool LandMaid::Move()
{
	if (chasing_player)
	{
		iPoint pos = App->map->WorldToMap(position.x, position.y);
		if(currentPathtile.x != 0)
			speed.x = currentPathtile.x - pos.x;

		speed.x *= maxSpeed.x;
		
	}
	else
		speed.x = 0;

	if (position.x < target->position.x && chasing_player)
	{
		direction_x = 1;
	}
	else if (position.x > target->position.x && chasing_player)
	{
		direction_x = -1;
	}

	PositionCollider();
	FlipImage();
	return true;
}

void LandMaid::LoadPushbacks()
{
	idle.PushBack({ 0,575,50,65 });
	idle.PushBack({ 73,575,50,65 });
	idle.PushBack({ 147,575,50,65 });
	idle.PushBack({ 219,575,50,65 });
	idle.PushBack({ 291,575,50,65 });
	idle.PushBack({ 362,575,50,65 });
	idle.loop = true;
	idle.speed = 9.0f;

	moving.PushBack({ 219,873,65,65 });
	moving.PushBack({ 291,873,65,65 });
	moving.PushBack({ 362,873,65,65 });
	moving.PushBack({ 435,873,65,65 });
	moving.PushBack({ 507,873,65,65 });
	moving.PushBack({ 578,873,65,65 });
	moving.loop = true;
	moving.speed = 9.0f;
}


void LandMaid::CleanUp()
{
}
