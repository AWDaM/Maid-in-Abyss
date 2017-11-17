#include "Enemy.h"
#include "j1EntityController.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"


Enemy::Enemy(Entity::entityType type) : Entity(type)
{
}

Enemy::Enemy(Entity::entityType type, iPoint pos) : Entity(type)
{
	position.x = pos.x;
	position.y = pos.y;
	
	speed = { 0,0 };
	maxSpeed = { 300,300 };
	gravity = 0;
	direction_x = 1;
	colOffset = { 5,5 };
	Collider.h = 31;
	Collider.w = 31;
	Collider.x = pos.x;
	Collider.y = pos.y;
	sightOffset = { 400,300 };
	SightCollider.x = pos.x - sightOffset.x;
	SightCollider.y = pos.y - sightOffset.y;
	SightCollider.w = 1000;
	SightCollider.h = 650;
}




Enemy::~Enemy()
{
}

Entity * Enemy::GetTarget()
{
	p2List_item<Entity*>* ret = nullptr;
	for (p2List_item<Entity*>* i = App->entitycontroller->Entities.start; i != nullptr; i = i->next)
	{
		if (i->data->type == Entity::entityType::PLAYER)
		{
			ret = i;
			break;
		}
	}

	return ret->data;
}

bool Enemy::Move()
{

	return true;
}
