#pragma once
#include "Shape.h"
#include "EasyGraphics.h"
#include <stdlib.h>

class Circle :
	public Shape
{
public:
	Circle(int x, int y, int r, int Fc, int Bc);
	~Circle();
	void draw(EasyGraphics* canvas);
	void edit(int Cx, int Cy, int Fc, int Bc, int Tool);
	bool hitTest(int Cy, int Cx);
	int getRadius();
	int getFillColour();
	int getBorderColour();
private:
	int radius, Fcolour, Bcolour;
};

inline int Circle::getRadius() { return radius; }
inline int Circle::getFillColour() { return Fcolour; }
inline int Circle::getBorderColour() { return Bcolour; }

