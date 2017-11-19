#include "j1EntityController.h"
#include "FlyingFurrball.h"
#include "j1Scene.h"
#include "j1Render.h"


FlyingFurrball::FlyingFurrball() : Enemy(entityType::FLYING_ENEMY)
{

}

FlyingFurrball::FlyingFurrball(iPoint pos) : Enemy(entityType::FLYING_ENEMY, pos)
{
	LoadPushbacks();
	Current_Animation = &idle;
	canFly = true;
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	config = config.child("entitycontroller").child("flyingfurrball");

	speed = { 0,0 };

	maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();
	gravity = config.child("gravity").attribute("value").as_float();
	direction_x = 1;
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


FlyingFurrball::~FlyingFurrball()
{
}

bool FlyingFurrball::Update(float dt)
{
	if (dt > 1) return true;

	if (target == nullptr)
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

					else speed = { 0,0 };
				}
				else
					speed = { 0,-100 };
			}
		}
		else
		{
			Current_Animation = &idle;
			speed = { 0, 0 };
		}
	}

		PositionCollider();
		Move();
		speed.x = speed.x*dt;
		speed.y = speed.y*dt;
		//speed = Collider_Overlay(speed, 1);

		FlipImage();
		position.x += speed.x;
		position.y += speed.y;
		Current_Animation->speed = animationSpeed*dt;



	return true;
}

bool FlyingFurrball::Start()
{
	return true;
}

bool FlyingFurrball::Move()
{
	
	if (chasing_player)
	{
  		iPoint pos = App->map->WorldToMap(position.x, position.y);
		speed.x = currentPathtile.x - pos.x;
		speed.y = currentPathtile.y - pos.y;


		speed.x *= maxSpeed.x;
		speed.y *= maxSpeed.y;
	}
	else
		speed = { 0,0 };

	if (position.x < target->position.x)
	{
		direction_x = 1;
	}
	else if(position.x > target->position.x)
	{
		direction_x = -1;
	}
	return true;
}



void FlyingFurrball::LoadPushbacks()
{
	idle.PushBack({ 732,575,53,53 });
	idle.PushBack({ 796,575,53,53 });
	idle.PushBack({ 860,575,53,53 });
	idle.loop = true;
	idle.speed = 0.3;

	moving.PushBack({ 732, 633,53,53});
	moving.PushBack({ 796, 633,53,53 });
	moving.loop = true;
	moving.speed = 0.3;
}

void FlyingFurrball::CleanUp()
{
}
