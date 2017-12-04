#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#include "j1App.h"
#include "SDL/include/SDL_rect.h"
struct SDL_Texture;


enum EventTypes
{
	DEFAULT_TYPE = -1,
	LEFT_MOUSE_PRESSED,
	LEFT_MOUSE_RELEASED,
	RIGHT_MOUSE_PRESSED,
	RIGHT_MOUSE_RELEASED,
	MOUSE_HOVER_IN,
	MOUSE_HOVER_OUT,
};


#define CURSOR_WIDTH 2
enum UIType
{
	NO_TYPE,
	INTERACTIVE,
	IMAGE,
	LABEL,
	INTERACTIVE_IMAGE,
	INTERACTIVE_LABELLED_IMAGE,
	INTERACTIVE_LABEL,
	LABELLED_IMAGE,
};
// TODO 1: Create your structure of classes
class UIElement;
class Window;
struct WinElement;
class InheritedInteractive;
class InheritedLabel;
class InheritedImage;
class InteractiveImage;
class InteractiveLabel;
class InteractiveLabelledImage;
class LabelledImage;
struct SDL_Rect;
struct SDL_Texture;
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods

	UIElement* AddElement(UIType type, SDL_Rect& position, iPoint positionOffset, bool draggable = false);
	InheritedInteractive* AddInteractive(SDL_Rect& position, iPoint positionOffset, SDL_Rect& size, j1Module* callback, bool draggable = false);
	InheritedLabel* AddLabel(SDL_Rect& position, iPoint positionOffset, p2SString fontPath, SDL_Color textColor, p2SString label, int size = 12, bool draggable = false);
	InheritedImage* AddImage(SDL_Rect& position, iPoint positionOffset, SDL_Rect* section, bool draggable = false);

	InteractiveImage* AddInteractiveImage(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, SDL_Rect image_section, j1Module* callback, bool draggable = false);
	InteractiveLabel* AddInteractiveLabel(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, j1Module* callback, bool draggable = false);
	InteractiveLabelledImage* AddInteractiveLabelledImage(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, iPoint positionOffsetC, SDL_Rect image_section, p2SString fontPath, SDL_Color textColor, p2SString label, int size, j1Module* callback, bool draggable = false);
	LabelledImage* AddLabelledImage(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, SDL_Rect image_section, bool draggable = false);


	UIElement* DeleteElement(UIElement* element);

	UIElement* AddImage_From_otherFile(SDL_Rect& position, iPoint positionOffset, p2SString& path, bool draggable = false);

	Window* AddWindow(SDL_Rect &window, bool draggable = false);
	// Gui creation functions

	bool CreateSceneIntroGUI();

	SDL_Texture* GetAtlas() const;
	p2List<UIElement*> elements;
	p2List<Window*> window_list;

private:

	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
	p2SString background;
	iPoint mouseLastFrame;
};

#endif // __j1GUI_H__