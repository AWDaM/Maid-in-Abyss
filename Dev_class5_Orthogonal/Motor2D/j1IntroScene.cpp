#include "j1IntroScene.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1GUI.h"
#include "UIElement.h"
#include "Window.h"


j1IntroScene::j1IntroScene()
{
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
	background = App->tex->Load("textures/introbg.png");
	SDL_Rect test;
	test.x = 0;
	test.y = 0;
	test.w = 123;
	test.h = 74;
	testWindow = App->gui->AddWindow(test, true);

	InheritedImage* tmp = App->gui->AddImage(test, { 0,0 }, &test, true);

	testWindow->AddElementToWindow((UIElement*)tmp, { 0,0 });

	return true;
}

bool j1IntroScene::PreUpdate()
{
	return true;
}

bool j1IntroScene::Update(float dt)
{
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
