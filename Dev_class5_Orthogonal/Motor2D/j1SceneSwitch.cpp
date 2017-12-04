#include "j1SceneSwitch.h"



j1SceneSwitch::j1SceneSwitch()
{
	name.create("sceneswitch")
}


j1SceneSwitch::~j1SceneSwitch()
{
}

bool j1SceneSwitch::Awake()
{
	return false;
}

bool j1SceneSwitch::Start()
{
	return false;
}

bool j1SceneSwitch::Update(float dt)
{
	bool ret = true;

	if (current_step == fade_step::none)
		return ret;

	float normalized = MIN(1.0f, switchtimer.ReadSec() / fadetime);

	switch (current_step)
	{
		case fade_step::fade_to_black:
		{
			if (switchtimer.ReadSec() >= fadetime)
			{
				to_disable->Disable();
				to_enable->Enable();
				switchtimer.Start();
				current_step = fade_step::fade_from_black;
			}
		}break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;

			if (switchtimer.ReadSec() >= fadetime)
			{
				current_step = fade_step::none:
			}
		}break;
	}
	return false;
}

bool j1SceneSwitch::SwitchScene(j1Module * SceneIn, j1Module * SceneOut)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		switchtimer.Start();
		to_enable = SceneIn;
		to_disable = SceneOut;
		ret = true;
	}
	return true;
}

bool j1SceneSwitch::IsSwitching() const
{
	return false;
}
