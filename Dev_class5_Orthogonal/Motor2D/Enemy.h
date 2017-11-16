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
	Entity* GetTarget();
	bool Move();
private:

public:
	float accumulated_time;
	float update_ms_cycle = 0;
	p2DynArray<iPoint> path;
	bool DoPathfinding = false;
	Entity* target = nullptr;
	iPoint currentPathtile;
	int pathIndex;

	
private:



};

