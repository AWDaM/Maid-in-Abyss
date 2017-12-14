#ifndef __j1ENTITYCONTROLLER_H__
#define __j1ENTITYCONTROLLER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Player.h"

//#include "Entity.h"


class Entity;

enum timeState
{
	NO_STATE,
	NORMAL,
	SLOWING,
	STOPPED,
	FASTENING,
	RECENTLY_STOPPED
};

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
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);
	bool Restart();
	void DeleteEnemies();
	bool Draw(float dt);
	bool DebugDraw();

	Entity* AddEntity(Entity::entityType type, iPoint position);
	bool DeleteEntity(Entity* entity);
	void DeleteEntities();
	void EnemyColliderCheck();
	float TimeManager(float enemy_dt, float dt);
private:

	//------------Attributes-----------//
public:
	p2SString		folder;
	p2SString		texture_path;
	SDL_Texture* texture;
	p2List<Entity*> Entities;
	bool timestopped = false;
	bool godmode = false;
	j1Timer timestop_timer;
	int totaltimestop;
	int totaltimeslow;
	int timestop_cooldown;
	float enemy_dt;
	bool wanttostop = false;
	timeState time_state;
private:
	
protected:
	


};



#endif // !

