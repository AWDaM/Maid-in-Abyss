#ifndef __j1SCENETWO_H__
#define __j1SCENETWO_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene_two : public j1Module
{
public:

	j1Scene_two();

	// Destructor
	virtual ~j1Scene_two();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	p2SString map_name;
};

#endif // __j1SCENETWO_H__