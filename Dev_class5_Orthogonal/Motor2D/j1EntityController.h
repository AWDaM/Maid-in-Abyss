#ifndef __j1ENTITYCONTROLLER_H__
#define __j1ENTITYCONTROLLER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"


enum ENTITY_TYPE
{
	NO_TYPE,
	PLAYER,
	MOVING_PLATFORM,
	FLYING_ENEMY,
	GROUND_ENEMY,
};

class Entity;

class j1EntityController : public j1Module
{
	//------------Methods-----------//
public:
	j1EntityController();
	~j1EntityController();

	bool Update(float dt);
	bool CleanUp();
	bool Save();
	bool Load();

	bool AddEntity(ENTITY_TYPE type, iPoint pos);
	bool DeleteEntity();
private:

	//------------Attributes-----------//
public:

private:
	p2List<Entity> Entities;

};



#endif // !

