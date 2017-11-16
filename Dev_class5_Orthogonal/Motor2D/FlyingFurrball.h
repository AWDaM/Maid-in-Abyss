#include "Enemy.h"

class FlyingFurrball : public Enemy
{
public:
	FlyingFurrball();
	FlyingFurrball(iPoint position);
	~FlyingFurrball();

	bool Update(float dt);
	bool Start();
	bool Move();
private:

	void LoadPushbacks();
	void CleanUp();
public:

private:
	Animation idle;
	Animation moving;
	Animation attacking;
};

