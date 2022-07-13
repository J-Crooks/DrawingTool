#pragma once
#include "Shape.h"
#include "EasyGraphics.h"

class Rec
	: public Shape
{
public:
	Rec(int x, int y, int w, int h, int Fc, int Bc);
	~Rec();
	void edit(int Cy, int Cx, int Fc, int Bc, int T);
	void draw(EasyGraphics* canvas);
	void recalibrateShape();
	bool hitTest(int Cy, int Cx);
	int getWidth();
	int getHeight();
	int getFillColour();
	int getBorderColour();

private:
	int width, height, Fcolour, Bcolour;
};

inline int Rec::getWidth() { return width; }
inline int Rec::getHeight() { return height; }
inline int Rec::getFillColour() { return Fcolour; }
inline int Rec::getBorderColour() { return Bcolour; }