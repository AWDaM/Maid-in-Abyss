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

	/*accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
	{ 
		DoPathfinding = true;
		accumulated_time = 0.0f;
	}*/
	if (chasing_player)
	{
		Current_Animation = &moving;
		//if (DoPathfinding)
		//{
			//DoPathfinding = false;
			App->pathfinding->CreatePath(App->map->WorldToMap(position.x,position.y), App->map->WorldToMap(target->position.x,target->position.y));
			path = *App->pathfinding->GetLastPath();
		//}
	}
	else
	{
		Current_Animation = &idle;
		speed = { 0, 0 };
	}
	PositionCollider();
	position.x += speed.x*dt;
	position.y += speed.y*dt;
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
		App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
	}
	return true;
}

bool FlyingFurrball::Start()
{
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
