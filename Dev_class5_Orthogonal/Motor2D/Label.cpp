#include "Label.h"
#include "j1Fonts.h"
#include "j1Render.h"


Label::Label(SDL_Rect& position, iPoint Labelrelativepos, p2SString fontPath, SDL_Color textColor, p2SString label, int size) : Labelrelativepos(Labelrelativepos)
{
	font = App->font->Load(fontPath.GetString(), size);
	fontTexture = App->font->Print(label.GetString(), textColor, font);

}


Label::~Label()
{
}

bool Label::LabelPreUpdate()
{

	return true;
}

bool Label::LabelUpdate(float dt)
{

	return true;
}

bool Label::LabelPostUpdate()
{

	return true;
}

bool Label::LabelDraw()
{
	App->render->Blit(fontTexture, position.x, position.y);

	return true;
}

Label::Label()
{
}


