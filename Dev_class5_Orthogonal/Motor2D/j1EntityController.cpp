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
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data.Awake(pugi::xml_node &config);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Start()
{
	bool ret = false;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data.Start();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Update(float dt)
{
	bool ret = false;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data.Update(dt);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::CleanUp()
{
	bool ret = true;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.CleanUp();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Save()
{
	bool ret = true;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Save();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Load()
{
	bool ret = true;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Load();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Restart()
{
	bool ret = true;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Restart();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Draw()
{
	bool ret = true;
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Draw();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::AddEntity(ENTITY_TYPE type, iPoint pos)
{

	if (type = PLAYER)
	{
		Entity* tmp = new Player();
		Entities.add(*tmp);
	}

	return true;
}

bool j1EntityController::DeleteEntity()
{
	return false;
}
