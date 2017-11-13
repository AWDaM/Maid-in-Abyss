#include "j1EntityController.h"
#include "Entity.h"
#include "Player.h"
#include "PugiXml/src/pugixml.hpp"

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
	AddEntity(Entity::entityType::PLAYER);
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
	bool ret = false;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}
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

bool j1EntityController::Save()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Save();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Load()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Load();
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

bool j1EntityController::Draw()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Draw();
		tmp = tmp->next;
	}
	return ret;
}

Entity* j1EntityController::AddEntity(Entity::entityType type)
{
	Entity* tmp = nullptr;

	switch (type)
		case Entity::entityType::PLAYER: tmp = new Player(); //break;
		//case Entity::entityType::FLYING_ENEMY: tmp = new Player(); break;
		//case Entity::entityType::LAND_ENEMY: tmp = new Player();
	
	if (tmp)
		Entities.add(tmp);

	return tmp;
}

bool j1EntityController::DeleteEntity()
{
	return false;
}
