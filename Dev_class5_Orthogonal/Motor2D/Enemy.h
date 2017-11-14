#include "Entity.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:
	Enemy(Entity::entityType type);
	Enemy(Entity::entityType type, iPoint position);
	void Draw();
	~Enemy();

public:
	Animation* Current_Animation = nullptr;
};

