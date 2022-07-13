#include "Button.h"

Button::Button(int x, int y, int ID, const wchar_t* i, int c, int bs) 
	: x(x), y(y), ID(ID), icon(i), buttonSize(50), colour(c), borderColour(bs), borderSize(1)
{
}

Button::~Button()
{
}

void Button::draw(EasyGraphics* canvas)
{
	canvas->selectBackColour(colour);
	canvas->setPenColour(borderColour, borderSize);
	canvas->drawRectangle(x, y, buttonSize, buttonSize, true);
	canvas->setPenColour(canvas->clBlack, 1);
	if (ID == 0)
		canvas->drawRectangle(x + 5,y + 5, 40, 40, true);
	else if (ID == 1)
		canvas->drawCircle(x + 25,y + 25 , 20, false);
	else if (ID == 2)
		canvas->drawLine(x+45, y+5, x+5, y+45);
	else if (ID > 2)
		canvas->drawBitmap(icon, x+5, y+5, 40, 40, canvas->clWhite);
}

bool Button::hitTest(int Cx, int Cy)
{
	if (((Cx >= x) && (Cx <= (x + buttonSize))) && ((Cy >= y) && (Cy <= (y + buttonSize))))
		return true;
	return false;
}

void Button::edit(int c, int bs)
{
	borderColour = c;
	borderSize = bs;
}