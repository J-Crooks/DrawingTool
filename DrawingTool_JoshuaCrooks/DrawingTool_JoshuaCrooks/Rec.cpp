#include "Rec.h"

Rec::Rec(int x, int y, int w, int h, int Fc, int Bc)
	:Shape(x, y), width(w), height(h), Fcolour(Fc), Bcolour(Bc)
{
	
}

Rec::~Rec()
{
}

void Rec::edit(int Cy, int Cx, int Fc, int Bc, int T)
{

	if (T != 5) {
		if (Cy != NULL)
			height = Cy - y;
		if (Cx != NULL)
			width = Cx - x;
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

void Rec::recalibrateShape()
{
	if (height < 0) {
		y = y + height;
		height = abs(height);
	}

	if (width < 0) {
		x = x + width;
		width = abs(width);
	}
}

void Rec::draw(EasyGraphics* canvas) 
{
	canvas->selectBackColour(Fcolour);
	canvas->setPenColour(Bcolour,5);
	canvas->drawRectangle(x, y, width, height, true);
}

bool Rec::hitTest(int Cy, int Cx) 
{	
	if (((Cx >= x) && (Cx <= (x + width))) && ((Cy >= y) && (Cy <= (y + height))))
		return true;
	return false;
	
}

