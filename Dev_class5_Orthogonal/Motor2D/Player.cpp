#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"


Player::Player()
{
}


Player::~Player()
{
}

bool Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	speed.x = data.child("speed").attribute("x").as_int();
	speed.y = data.child("speed").attribute("y").as_int();
	Collider.w = data.child("collider").attribute("width").as_int();
	Collider.h = data.child("collider").attribute("height").as_int();
	Collider.x = data.child("collider").attribute("x").as_int();
	Collider.y = data.child("collider").attribute("y").as_int();
	grounded = data.child("grounded").attribute("value").as_bool();
	isDashing = data.child("dashing").attribute("value").as_bool();
	currentDashtime = data.child("dashtime").attribute("value").as_float();
	return true;
}

// Save Game State
bool Player::Save(pugi::xml_node& data) const
{
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;
	data.append_child("speed").append_attribute("x") = speed.x;
	data.child("speed").append_attribute("y") = speed.y;
	data.append_child("collider").append_attribute("width") = Collider.w;
	data.child("collider").append_attribute("height") = Collider.h;
	data.child("collider").append_attribute("x") = Collider.x;
	data.child("collider").append_attribute("y") = Collider.y;
	data.append_child("grounded").append_attribute("value") = grounded;
	data.append_child("dashing").append_attribute("value") = isDashing;
	data.append_child("currentDashtime").append_attribute("value") = currentDashtime;
	return true;
}

void Player::ChangeAnimation()
{
	if (!isDashing)
	{
		if (speed.y == 0 && grounded)
			if (speed.x == 0)
				Current_Animation = &idle;
			else
				Current_Animation = &running;
		else if (speed.y < 0)
			Current_Animation = &jumping_up;
		else
			Current_Animation = &falling;
	}

	else
		Current_Animation = &dashing;
}

void Player::Restart()
{
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
		if (obj->data->name == ("Collisions"))
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
				if (objdata->data->name == ("Start"))
				{
					position = { objdata->data->x, objdata->data->y };
					Collider.x = position.x + colOffset.x;
					Collider.y = position.y + colOffset.y;
					speed.x = 0;
					speed.y = 0;
					Current_Animation = &idle;
				}
	alive = true;
}

bool Player::PositionCameraOnPlayer()
{
	App->render->camera.x = position.x - App->render->camera.w / 3;
	if (App->render->camera.x < 0)App->render->camera.x = 0;
	App->render->camera.y = position.y - App->render->camera.h / 2;
	if (App->render->camera.y + App->win->height > App->map->data.height*App->map->data.tile_height)App->render->camera.y = App->map->data.height*App->map->data.tile_height - App->win->height;
	return true;
}

void Player::BecomeGrounded()
{
	if (isJumping)
	{
		isJumping = false;
		maxSpeed.x -= jumpForce.x;
	}

	if (Current_Animation == &falling)
		AddSFX(3, 0);

	canDash = true;
	grounded = true;
	jumping_up.Reset();
}
