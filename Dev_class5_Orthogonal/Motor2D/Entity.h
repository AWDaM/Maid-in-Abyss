#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1EntityController.h"
#include "p2Log.h"
#include "j1Map.h"

class Entity
{
public:
	Entity();
	~Entity();

	virtual bool PreUpdate(float dt) { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate(float dt) { return true; };
	virtual bool Move() { return true; };
	virtual void Draw() {};
	virtual void CleanUp() {};
	virtual void Save() const{};
	virtual void Load() {};
	virtual void ChangeAnimation() {};
	virtual void BecomeGrounded() {};
	virtual void Restart() {};

	//Applies the gravity (acceleration.y) and checks if the speed is out of thee boundaries
	//Detects if the player's collider is going to collide in the next frame and acts in consequence

	//Called if the player's collider is going to collide with unpasable terrain, adjousts the player's speed to avoid it

	//Returns the rect of the Object passed using its data
	fPoint SpeedBoundaries(fPoint originalvec);
	fPoint Collider_Overlay(fPoint originalvec);
	fPoint AvoidCollision(fPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata);
	SDL_Rect CreateRect_FromObjData(ObjectsData* data);

	//Plays the indicated sfx
	void AddSFX(int channel, int repeat, uint volume = 128);

private:

public:
	SDL_Texture* texture = nullptr;
	ENTITY_TYPE Type;
	fPoint speed;
	iPoint position;
	iPoint maxSpeed;
	float gravity;
	int direction_x; //-1 if going left, 1 if going right
	iPoint colOffset;
	SDL_Rect Collider;
	bool alive = false;
private:
	
};

#endif