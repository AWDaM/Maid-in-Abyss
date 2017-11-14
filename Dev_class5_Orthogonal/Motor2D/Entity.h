#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "j1Map.h"



class Entity
{
public:
	enum entityType
	{
		NO_TYPE,
		PLAYER,
		MOVING_PLATFORM,
		FLYING_ENEMY,
		LAND_ENEMY,
	};

public:
	Entity(entityType type);
	~Entity();
	virtual bool Awake(pugi::xml_node & config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool Move() { return true; };
	virtual void Draw() {};
	virtual void CleanUp() {};
	virtual void Save() const{};
	virtual void Load() {};
	virtual void ChangeAnimation() {};
	virtual void BecomeGrounded() {};
	virtual void Restart() {};
	virtual void LoadPushbacks() {};

	//Applies the gravity (speed.y) and checks if the speed is out of the limits
	fPoint SpeedBoundaries(fPoint originalvec, float dt);

	//Detects if the player's collider is going to collide in the next frame and acts in consequence
	fPoint Collider_Overlay(fPoint originalvec, float dt);

	//Called if the player's collider is going to collide with unpasable terrain, adjousts the player's speed to avoid it
	fPoint AvoidCollision(fPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata);

	//Returns the rect of the Object passed using its data
	SDL_Rect CreateRect_FromObjData(ObjectsData* data);

	//Plays the indicated sfx
	void AddSFX(int channel, int repeat, uint volume = 128);

	void normalizeAnimationSpeed(float dt);
private:

public:
	Animation* Current_Animation = nullptr;
	SDL_Texture* texture = nullptr;
	entityType type;
	fPoint speed;
	iPoint position;
	iPoint maxSpeed;
	float gravity;
	int direction_x = 1; //-1 if going left, 1 if going right
	iPoint colOffset;
	SDL_Rect Collider;
	bool alive = false;
	bool grounded = false;
private:
	
};

#endif