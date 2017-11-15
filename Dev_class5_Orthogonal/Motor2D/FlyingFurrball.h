#include "Enemy.h"

class FlyingFurrball : public Enemy
{
public:
	FlyingFurrball();
	FlyingFurrball(iPoint position);
	~FlyingFurrball();

	bool Start();

private:

	void LoadPushbacks();

private:
	Animation idle;
	Animation moving;
	Animation attacking;
};

