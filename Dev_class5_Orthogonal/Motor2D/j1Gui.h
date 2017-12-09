#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
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
	PRESSED_ENTER
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

enum InteractiveType
{
	DEFAULT,
	QUIT,
	CONTINUE,
	NEWGAME,
	OPEN_SETTINGS,
	OPEN_CREDITS,
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

	InheritedInteractive* AddInteractive(SDL_Rect& position, iPoint positionOffset, SDL_Rect& size,	 InteractiveType type, j1Module* callback, bool draggable = false);
	InheritedLabel* AddLabel(SDL_Rect& position, iPoint positionOffset, p2SString fontPath, SDL_Color textColor, p2SString label, int size = 12, bool draggable = false);
	InheritedImage* AddImage(SDL_Rect& position, iPoint positionOffset, SDL_Rect& section, bool draggable = false);

	InteractiveImage* AddInteractiveImage(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, SDL_Rect image_section, InteractiveType type, j1Module* callback, bool draggable = false);
	InteractiveLabel* AddInteractiveLabel(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, InteractiveType type, j1Module* callback, bool draggable = false);
	InteractiveLabelledImage* AddInteractiveLabelledImage(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, iPoint positionOffsetC, SDL_Rect& image_section, p2SString& fontPath, SDL_Color& textColor, p2SString& label, int size, InteractiveType type, j1Module* callback, bool draggable = false);
	LabelledImage* AddLabelledImage(SDL_Rect& position, iPoint positionOffsetA, iPoint positionOffsetB, p2SString fontPath, SDL_Color textColor, p2SString label, int size, SDL_Rect image_section, bool draggable = false);


	UIElement* DeleteElement(UIElement* element);

	UIElement* AddImage_From_otherFile(SDL_Rect& position, iPoint positionOffset, p2SString& path, bool draggable = false);

	Window* AddWindow(SDL_Rect &window, bool draggable = false);
	// Gui creation functions

	void Load_UIElements(pugi::xml_node node);
	void Load_SceneWindows(pugi::xml_node node);
	UIElement* Load_InteractiveLabelledImage_fromXML(pugi::xml_node		tmp);
	Window* Load_Window_fromXML(pugi::xml_node node);
	void Load_WindowElements_fromXML(pugi::xml_node node, Window* window);
	UIElement* Load_Image_fromXML(pugi::xml_node node);
	UIElement* Load_AlterantiveImage_fromXML(pugi::xml_node node);

	InteractiveType InteractiveType_from_int(int type);
	bool CreateSceneIntroGUI();
	bool OnEvent(UIElement * element, int eventType);

	SDL_Texture* GetAtlas() const;
	p2List<UIElement*> elements;
	p2List<Window*> window_list;
	bool dragging_window = false;
	int currentFocus = -1;
	p2List<UIElement*> focusList;


private:

	SDL_Texture* atlas = nullptr;

	p2SString atlas_file_name;
	p2SString background;
	iPoint mouseLastFrame;
};

#endif // __j1GUI_H__