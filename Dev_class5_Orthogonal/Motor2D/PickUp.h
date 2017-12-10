#pragma once
#include "Entity.h"
class PickUp : public Entity
{
public:
	PickUp();
	PickUp(iPoint position);
	~PickUp();

	void LoadPushbacks();
private:
public:
	int points;
	Animation idle;
private:
};

