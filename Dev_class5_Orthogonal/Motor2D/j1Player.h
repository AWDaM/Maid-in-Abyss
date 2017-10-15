#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Module.h"

struct SDL_Texture;
struct ObjectsData;

struct PlayerData
{			
	bool			isJumping;
	bool			isDashing;
	bool			flip;
	int				direction_x;
	iPoint			jumpForce;
	bool			grounded;
	float			Dashtime;
	float			currentDashtime;
	float			initialDashtime;
	iPoint			colOffset;
	iPoint			speed;
	iPoint			maxSpeed;
	iPoint			dashingSpeed;
	int				dashingColliderDifference;
	iPoint			accel;
	iPoint			position;
	SDL_Rect		collider;
	Animation*		current_animation;
	Animation		idle;
	Animation		running;
	Animation		jumping_up;
	Animation		falling;
	Animation		dashing;
	SDL_Texture*	Player_tex;

	void LoadPushbacks();
};

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called each loop iteration
	void Draw();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	iPoint Overlay_avoid(iPoint originalvec);
	iPoint AvoidCollision(iPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata);

	SDL_Rect CreateRect_FromObjData(ObjectsData* data);


	// Called before quitting
	bool CleanUp();

	void FlipImage();

	void BecomeGrounded();

	void StartDashing();
	uint DashingTimer();
	void StopDashing();

	void AddSpeed();
	void ReduceSpeed();
	void ChangeAnimation();
	void PlayerMovement();
	iPoint ApplyGravity(iPoint originalvec);



	void Restart();

	bool PositionCameraOnPlayer();

	
private:

	PlayerData		Player;
	p2SString		folder;
	p2SString		texture_path;

public:

	bool			isPlayerAlive;
};

#endif // __j1MAP_H__