#include "j1EntityController.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1SceneSwitch.h"
#include "j1SceneChange.h"
#include "Entity.h"
#include "Player.h"
#include "LandMaid.h"
#include "FlyingFurrball.h"
#include "PickUp.h"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"
#include "j1Gui.h"
#include "UIClock.h"
#include "LabelledImage.h"

j1EntityController::j1EntityController()
{
	name.create("entitycontroller");
}

j1EntityController::~j1EntityController()
{
}

bool j1EntityController::Awake(pugi::xml_node &config)
{
	bool ret = true;
	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();
	totaltimestop = config.child("totaltimestop").attribute("value").as_int();
	totaltimeslow = config.child("totaltimeslow").attribute("value").as_int();
	timestop_cooldown = config.child("timestop_cooldown").attribute("value").as_int();

	time_state = NORMAL;
	
	return ret;
}

bool j1EntityController::Start()
{
	bool ret = true;
	texture = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));

	return ret;
}

bool j1EntityController::Update(float dt)
{
	if (App->scene->pause)
		return true;

	BROFILER_CATEGORY("Entites update", Profiler::Color::Maroon);
	if (App->map->debug)
	{
		DebugDraw();
	}
	enemy_dt = TimeManager(enemy_dt,dt);

	EnemyColliderCheck();

	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::PLAYER)
			ret = tmp->data->Update(dt);
		else
			ret = tmp->data->Update(enemy_dt);
		tmp = tmp->next;
	}

	//if (App->map->debug)DebugDraw();

	return ret;
}

bool j1EntityController::PostUpdate()
{
	if (App->scene->pause)
		return true;

	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::CleanUp()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Save(pugi::xml_node& file) const
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Save( file);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Load(pugi::xml_node& file)
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	pugi::xml_node furrballFile = file.child("flyingfurrball");
	pugi::xml_node landMaid = file.child("landmaid");
	pugi::xml_node pickup = file.child("pickup");
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::PLAYER)
		{
			tmp->data->Load(file.child("player"));
		}
		else if (tmp->data->type == Entity::entityType::FLYING_ENEMY)
		{
			tmp->data->Load(furrballFile);
			furrballFile = furrballFile.next_sibling("flyingfurrball");
			
		}
		else if (tmp->data->type == Entity::entityType::LAND_ENEMY)
		{
			tmp->data->Load(landMaid);
			landMaid = landMaid.next_sibling("landmaid");
		}
		else if (tmp->data->type == Entity::entityType::PICKUP)
		{
			tmp->data->Load(pickup);
			pickup = pickup.next_sibling("pickup");
		}
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Restart()
{
	bool ret = true;
	time_state = NORMAL;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Restart();
		tmp = tmp->next;
	}
	return ret;
}

void j1EntityController::DeleteEnemies()
{
	p2List_item<Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
		p2List_item<Entity*>* tmp2 = tmp;
		if (tmp->data->type != Entity::entityType::PLAYER)
		{
			RELEASE(tmp->data);
			Entities.del(tmp2);
			tmp = tmp->prev;
		}
		else
			tmp = tmp->prev;

	}
}

void j1EntityController::DeleteEntities()
{
	p2List_item<Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
	p2List_item<Entity*>* tmp2 = tmp;
		RELEASE(tmp->data);
		Entities.del(tmp2);
		tmp = tmp->prev;
	}

}

