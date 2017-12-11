#ifndef __LABEL_H__
#define __LABEL_H__

#include "UIElement.h"
#include "j1Fonts.h"

class Label : virtual public UIElement
{
public:
	bool LabelPreUpdate();
	bool LabelUpdate(float dt);
	bool LabelPostUpdate();
	bool LabelDraw();
	bool LabelCleanUp();

	Label();

	Label(SDL_Rect& position, iPoint positionOffset, p2SString fontPath, SDL_Color textColor, p2SString label, int size = 12);

	~Label();

	void ChangeText(p2SString& newlabel);

public:
	iPoint Labelrelativepos = { 0,0 };
	p2SString label;
	_TTF_Font* font = nullptr;
	SDL_Texture* fontTexture;
	SDL_Color textColor;
};

#endif