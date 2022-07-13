#pragma once
#include "EasyGraphics.h"

class Button
{
public:
	Button(int x, int y, int ID, const wchar_t* i, int c, int bs);
	~Button();
	void draw(EasyGraphics* canvas);
	bool hitTest(int Cx, int Cy);
	void edit(int c, int bs);
	int getID();
	const wchar_t* getIcon();

private:
	int x, y, ID, colour;
	int borderColour, borderSize;
	int buttonSize;
	const wchar_t* icon;
};

inline int Button::getID() { return ID; }
inline const wchar_t* Button::getIcon() { return icon;  }