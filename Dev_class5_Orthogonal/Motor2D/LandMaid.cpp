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
