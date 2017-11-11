#include "j1App.h"
#include "Entity.h"
#include "j1Audio.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

iPoint Entity::SpeedBoundaries(iPoint originalvec)
{
	if (direction_x > 0)
	{
		if (originalvec.x > maxSpeed.x)
			originalvec.x = maxSpeed.x;
	}

	else
	{
		if (originalvec.x < direction_x*maxSpeed.x)
			originalvec.x = direction_x*maxSpeed.x;
	}

	if (originalvec.y > maxSpeed.y)
	{
		originalvec.y = maxSpeed.y;
	}
	return originalvec;
}

void Entity::AddSFX(int channel, int repeat, uint volume)
{
	App->audio->PlayFx(channel, repeat, volume);
}
