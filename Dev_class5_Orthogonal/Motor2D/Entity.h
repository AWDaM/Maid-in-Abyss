#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"


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
		PICKUP
	};

public:
	Entity();
	Entity(entityType type);
	~Entity();
	virtual bool Awake(pugi::xml_node & config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool Move() { return true; };

	virtual void CleanUp() {};
	virtual void Save(pugi::xml_node& file) const{};
	virtual void Load(pugi::xml_node& file) {};
	virtual void ChangeAnimation() {};
	virtual void BecomeGrounded() {};
	virtual void Restart() {};
	virtual void LoadPushbacks() {};

	//Applies the gravity (speed.y) and checks if the speed is out of the limits
	fPoint SpeedBoundaries(fPoint originalvec);

	//Detects if the player's collider is going to collide in the next frame and acts in consequence
	fPoint Collider_Overlay(fPoint originalvec);

	//Called if the player's collider is going to collide with unpasable terrain, adjousts the player's speed to avoid it
	fPoint AvoidCollision(fPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata);

	//Returns the rect of the Object passed using its data
	SDL_Rect CreateRect_FromObjData(ObjectsData* data);

	//Plays the indicated sfx
	void AddSFX(int channel, int repeat, uint volume = 128);

	void NormalizeAnimationSpeed(float dt);

	void FlipImage();

	void PositionCollider();

	void Draw(float dt);

private:

public:
	int points;
	float animationSpeed = 3;
	Animation* Current_Animation = nullptr;
	entityType type;
	fPoint speed;
	iPoint position;
	iPoint maxSpeed;
	float gravity;
	int direction_x = 1; //-1 if going left, 1 if going right
	iPoint colOffset;
	iPoint sightOffset;
	SDL_Rect Collider;
	SDL_Rect SightCollider;
	bool alive = false;
	bool isDying = false;
	bool grounded = false;
	bool flip = false;
	bool chasing_player = false;
private:
	
};

#endif