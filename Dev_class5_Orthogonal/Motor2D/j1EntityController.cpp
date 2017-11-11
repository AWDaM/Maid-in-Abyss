#include "j1EntityController.h"

bool j1EntityController::Update(float dt)
{
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Update(dt);
		tmp = tmp->next;
	}
	return true;
}

bool j1EntityController::CleanUp()
{
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.CleanUp();
		tmp = tmp->next;
	}
	return true;
}

bool j1EntityController::Save()
{
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Save();
		tmp = tmp->next;
	}
	return true;
}

bool j1EntityController::Load()
{
	p2List_item<Entity>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data.Load();
		tmp = tmp->next;
	}
	return true;
}
