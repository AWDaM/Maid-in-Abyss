#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1SceneSwitch.h"
#include "j1IntroScene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UIElement.h"
#include "InheritedImage.h"
#include "InteractiveLabelledImage.h"
#include "Window.h"



j1IntroScene::j1IntroScene()
{
	name.create("introscene");
}


j1IntroScene::~j1IntroScene()
{
}

bool j1IntroScene::Awake()
{
	return true;
}

bool j1IntroScene::Start()
{
	pugi::xml_document	Gui_config_file;
	pugi::xml_node		guiconfig;

	guiconfig = App->LoadConfig(Gui_config_file, "Gui_config.xml");

	guiconfig = guiconfig.child("introscene");

	App->audio->PlayMusic("MainTheme.ogg");

	App->gui->Load_UIElements(guiconfig,this);
	App->gui->Load_SceneWindows(guiconfig,this);
	return true;
}

bool j1IntroScene::PreUpdate()
{
	return true;
}

bool j1IntroScene::Update(float dt)
{

	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->sceneswitch->SwitchScene(App->scene,this);
	}

	App->gui->Draw(dt);

	//App->render->Blit(background, 0, 0);

	return true;
}

bool j1IntroScene::PostUpdate()
{
	return true;
}

bool j1IntroScene::CleanUp()
{
	return true;
}

bool j1IntroScene::OnEvent(UIElement* element, int eventType)
{
	bool ret = true;
	element->HandleAnimation(eventType);

	if (eventType == EventTypes::LEFT_MOUSE_PRESSED)
	{
		App->audio->PlayFx(1);
	}

	if (element->type == InteractiveType::CLOSE_WINDOW && eventType == EventTypes::LEFT_MOUSE_PRESSED)
	{
		settings->active = false;
		credits->active = false;
	}
	if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == QUIT)
	{
		LOG("Quit button");
		ret = false;
	}

	else if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == NEWGAME)
	{
		App->sceneswitch->SwitchScene(App->scene, App->introscene);
		LOG("New game");
	}

	else if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == CONTINUE)
	{
		App->sceneswitch->SwitchScene(App->scene, App->introscene);
		App->sceneswitch->switchingFromSaveGame = true;
	}
	else if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == OPEN_SETTINGS)
		settings->active = true;
	else if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == OPEN_CREDITS)
		credits->active = true;


	if (eventType == EventTypes::PRESSED_ENTER)
		ret = element->OnEvent();

	return ret;
}

