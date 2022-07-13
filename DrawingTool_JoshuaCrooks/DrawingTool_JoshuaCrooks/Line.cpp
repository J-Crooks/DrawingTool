#include "Line.h"

Line::Line(int Sx, int Sy, int Ex, int Ey, int Pc) 
	: Shape(Sx,Sy), endx(Ex), endy(Ey), Pcolour(Pc)
{
}

Line::~Line()
{
}

void Line::draw(EasyGraphics* canvas) 
{
	canvas->selectBackColour(canvas->clBlue);
	canvas->setPenColour(Pcolour, 5);
	canvas->drawLine(x, y, endx, endy);
}

void Line::edit(int Cy, int Cx, int Ps, int Tool)
{
	if (Tool != 5) {
		if (Cy != NULL)
			endy = Cy;
		if (Cx != NULL)
			endx = Cx;
		if (Ps != NULL)
			Pcolour = Ps;
	}
	else {
		x = x + Cx;
		y = y + Cy;
		endx = endx + Cx;
		endy = endy + Cy;
	}
}


void Line::swapPoints()
{
	int tmpy = endy;
	endy = y;
	y = tmpy;

	int tmpx = endx;
	endx = x;
	x = tmpx;
}

bool Line::hitTest(int Cy, int Cx)
{
		if (endy >= y) {
			if (((Cx >= x) && (Cx <= endx)) && ((Cy >= y) && (Cy <= endy))) {
				if (!insideTri(x, y + 10, x, endy, endx - 10, endy, Cx, Cy) && !insideTri(x + 10, y, endx, y, endx, endy - 10, Cx, Cy))
					return true;
			}
		}
		else {
			if (((Cx >= x) && (Cx <= endx)) && ((Cy <= y) && (Cy >=  endy))) {
				if (!insideTri(x, y - 10, x, endy, endx - 10, endy, Cx, Cy) && !insideTri(x + 10, y, endx, y, endx, endy + 10, Cx, Cy))
					return true;
			}
		}

	return false;
}

bool Line::insideTri(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
	float A = area(x1, y1, x2, y2, x3, y3);
	float A1 = area(x, y, x2, y2, x3, y3);
	float A2 = area(x1, y1, x, y, x3, y3);
	float A3 = area(x1, y1, x2, y2, x, y);

	if (A == (A1 + A2 + A3))
		return true;
	return false;
}
