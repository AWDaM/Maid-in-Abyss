#include "FlyingFurrball.h"



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
	if (chasing_player)
	{
		Current_Animation = &moving;
		speed = { 100,100 };
	}
	else
	{
		Current_Animation = &idle;
		speed = { 0, 0 };
	}
	PositionCollider();
	position.x += speed.x*dt;
	position.y += speed.y*dt;
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
