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

//	p2SString name("gui/background.png");
//	SDL_Rect rect = { 0,0,0,0 };
//	App->gui->AddImage_From_otherFile(rect, { 0,0 }, name);
//
////	background = App->tex->Load("textures/introbg.png");
//	SDL_Rect test;
//	test.x = 0;
//	test.y = 0;
//	test.w = 123;
//	test.h = 74;
//	testWindow = App->gui->AddWindow(test, true);
//
//	InheritedImage* tmp = App->gui->AddImage(test, { 0,0 }, &test, true);
//
//	testWindow->AddElementToWindow(tmp, { 0,0 });

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
		
	App->gui->Draw();

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

bool j1IntroScene::OnEvent(UIElement * element, int eventType)
{
	return true;
}

void j1IntroScene::Load_Gui()
{
	pugi::xml_document	Gui_config_file;
	pugi::xml_node		guiconfig;
	pugi::xml_node		tmp;

	guiconfig = App->LoadConfig(Gui_config_file,"Gui_config");

	guiconfig = guiconfig.child("introscene");

	tmp = guiconfig.child("interactivelabelledimage");
	if (tmp)
	{
		SDL_Rect pos = { tmp.child("pos").attribute("x").as_int(), tmp.child("pos").attribute("y").as_int(), tmp.child("pos").attribute("w").as_int(), tmp.child("pos").attribute("h").as_int()};
		iPoint relativeposA = { tmp.child("relativeposA").attribute("x").as_int(),tmp.child("relativeposA").attribute("y").as_int() };
		iPoint relativeposB = { tmp.child("relativeposB").attribute("x").as_int(),tmp.child("relativeposB").attribute("y").as_int() };
		iPoint relativeposC = { tmp.child("relativeposC").attribute("x").as_int(),tmp.child("relativeposC").attribute("y").as_int() };
		SDL_Rect section = { tmp.child("imagesection").attribute("x").as_int(), tmp.child("imagesection").attribute("y").as_int(), tmp.child("imagesection").attribute("w").as_int(), tmp.child("imagesection").attribute("h").as_int() };
		p2SString path = (tmp.child("fontpath").attribute("path").as_string());
		SDL_Color color = { tmp.child("color").attribute("r").as_int(), tmp.child("color").attribute("g").as_int(), tmp.child("color").attribute("b").as_int(), tmp.child("color").attribute("a").as_int() };
		p2SString label = (tmp.child("label").attribute("string").as_string());
		int size = tmp.child("size").attribute("value").as_int();
		bool draggable = tmp.child("draggable").attribute("value").as_bool();
		InteractiveLabelledImage* added = App->gui->AddInteractiveLabelledImage(pos, relativeposA, relativeposB, relativeposC, section, path, color, label, size, this, draggable);
		
		added->hover
		while (tmp = tmp.next_sibling("interactivelabelledimage"))
		{
			SDL_Rect pos = { tmp.child("pos").attribute("x").as_int(), tmp.child("pos").attribute("y").as_int(), tmp.child("pos").attribute("w").as_int(), tmp.child("pos").attribute("h").as_int() };
			iPoint relativeposA = { tmp.child("relativeposA").attribute("x").as_int(),tmp.child("relativeposA").attribute("y").as_int() };
			iPoint relativeposB = { tmp.child("relativeposB").attribute("x").as_int(),tmp.child("relativeposB").attribute("y").as_int() };
			iPoint relativeposC = { tmp.child("relativeposC").attribute("x").as_int(),tmp.child("relativeposC").attribute("y").as_int() };
			SDL_Rect section = { tmp.child("imagesection").attribute("x").as_int(), tmp.child("imagesection").attribute("y").as_int(), tmp.child("imagesection").attribute("w").as_int(), tmp.child("imagesection").attribute("h").as_int() };
			p2SString path = (tmp.child("fontpath").attribute("path").as_string());
			SDL_Color color = { tmp.child("color").attribute("r").as_int(), tmp.child("color").attribute("g").as_int(), tmp.child("color").attribute("b").as_int(), tmp.child("color").attribute("a").as_int() };
			p2SString label = (tmp.child("label").attribute("string").as_string());
			int size = tmp.child("size").attribute("value").as_int();
			bool draggable = tmp.child("draggable").attribute("value").as_bool();
			App->gui->AddInteractiveLabelledImage(pos, relativeposA, relativeposB, relativeposC, section, path, color, label, size, this, draggable);
		}
	}
}
