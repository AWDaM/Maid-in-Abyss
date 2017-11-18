#include "LandMaid.h"



LandMaid::LandMaid() : Enemy(entityType::LAND_ENEMY)
{
	canFly = false;
}

LandMaid::LandMaid(iPoint position) : Enemy(entityType::LAND_ENEMY, position)
{
	gravity = 50;
	LoadPushbacks();
	Current_Animation = &idle;
	canFly = false;
	Collider.h = 60;
	colOffset.x = 5;
	colOffset.y = 5;
}

LandMaid::~LandMaid()
{
}

bool LandMaid::Update(float dt)
{
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
				speed.x = 0;

		}
	}
	else
	{
		Current_Animation = &idle;
		speed.x = 0;
	}

	PositionCollider();
	Move();
	//speed = Collider_Overlay(speed, dt);
	speed.x = speed.x*dt;
	speed.y = gravity*dt;
	speed = Collider_Overlay(speed, 1);
	position.x += speed.x;
	position.y += speed.y;
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
		speed.x = currentPathtile.x - pos.x;
		


		speed.x *= 100;
		
	}
	else
		speed.x = 0;
	return true;
}

void LandMaid::LoadPushbacks()
{
	idle.PushBack({ 0,575,65,50 });
	idle.PushBack({ 147,575,65,50 });
	idle.PushBack({ 223,575,65,50 });
	idle.PushBack({ 249,575,65,50 });
	idle.PushBack({ 372,575,65,50 });
	idle.PushBack({ 249,575,65,50 });
	idle.loop = true;
	idle.speed = 0.3;

	moving.PushBack({219,873,65,65 });
	moving.PushBack({ 291,873,65,65 });
	moving.PushBack({ 362,873,65,65 });
	moving.PushBack({ 435,873,65,65 });
	moving.PushBack({ 507,873,65,65 });
	moving.PushBack({ 578,873,65,65 });
	moving.loop = true;
	moving.speed = 0.3;
}


void LandMaid::CleanUp()
{
}
