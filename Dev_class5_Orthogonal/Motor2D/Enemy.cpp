#include "j1App.h"
#include "Enemy.h"
#include "j1EntityController.h"
#include "j1Textures.h"
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

void Enemy::Restart()
{
	position.x = originalpos.x;
	position.y = originalpos.y;
	speed.SetToZero();

}

void Enemy::Save(pugi::xml_node &data) const
{
	if (type == FLYING_ENEMY)
	{
		pugi::xml_node furrball = data.append_child("flyingfurrball");;
		furrball.append_child("position").append_attribute("x") = position.x;
		furrball.child("position").append_attribute("y") = position.y;
	}
	else if (type == LAND_ENEMY)
	{
	pugi::xml_node landmaid = data.append_child("landmaid");;
	landmaid.append_child("position").append_attribute("x") = position.x;
	landmaid.child("position").append_attribute("y") = position.y;
	}

}

void Enemy::Load(pugi::xml_node &data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
}
