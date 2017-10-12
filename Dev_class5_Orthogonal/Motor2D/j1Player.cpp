#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"

j1Player::j1Player() : j1Module()
{
	
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	Player.Marisa = App->tex->Load(config.child("sprite_sheet").attribute("source").as_string());


	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

void j1Player::Draw()
{
	
}


// Called before quitting
bool j1Player::CleanUp()
{
	
	return true;
}

// Load Game State
bool j1Player::Load(pugi::xml_node& data)
{
	/*camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();*/

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	/*pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;
*/
	return true;
}
