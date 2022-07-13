#pragma once
#include "Shape.h"
#include "EasyGraphics.h"

class Line :
	public Shape
{
public:
	Line(int Sx, int Sy, int Ex, int Ey, int Pc);
	~Line();
	void draw(EasyGraphics* canvas);
	void edit(int Cy, int Cx, int Ps, int Tool);
	bool hitTest(int Cy, int Cx);
	int getEndX();
	int getEndY();
	int getBorderColour();
	void recalibrateShape();

private:
	float area(int x1, int y1, int x2, int y2, int x3, int y3);
	bool insideTri(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
	void swapPoints();
	int endx, endy;
	int Pcolour;
};

inline int Line::getEndX() { return endx; }
inline int Line::getEndY() { return endy; } 
inline int Line::getBorderColour() { return Pcolour; }

inline void Line::recalibrateShape()
{
	if ((endy < y && endx < x) || (endx < x && endy > y))
		swapPoints();
}
inline float Line::area(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}