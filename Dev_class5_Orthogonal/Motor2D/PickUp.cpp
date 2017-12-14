#include "PickUp.h"
#include "j1Render.h"


PickUp::PickUp()
{
}

PickUp::PickUp(iPoint position) : Entity(entityType::PICKUP)
{
	this->position = position;
	LoadPushbacks();
	Current_Animation = &idle;
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	config = config.child("entitycontroller").child("pickup");

	points = config.child("points").attribute("value").as_int();
	Collider.x = position.x;
	Collider.y = position.y;
	Collider.w = config.child("collider").attribute("w").as_int();
	Collider.h = config.child("collider").attribute("h").as_int();
}


PickUp::~PickUp()
{
}

void PickUp::LoadPushbacks()
{
	idle.PushBack({ 0,1015,14,14 });
	idle.PushBack({ 16,1015,14,14 });
	idle.PushBack({ 32,1015,14,14 });
	idle.speed = 15.0f;
	idle.loop = true;
}

bool PickUp::Update(float dt)
{
	return true;
}

