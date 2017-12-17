#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "Window.h"
#include "j1Map.h"
#include "LabelledImage.h"
#include "j1Scene.h"
#include "j1IntroScene.h"
#include "j1SceneChange.h"
#include "j1Sceneswitch.h"
#include "j1Pathfinding.h"
#include "j1EntityController.h"
#include "j1Gui.h"
#include "LifeBar.h"
#include "UIClock.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	fade_time = config.child("fade_time").attribute("value").as_float();

	for (pugi::xml_node map = config.child("map_name"); map; map = map.next_sibling("map_name"))
	{
		p2SString* data = new p2SString;

		data->create(map.attribute("name").as_string());
		map_names.add(data);
	}
	
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	to_end = false;
	bool ret = App->map->Load_map(map_names.start->data->GetString());
	App->audio->PlayMusic(App->map->data.musicFile.GetString());

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	debug_tex = App->tex->Load("maps/Navigable.png");

	currentMap = 0;
	Entity* player = App->entitycontroller->AddEntity(Entity::entityType::PLAYER, { 0,0 });
	player->Awake(config.child(App->entitycontroller->name.GetString()));

	player->Start();

	SpawnEnemies();

	pugi::xml_document	Gui_config_file;
	pugi::xml_node		guiconfig;

	guiconfig = App->LoadConfig(Gui_config_file, "Gui_config.xml");

	guiconfig = guiconfig.child("scene");

	App->gui->Load_UIElements(guiconfig, this);
	App->gui->Load_SceneWindows(guiconfig, this);

	transcurredTime.Start();

	
	return true;

}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (transcurredTime.ReadSec() >= 1 && !pause && !App->sceneswitch->IsSwitching() && !App->scenechange->IsChanging()&& !App->gui->clock->TimeIsModified())
	{
		currentTime += 1;
		if (App->gui->scoreNumber > 0)
		{
			App->gui->scoreNumber -= 10;
			p2SString temp("Score: %i", App->gui->scoreNumber);
			App->gui->currentScore->ChangeText(&temp);
		}
		p2SString temp("Time: %i", currentTime);
		LOG("IsChanging: %i", App->scenechange->IsChanging());
		App->gui->timeLabel->ChangeText(&temp);
		transcurredTime.Start();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene update", Profiler::Color::Black);
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Load_lvl(0) , currentMap = 0;

	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->entitycontroller->Restart();

	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		Load_lvl(1) ,  currentMap = 1;

	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame();

	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->map->debug = !App->map->debug;

	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->entitycontroller->godmode = !App->entitycontroller->godmode;

	else if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->fpsCapON = !App->fpsCapON;



	App->map->Draw();
	App->entitycontroller->Draw(dt);
	App->gui->Draw(dt);



	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (to_end && !App->scenechange->IsChanging())
	{
		App->entitycontroller->DeleteEnemies();
		
		if(currentMap < map_names.count() - 1)
			ret = App->scenechange->ChangeMap(++currentMap, fade_time);
		else
			currentMap = 0, ret = App->scenechange->ChangeMap(currentMap, fade_time);


		to_end = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		pause = !pause;
		if (pause)
			OpenPauseMenu();
		else
			ClosePauseMenu();
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	currentTime = 0;
	LOG("Freeing scene");

	return true;
}

bool j1Scene::OnEvent(UIElement* element, int eventType)
{
	bool ret = true;

	element->HandleAnimation(eventType);

	if (eventType == EventTypes::LEFT_MOUSE_PRESSED)
	{
		App->audio->PlayFx(1);
	}

	if (eventType == EventTypes::PRESSED_ENTER && element->type == InteractiveType::CLOSE_WINDOW)
	{
		ret = element->OnEvent();
		pause = !pause;
	}

	if (element->type == InteractiveType::CLOSE_WINDOW && eventType == EventTypes::LEFT_MOUSE_PRESSED)
		pause = false, ClosePauseMenu(), App->gui->UIunlocked = !App->gui->UIunlocked;

	if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == InteractiveType::EXIT_TO_MENU)
	{
		pause = false;
		App->sceneswitch->SwitchScene(App->introscene, App->scene);
	}

	if(eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == InteractiveType::SAVE_GAME)
	{
		App->SaveGame();
	}

	if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == InteractiveType::UNLOCKUI)
	{
		App->gui->UnlockUI();
		App->gui->UIunlocked = !App->gui->UIunlocked;
	}
	return ret;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	currentTime = data.child("time").attribute("value").as_int();
	p2SString temp("Time: %i", currentTime);
	LOG("IsChanging: %i", App->scenechange->IsChanging());
	App->gui->timeLabel->ChangeText(&temp);

	pugi::xml_node tmp = data.child("uipos");
	App->gui->clock->position.x = tmp.child("uiclock").attribute("x").as_int();
	App->gui->clock->position.y = tmp.child("uiclock").attribute("y").as_int();
	App->gui->timeLabel->position.x = tmp.child("timer").attribute("x").as_int();
	App->gui->timeLabel->position.y = tmp.child("timer").attribute("y").as_int();
	App->gui->currentCoins->position.x = tmp.child("coins").attribute("x").as_int();
	App->gui->currentCoins->position.y = tmp.child("coins").attribute("y").as_int();
	App->gui->currentScore->position.x = tmp.child("score").attribute("x").as_int();
	App->gui->currentScore->position.y = tmp.child("score").attribute("y").as_int();
	lifebar->position.x = tmp.child("lifebar").attribute("x").as_int();
	lifebar->position.y = tmp.child("lifebar").attribute("y").as_int();

	if (currentMap != data.child("currentMap").attribute("num").as_int())
	{
		LOG("Calling switch maps");
		currentMap = data.child("currentMap").attribute("num").as_int();
		App->map->SwitchMaps(map_names[data.child("currentMap").attribute("num").as_int()]);

	}
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	data.append_child("currentMap").append_attribute("num") = currentMap;
	data.append_child("time").append_attribute("value") = currentTime;

	data.append_child("uipos");
	data.child("uipos").append_child("uiclock").append_attribute("x") = App->gui->clock->position.x;
	data.child("uipos").child("uiclock").append_attribute("y") = App->gui->clock->position.y;
	data.child("uipos").append_child("lifebar").append_attribute("x") = lifebar->position.x;
	data.child("uipos").child("lifebar").append_attribute("y") = lifebar->position.y;
	data.child("uipos").append_child("timer").append_attribute("x") = App->gui->timeLabel->position.x;
	data.child("uipos").child("timer").append_attribute("y") = App->gui->timeLabel->position.y;
	data.child("uipos").append_child("coins").append_attribute("x") = App->gui->currentCoins->position.x;
	data.child("uipos").child("coins").append_attribute("y") = App->gui->currentCoins->position.y;
	data.child("uipos").append_child("score").append_attribute("x") = App->gui->currentScore->position.x;
	data.child("uipos").child("score").append_attribute("y") = App->gui->currentScore->position.y;

	return true;
}

bool j1Scene::Load_lvl(int time)
{
	App->map->SwitchMaps(map_names[time]);
	App->entitycontroller->DeleteEnemies();
	App->entitycontroller->Restart();
	App->scene->SpawnEnemies();
	return true;
}

void j1Scene::OpenPauseMenu()
{
	if(sceneMenu)
	sceneMenu->active = true;
}

void j1Scene::ClosePauseMenu()
{
	if (sceneMenu)
	sceneMenu->active = false;
}

void j1Scene::SpawnEnemies()
{
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Enemies"))
		{
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (objdata->data->name == 7)
				{
					App->entitycontroller->AddEntity(Entity::entityType::FLYING_ENEMY, { objdata->data->x,objdata->data->y });
				}

				else if (objdata->data->name == 8)
				{
					App->entitycontroller->AddEntity(Entity::entityType::LAND_ENEMY, { objdata->data->x,objdata->data->y });
				}

				else if (objdata->data->name == 9)
				{
					App->entitycontroller->AddEntity(Entity::entityType::PICKUP, { objdata->data->x,objdata->data->y });
				}
				
			}
		}
	}
}

