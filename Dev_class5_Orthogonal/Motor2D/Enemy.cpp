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
	Collider.h = 32;
	Collider.w = 32;
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

void Enemy::Save(pugi::xml_node &data) const
{
	
	data = data.append_child("FlyingFurrball");
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;
}

void Enemy::Load(pugi::xml_node &data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
}
