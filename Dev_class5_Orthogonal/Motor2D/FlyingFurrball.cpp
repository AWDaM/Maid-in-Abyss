#include "FlyingFurrball.h"



FlyingFurrball::FlyingFurrball() : Enemy(entityType::FLYING_ENEMY)
{
	type = FLYING_ENEMY;
}

FlyingFurrball::FlyingFurrball(iPoint position) : Enemy(entityType::FLYING_ENEMY, position)
{
	type = FLYING_ENEMY;
	LoadPushbacks();
	Current_Animation = &idle;
}


FlyingFurrball::~FlyingFurrball()
{
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
