#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1EntityController.h"

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
	to_end = false;
	bool ret = App->map->Load_map(map_names.start->data->GetString());
	App->audio->PlayMusic(App->map->data.musicFile.GetString());
	LOG("Boi: %s", map_names.start->data->GetString());
	App->entitycontroller->AddEntity(Entity::entityType::FLYING_ENEMY, { 350,1880 });
	return true;

}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Load_lvl(0);

	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->entitycontroller->Restart();

	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		Load_lvl(1);

	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame();

	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame();

	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		App->audio->LowerMusicVolume();

	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		App->audio->RaiseMusicVolume();

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		App->audio->LowerSFXVolume();

	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->audio->RaiseSFXVolume();

	else if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		slowing = true;
	//if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && !App->scenechange->IsFading())
		//App->scenechange->ChangeScene(map_names[currentMap], 1.0f);

	//if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN && !App->scenechange->IsFading())
		//App->scenechange->ChangeScene(map_names[OuterWorld], 1.0f);
	if (slowing)SlowMo();

	App->map->Draw();
	App->entitycontroller->Draw();

	p2SString title("Maid in Abyss");

	//App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (to_end)
	{
		if(currentMap < map_names.count() - 1)
			ret = App->scenechange->ChangeMap(++currentMap, fade_time);
		else
			currentMap = 0, ret = App->scenechange->ChangeMap(currentMap, fade_time);


		to_end = false;
	}
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{

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
	return true;
}

bool j1Scene::Load_lvl(int time)
{
	App->map->SwitchMaps(map_names[time]);
	App->entitycontroller->Restart();
	return false;
}

void j1Scene::SlowMo()
{
	timeScale += 0.02f*backToNormal;

	if (timeScale < 0.5f)backToNormal = 1;

	if (backToNormal == 1 && timeScale == 1)slowing = false, backToNormal = -1;

}