#include "Enemy.h"

class FlyingFurrball : public Enemy
{
public:
	FlyingFurrball();
	~FlyingFurrball();

private:

	void LoadPushbacks();

private:

	Animation idle;
	Animation moving;
	Animation attacking;
};

