#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1EntityController.h"

struct SDL_Texture;

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

	//Applies the gravity (acceleration.y) and checks if the speed is out of thee boundaries
	iPoint SpeedBoundaries(iPoint originalvec);

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
private:
	
};

#endif