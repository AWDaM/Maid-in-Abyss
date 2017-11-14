#ifndef __j1ENTITYCONTROLLER_H__
#define __j1ENTITYCONTROLLER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Player.h"
//#include "Entity.h"


class Entity;


class j1EntityController : public j1Module
{
	//------------Methods-----------//
public:
	j1EntityController();
	~j1EntityController();

	bool Awake(pugi::xml_node & config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Save();
	bool Load();
	bool Restart();
	bool Draw();
	bool DebugDraw();

	Entity* AddEntity(Entity::entityType type);
	bool DeleteEntity();
private:

	//------------Attributes-----------//
public:

private:
	p2List<Entity*> Entities;

};



#endif // !

