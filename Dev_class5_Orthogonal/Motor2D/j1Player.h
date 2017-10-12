#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Module.h"

struct SDL_Texture;

struct PlayerData
{
	iPoint			speed;
	iPoint			accel;
	iPoint			position;
	SDL_Rect		collider;
	Animation		idle;
	Animation		running;
	Animation		jumping;
	Animation		dashing;
	SDL_Texture*	Marisa;
};

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called each loop iteration
	void Draw();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Called before quitting
	bool CleanUp();

private:
	
	PlayerData Player;

};

#endif // __j1MAP_H__