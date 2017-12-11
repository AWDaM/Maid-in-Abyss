#pragma once
#include "Entity.h"
class PickUp : public Entity
{
public:
	PickUp();
	PickUp(iPoint position);
	~PickUp();

	void LoadPushbacks();

	bool Update(float dt);


private:
public:

	Animation idle;
private:
};

