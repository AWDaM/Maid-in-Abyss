#include "Enemy.h"

class LandMaid : public Enemy
{
public:
	LandMaid();
	LandMaid(iPoint position);
	~LandMaid();

private:
	void LoadPushbacks();
	void CleanUp();

private:
	Animation idle;
	Animation moving;
};

