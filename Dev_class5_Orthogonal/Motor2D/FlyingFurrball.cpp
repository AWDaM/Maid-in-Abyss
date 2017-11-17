#include "FlyingFurrball.h"
#include "j1Scene.h"
#include "j1Render.h"


FlyingFurrball::FlyingFurrball() : Enemy(entityType::FLYING_ENEMY)
{
}

FlyingFurrball::FlyingFurrball(iPoint position) : Enemy(entityType::FLYING_ENEMY, position)
{
	LoadPushbacks();
	Current_Animation = &idle;
	canFly = true;
}


FlyingFurrball::~FlyingFurrball()
{
}

bool FlyingFurrball::Update(float dt)
{
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


	if (chasing_player)
	{
        		Current_Animation = &moving;
		if (DoPathfinding)
		{
			DoPathfinding = false;
			if (App->pathfinding->CreatePath(App->map->WorldToMap(position.x + colOffset.x, position.y + colOffset.y), App->map->WorldToMap(target->position.x + target->colOffset.x, target->position.y + target->colOffset.y), true) > 0)
			{
				path = *App->pathfinding->GetLastPath();

				if (currentPathtile != *path.At(0))currentPathtile = *path.At(0);

				else if (path.Count() > 1)currentPathtile = *path.At(1);

				else speed = { 0,0 };
			}
			else
				speed = { 0,0 };
		}
	}
	else
	{
		Current_Animation = &idle;
		speed = { 0, 0 };
	}

		PositionCollider();
		Move();
		//speed = Collider_Overlay(speed, dt);
		speed.x = (int)speed.x;
		speed.y = (int)speed.y;
		position.x += speed.x*dt;
		position.y += speed.y*dt;

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


		speed.x *= 100;
		speed.y *= 100;
	}
	else
		speed = { 0,0 };
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
