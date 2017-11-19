#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
struct SDL_Texture;

class Enemy : public Entity
{
public:
	Enemy(Entity::entityType type);
	Enemy(Entity::entityType type, iPoint position);
	~Enemy();
	Entity* GetTarget() const;
	void Restart();
	void Save(pugi::xml_node&) const;
	void Load(pugi::xml_node&);
private:

public:
	iPoint originalpos;
	float accumulated_time;
	float update_ms_cycle = 0;
	p2DynArray<iPoint> path;
	bool DoPathfinding = false;
	Entity* target = nullptr;
	iPoint currentPathtile = { 0,0 };
	int pathIndex;
	bool canFly;
private:



};
#endif
