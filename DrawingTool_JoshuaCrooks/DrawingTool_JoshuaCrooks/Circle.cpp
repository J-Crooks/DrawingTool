#include "Circle.h"

Circle::Circle(int x, int y, int r, int Fc, int Bc) :
	Shape(x,y), radius(r), Fcolour(Fc), Bcolour(Bc)
{
}

Circle::~Circle()
{
}

void Circle::edit(int Cy, int Cx, int Fc, int Bc, int Tool)
{
	if (Tool != 5) {
		if (Cy != NULL && Cx != NULL) {
			int newX = abs(x - Cx);
			int newY = abs(y - Cy);
			radius = sqrt((newX * newX) + (newY * newY));
		}

		if (Fc != NULL)
			Fcolour = Fc;
		if (Bc != NULL)
			Bcolour = Bc;
	}
	else {
		x = x + Cx;
		y = y + Cy;
	}
}

void Circle::draw(EasyGraphics* canvas)
{
	canvas->selectBackColour(Fcolour);
	canvas->setPenColour(Bcolour, 5);
	canvas->drawCircle(x, y, radius,true);
}

bool Circle::hitTest(int Cy, int Cx)
{
if (((Cx - x)*(Cx - x)) + ((Cy - y)*(Cy - y)) <= radius * radius)
		return true;
	return false;
}