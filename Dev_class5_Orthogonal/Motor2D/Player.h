#pragma once
#include "Entity.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Timer.h"


struct SDL_Texture;
struct ObjectsData;


class Player : public Entity
{
	//-------------Functions-----------------
public:

	Player();

	// Destructor
	~Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	//Called before the first frame
	bool Start();

	bool Update(float dt);
	bool PostUpdate();

	// Called each loop iteration

	void Load(pugi::xml_node& file);
	void Save(pugi::xml_node& file) const;

	// Called before quitting
	void CleanUp();

	//Decides if the sprite needs to be flipped


	//When called, sets the corresponding bools and plays the sfx
	void BecomeGrounded();

	//When called, sets the player in the dash state
	void StartDashing(float dt);

	//When called, sets the corresponding bools and readjousts the player's collider
	void StopDashing();

	//Increased the speed according to the player's acceleration


	//Reduces the player's speed according to the player's acceleration

	//Changes the animation according to the player's state
	void ChangeAnimation();


	

	//Adds the speed to the player's position and checks if its out of the boundaries
	void PlayerMovement(float dt);

	
	

	//Positions the player in the start of the current level
	void Restart();

	//Centers the camera on the player and checks if it is out of the boundaries
	bool PositionCameraOnPlayer();

	void LoadPushbacks();

private:
	
public:


private:

	bool grounded = false;
	bool isDashing = false;
	bool isJumping = false;

	bool canDash = false;

	iPoint jumpForce;
	iPoint			dashingSpeed;
	int lives;
	int Dashtime;
	float currentDashtime = 0;
	Animation		idle;
	Animation		running;
	Animation		jumping_up;
	Animation		falling;
	Animation		dashing;
	Animation		dying;
	j1Timer			dashtimer;
	p2SString		jumpFX;
	p2SString		deathFX;
	p2SString		landFX;
	p2SString		dashFX;
	p2SString		pickupFX;
	p2SString		timestopFX;
	bool gravityApplied = false;


};

