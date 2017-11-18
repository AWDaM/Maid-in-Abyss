#include "Enemy.h"

class LandMaid : public Enemy
{
public:
	LandMaid();
	LandMaid(iPoint position);
	~LandMaid();

	bool Update(float dt);
	bool Start();
	bool Move();

private:
	void LoadPushbacks();
	void CleanUp();

private:
	Animation idle;
	Animation moving;
};

