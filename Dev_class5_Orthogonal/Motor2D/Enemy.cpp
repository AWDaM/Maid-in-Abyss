#include "j1App.h"
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
	originalpos.x = pos.x;
	originalpos.y = pos.y;

	//pugi::xml_document	config_file;
	//pugi::xml_node		config;

	//config = App->LoadConfig(config_file);
	//if (type == Entity::entityType::FLYING_ENEMY)
	//{
	//	config = config.child("entitycontroller").child("flyingfurrball");
	//}

	//speed = { 0,0 };

	//maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	//maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();
	//gravity = config.child("gravity").attribute("value").as_float();
	//direction_x = 1;
	//colOffset.x = config.child("colOffset").attribute("x").as_int();
	//colOffset.y = config.child("colOffset").attribute("y").as_int();
	//Collider.h = config.child("Collider").attribute("h").as_int();
	//Collider.w = config.child("Collider").attribute("w").as_int();
	//Collider.x = pos.x;
	//Collider.y = pos.y;
	//sightOffset.x = config.child("sighOffset").attribute("x").as_int();
	//sightOffset.y = config.child("sighOffset").attribute("y").as_int();
	//SightCollider.x = pos.x - sightOffset.x;
	//SightCollider.y = pos.y - sightOffset.y;
	//SightCollider.w = config.child("SightCollider").attribute("w").as_int();
	//SightCollider.h = config.child("SightCollider").attribute("h").as_int();
}




Enemy::~Enemy()
{
}

Entity * Enemy::GetTarget() const
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

void Enemy::Restart()
{
	position.x = originalpos.x;
	position.y = originalpos.y;
}

void Enemy::Save(pugi::xml_node &data) const
{
	
	data = data.append_child("flyingfurrball");
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;
}

void Enemy::Load(pugi::xml_node &data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
}
