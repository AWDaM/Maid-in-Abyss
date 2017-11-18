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
	void ChangeMapEnemies();
	bool Draw();
	bool DebugDraw();

	Entity* AddEntity(Entity::entityType type, iPoint position);
	bool DeleteEntity();
	void EnemyColliderCheck();
	void TimeManager();
	void StopTime();
private:

	//------------Attributes-----------//
public:
	p2SString		folder;
	p2SString		texture_path;
	SDL_Texture* texture;
	p2List<Entity*> Entities;
	bool timestopped = false;
	j1Timer timestopped_timer;
	j1Timer timeslowed_timer;
	int totaltimestop;
	int totaltimeslow;
	bool wanttostop = false;
	timeState time_state;
private:
	
protected:
	


};



#endif // !

