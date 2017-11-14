#include "FlyingFurrball.h"



FlyingFurrball::FlyingFurrball() : Enemy(entityType::FLYING_ENEMY)
{
	type = FLYING_ENEMY;

}


FlyingFurrball::~FlyingFurrball()
{
}

void FlyingFurrball::LoadPushbacks()
{
	idle.PushBack({ 732,575,50,50 });
	idle.PushBack({ 796,575,50,50 });
	idle.PushBack({ 860,575,50,50 });
	idle.loop = true;
	idle.speed = 0.3;

	moving.PushBack({ 732, 633,50,50});
	moving.PushBack({ 796, 633,50,50 });
	moving.loop = true;
	moving.speed = 0.3;
}
