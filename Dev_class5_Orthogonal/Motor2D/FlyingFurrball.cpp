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
			if (App->pathfinding->CreatePath(App->map->WorldToMap(position.x + colOffset.x, position.y + colOffset.y), App->map->WorldToMap(target->position.x + target->colOffset.x, target->position.y + target->colOffset.y),true) != -1)
			{
				path = *App->pathfinding->GetLastPath();
				pathIndex = path.Count() - 1;
				currentPathtile = *path.At(pathIndex);
			}
			else
				speed = { 0,0 };
		}
	}
	else
	{
		Current_Animation = &idle;
		//speed = { 0, 0 };
	}
	if (dt < 1)
	{
		speed = { 0,30 };
		PositionCollider();
		Move();
		speed = Collider_Overlay(speed, dt);
		speed.x = (int)speed.x;
		speed.y = (int)speed.y;
		position.x += speed.x*dt;
		position.y += speed.y*dt;
	}/*for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
		App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
	}*/
	return true;
}

bool FlyingFurrball::Start()
{
	return true;
}

bool FlyingFurrball::Move()
{
	/*if (chasing_player)
	{
  		iPoint pos = App->map->WorldToMap(position.x, position.y);
		iPoint objective = currentPathtile;
		speed.x = currentPathtile.x - pos.x;
		speed.y = currentPathtile.y - pos.y;
		speed.Normalize();
		if (speed.x == 0 && speed.y == 0)
		{
			pathIndex--;
			currentPathtile = *path.At(pathIndex);
		}
		speed.x *= 100;
		speed.y *= 100;
	}
	else
		speed = { 0,0 };*/
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
