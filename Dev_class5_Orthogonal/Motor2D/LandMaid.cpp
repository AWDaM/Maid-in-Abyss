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
}

LandMaid::~LandMaid()
{
}

void LandMaid::LoadPushbacks()
{
	idle.PushBack({ 732,575,53,53 });
	idle.PushBack({ 796,575,53,53 });
	idle.PushBack({ 860,575,53,53 });
	idle.loop = true;
	idle.speed = 0.3;

	moving.PushBack({ 732, 633,53,53 });
	moving.PushBack({ 796, 633,53,53 });
	moving.loop = true;
	moving.speed = 0.3;
}


void LandMaid::CleanUp()
{
}
