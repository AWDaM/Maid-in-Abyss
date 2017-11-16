#include "j1App.h"
#include "Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1SceneChange.h"
#include "j1Scene.h"
#include "j1EntityController.h"

Player::Player() : Entity(entityType::PLAYER)
{
}


Player::~Player()
{
}

bool Player::Awake(pugi::xml_node & config)
{
	bool ret = true;
	config = config.child("player");

	

	jumpFX = config.child("jumpFX").attribute("source").as_string();
	deathFX = config.child("deathFX").attribute("source").as_string();
	landFX = config.child("landFX").attribute("source").as_string();
	dashFX = config.child("dashFX").attribute("source").as_string();

	jumpForce.x = config.child("jumpForce").attribute("x").as_int();
	jumpForce.y = config.child("jumpForce").attribute("y").as_int();

	maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();

	dashingSpeed.x = config.child("dashingSpeed").attribute("x").as_int();
	dashingSpeed.y = config.child("dashingSpeed").attribute("y").as_int();

	Dashtime = config.child("dashtime").attribute("value").as_int();
	gravity = config.child("gravity").attribute("value").as_int();

	colOffset.x = config.child("colOffset").attribute("x").as_int();
	colOffset.y = config.child("colOffset").attribute("y").as_int();

	animationSpeed = 5;

	SightCollider.x = 0;
	SightCollider.y = 0;
	SightCollider.w = 1;
	SightCollider.h = 1;
	return ret;
}

bool Player::Start()
{
	LoadPushbacks();

	speed = { 0,0 };

	App->audio->LoadFx(jumpFX.GetString());
	App->audio->LoadFx(deathFX.GetString());
	App->audio->LoadFx(landFX.GetString());
	App->audio->LoadFx(dashFX.GetString());

	alive = true;

	isJumping = false;
	isDashing = false;
	canDash = true;

	Current_Animation = &idle;

	//Sets the player in the start position
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (objdata->data->name == 6)
				{
					Collider.h = objdata->data->height;
					Collider.w = objdata->data->width;
					Collider.x = objdata->data->x;
					Collider.y = objdata->data->y;
				}
				else if (objdata->data->name == 4)
				{
					position = { objdata->data->x, objdata->data->y };
					Collider.x = position.x + colOffset.x;
					Collider.y = position.y + colOffset.y;
				}
			}
		}
	}

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	App->scene->test = position;
	if (dt < 1)
	{
		FlipImage();

		if (!isDashing && canDash)
		{
			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
				StartDashing(dt);
		}
		else if (isDashing)
		{

			if (dashtimer.Read() >= Dashtime || App->input->GetKey(SDL_SCANCODE_X) == KEY_UP)
			{
				StopDashing();
			}
		}
		if (!isDashing)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				direction_x = 1;
				speed.x = maxSpeed.x*dt*direction_x*App->scene->timeScale;
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				direction_x = -1;
				speed.x = maxSpeed.x*dt*direction_x*App->scene->timeScale;
			}
			else
				speed.x = 0;


			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
			{
				AddSFX(1, 0);
				isJumping = true;
				grounded = false;
				//maxSpeed.x += jumpForce.x;
				//speed.x = jumpForce.x*direction_x*dt;
				speed.y = jumpForce.y;
			}

			speed.y += gravity*dt*App->scene->timeScale;
		}
		//speed.y = speed.y*dt;
		//speed.x = speed.x*dt;

		float meh = 1;
		speed = SpeedBoundaries(speed, meh);


		speed = Collider_Overlay(speed, meh);
		speed.x = (int)speed.x;
		speed.y = (int)speed.y;

		NormalizeAnimationSpeed(dt);

		ChangeAnimation();
		PlayerMovement(meh);
		PositionCollider();
	}

	return true;
}

bool Player::PostUpdate()
{
	if (!alive)
	{
		AddSFX(2, 0, 70);
		App->scenechange->ChangeMap(App->scene->currentMap, App->scene->fade_time);
	}

	PositionCameraOnPlayer();

	return true;
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

void Player::PlayerMovement(float dt)
{
	/*position += speed*dt;*/
	position.x += speed.x*dt*App->scene->timeScale;
	position.y += speed.y*dt*App->scene->timeScale;
	//LOG("HEYLISTENHERE %f", speed.x);
	
}

void Player::Restart()
{
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
		if (obj->data->name == ("Collisions"))
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
				if (objdata->data->name == 4)
				{
					position = { objdata->data->x, objdata->data->y };
					Collider.x = position.x + colOffset.x;
					Collider.y = position.y + colOffset.y;
					speed.x = 0;
					speed.y = 0;
					Current_Animation = &idle;
				}
	LOG("Ded");

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

void Player::LoadPushbacks()
{
	idle.PushBack({ 5, 17, 56, 73 });

	running.PushBack({ 89, 17, 60, 73 });
	running.PushBack({ 180, 17, 60, 73 });
	running.PushBack({ 277, 17, 60, 73 });
	running.PushBack({ 375, 17, 60, 73 });
	running.PushBack({ 470, 17, 60, 73 });
	running.PushBack({ 565, 17, 60, 73 });
	running.loop = true;
	running.speed = 0.25f;

	jumping_up.PushBack({ 672, 27, 53, 63 });
	jumping_up.PushBack({ 764, 0, 49, 75 });
	jumping_up.loop = false;
	jumping_up.speed = 0.5f;

	falling.PushBack({ 861, 17, 53, 73 });

	dashing.PushBack({ 635, 224, 79, 71 });
	dashing.PushBack({ 741, 226, 81, 69 });
	dashing.PushBack({ 834, 227, 82, 68 });
	dashing.PushBack({ 929, 228, 82, 67 });
	dashing.PushBack({ 390, 223, 73, 72 });
	dashing.PushBack({ 468, 219, 76, 69 });
	dashing.PushBack({ 548, 219, 76, 69 });
	dashing.loop = false;
	dashing.speed = 0.6f;
}



void Player::CleanUp()
{
	LOG("Deleting player");

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

void Player::StartDashing(float dt)
{
	AddSFX(4, 0);
	isDashing = true;
	canDash = false;
	speed.x = dashingSpeed.x * direction_x*dt*App->scene->timeScale;
	speed.y = dashingSpeed.y*dt*App->scene->timeScale;
	dashtimer.Start();
}

void Player::StopDashing()
{
	isDashing = false;
	dashing.Reset();
}