bool j1EntityController::Draw(float dt)
{
	BROFILER_CATEGORY("Entity draw", Profiler::Color::Chocolate);
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Draw(dt);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::DebugDraw()
{
	p2List_item<Entity*>* tmp = Entities.start;
	SDL_Rect col;
	SDL_Rect col2;
	while (tmp != nullptr)
	{
		col.h = tmp->data->Collider.h, col.w = tmp->data->Collider.w, col.x = tmp->data->Collider.x, col.y = tmp->data->Collider.y;
		App->render->DrawQuad(col, 255, 0, 0, 50);
		if (tmp->data->type == Entity::entityType::FLYING_ENEMY || tmp->data->type == Entity::entityType::LAND_ENEMY)
		{
			col2.h = tmp->data->SightCollider.h, col2.w = tmp->data->SightCollider.w, col2.x = tmp->data->SightCollider.x, col2.y = tmp->data->SightCollider.y;
			App->render->DrawQuad(col2, 255, 0, 0, 50);
		}
		tmp = tmp->next;
	}

	return true;
}

Entity* j1EntityController::AddEntity(Entity::entityType type, iPoint position)
{
	Entity* tmp = nullptr;

	switch (type)
	{
	case Entity::entityType::PLAYER:
		tmp = new Player();
		break;
	case Entity::entityType::FLYING_ENEMY:
		tmp = new FlyingFurrball(position);
		break;
	case Entity::entityType::LAND_ENEMY:
		tmp = new LandMaid(position);
		break;
	case Entity::entityType::PICKUP:
		tmp = new PickUp(position);
		break;
	}

	
if (tmp)
		Entities.add(tmp);

	return tmp;
}

bool j1EntityController::DeleteEntity(Entity * entity)
{
	entity->CleanUp();
	Entities.del(Entities.At(Entities.find(entity)));
	return true;
}


void j1EntityController::EnemyColliderCheck()
{
	p2List_item<Entity*>* player =  nullptr;
	for (p2List_item<Entity*>* i = Entities.start; i != nullptr; i = i->next)
	{
		if (i->data->type == Entity::entityType::PLAYER)
		{
			player = i;
			break;
		}
	}
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type != Entity::entityType::PLAYER &&tmp->data->type != Entity::entityType::PICKUP)
		{
			if (SDL_HasIntersection(&tmp->data->SightCollider, &player->data->Collider))
			{
				tmp->data->chasing_player = true;
			}
			else
			{
				tmp->data->chasing_player = false;
			}
			if (SDL_HasIntersection(&tmp->data->Collider, &player->data->Collider) && !godmode && !App->scenechange->IsChanging() && !App->sceneswitch->IsSwitching())
			{
				player->data->AddSFX(4, 0, 50);

				player->data->isDying = true;
			}
		}

		else if (tmp->data->type == Entity::entityType::PICKUP)
		{
			if (SDL_HasIntersection(&tmp->data->Collider, &player->data->Collider))
			{
				player->data->AddSFX(8, 0);
				App->gui->AddScore(tmp->data->points);
				p2SString temp("Score: %i", App->gui->scoreNumber);
				App->gui->currentScore->ChangeText(&temp);

				App->gui->coins++;
				p2SString temo("Coins: %i", App->gui->coins);
				App->gui->currentCoins->ChangeText(&temo);
				DeleteEntity(tmp->data);
			}
			//add the score and make the pickup disapear
		}
		tmp = tmp->next;
	}
}

float j1EntityController::TimeManager(float enemy_dt, float dt)
{
	switch (time_state)
	{
	case NORMAL:
		enemy_dt = dt;
		if (wanttostop && time_state != RECENTLY_STOPPED)
		{
			time_state = SLOWING;
			App->gui->clock->HandleAnimation(time_state);
			wanttostop = false;
			timestop_timer.Start();
		}

	break;

	case SLOWING:
		LOG("slowing during %f secs", timestop_timer.ReadSec());
		enemy_dt -= (enemy_dt / 10);
		if (enemy_dt < 0)
		{
			enemy_dt = 0;
		}
		if (timestop_timer.ReadSec() > totaltimeslow)
		{
			time_state = STOPPED;
			App->gui->clock->HandleAnimation(time_state);
			timestop_timer.Start();
		}

	break;

	case STOPPED:
		LOG("stopped during %f secs", timestop_timer.ReadSec());
		enemy_dt = 0;
		if (timestop_timer.ReadSec() > totaltimestop)
		{
			time_state = FASTENING;
			App->gui->clock->HandleAnimation(time_state);
			timestop_timer.Start();
			enemy_dt = dt / 10;
		}
		
		break;

	case FASTENING:
		LOG("fastening during %f secs", timestop_timer.ReadSec());
		enemy_dt += (enemy_dt / 20);
		if (enemy_dt > dt)
		{
			enemy_dt = dt;
		}
		if (timestop_timer.ReadSec() > totaltimeslow)
		{
			time_state = RECENTLY_STOPPED;
			App->gui->clock->HandleAnimation(time_state);
			timestop_timer.Start();
		}

		break;

	case RECENTLY_STOPPED:
		LOG("in cooldown during %f secs", timestop_timer.ReadSec());
		enemy_dt = dt;
		if (timestop_timer.ReadSec() > timestop_cooldown)
		{
			time_state = NORMAL;
			App->gui->clock->HandleAnimation(time_state);
			LOG("back to normal");
		}

		break;
	}
	return enemy_dt;
}

