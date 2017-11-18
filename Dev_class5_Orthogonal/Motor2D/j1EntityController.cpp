#include "j1EntityController.h"
#include "j1App.h"
#include "j1Render.h"
#include "Entity.h"
#include "Player.h"
#include "LandMaid.h"
#include "FlyingFurrball.h"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"

j1EntityController::j1EntityController()
{
	name.create("entitycontroller");
}

j1EntityController::~j1EntityController()
{
}

bool j1EntityController::Awake(pugi::xml_node &config)
{
	bool ret = false;
	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();
	totaltimestop = config.child("totaltimestop").attribute("value").as_int();
	totaltimeslow = config.child("total").attribute("value").as_int();
	AddEntity(Entity::entityType::PLAYER, { 0,0 });
	
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Awake(config);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Start()
{
	bool ret = false;
	texture = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));

	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Start();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Update(float dt)
{
	BROFILER_CATEGORY("Entites update", Profiler::Color::Maroon);
	if (App->map->debug)
	{
		DebugDraw();
	}
	StopTime();
	EnemyColliderCheck();
	bool ret = false;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}
	//if (App->map->debug)DebugDraw();

	return ret;
}

bool j1EntityController::PostUpdate()
{
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
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Restart()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Restart();
		tmp = tmp->next;
	}
	return ret;
}

void j1EntityController::ChangeMapEnemies()
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

bool j1EntityController::Draw()
{
	BROFILER_CATEGORY("Entity draw", Profiler::Color::Chocolate);
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Draw();
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
		if (tmp->data->type == Entity::entityType::FLYING_ENEMY)
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
	}
		//case Entity::entityType::LAND_ENEMY: tmp = new Player();
	
if (tmp)
		Entities.add(tmp);

	return tmp;
}

bool j1EntityController::DeleteEntity()
{
	return false;
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
		if (tmp->data->type != Entity::entityType::PLAYER)
		{
			if (SDL_HasIntersection(&tmp->data->SightCollider, &player->data->Collider))
			{
				tmp->data->chasing_player = true;
			}
			else
			{
				tmp->data->chasing_player = false;
			}
			if (SDL_HasIntersection(&tmp->data->Collider, &player->data->Collider))
			{
				player->data->alive = false;
			}
		}
		tmp = tmp->next;
	}
}

void j1EntityController::TimeManager()
{
	if (wanttostop && time_state == NORMAL)
	{
		time_state = SLOWING;
		timeslowed_timer.Start();
	}

	else if (time_state == SLOWING && timeslowed_timer.ReadSec() > totaltimeslow)
	{
		time_state = STOPPED;
		timestopped_timer.Start();
	}
	if (timestopped_timer.ReadSec()>totaltimestop && timestopped)
	{
		timestopped = false;
	}
}

void j1EntityController::StopTime()
{
	if (wanttostop)
	{
		timestopped = true;
		wanttostop = false;
	}

	if (timestopped_timer.ReadSec()>totaltimestop && timestopped)
	{
		timestopped = false;
	}

}
