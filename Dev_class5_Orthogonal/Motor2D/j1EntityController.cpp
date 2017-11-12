#include "j1EntityController.h"
#include "Entity.h"

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
