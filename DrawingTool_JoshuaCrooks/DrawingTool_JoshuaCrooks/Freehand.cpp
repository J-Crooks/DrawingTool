#include "Freehand.h"

Freehand::Freehand(int x, int y, int Pcolour)
	:Shape(x,y), Pcolour(Pcolour)
{
	AddXY(x, y);
}

Freehand::Freehand(int x, int y, vector<POINT*> data, int Pc)
	:Shape(x,y), data(data), Pcolour(Pc)
{

}
Freehand::~Freehand()
{
	for (int i = 0; i < data.size(); i++)
		delete(data[i]);
}

void Freehand::draw(EasyGraphics* canvas)
{
	if (data.size())
	{
		POINT* last = data[0];
		for (int i = 1; i < data.size(); i++) {
			POINT* next = data[i];
			canvas->setPenColour(Pcolour, 5);
			canvas->drawLine(last->x, last->y, next->x, next->y);
			last = next;
		}
	}
}

void Freehand::edit(int Cy, int Cx, int Pc)
{
	AddXY(Cx, Cy);
	Pcolour = Pc;
}

bool Freehand::hitTest(int Cy, int Cx)
{
	POINT* last = data[0];
	int i = 1;
	while(i < data.size()) {
		POINT* next = data[i];
		if (Cx <= next->x && Cx >= last->x && Cy <= next->y && y >= last->y) {
			double A, B, C, ans;
			A = last->y - next->y;
			B = next->x - last->x;
			C = (last->x - next->x) * last->y + (next->y - last->y) * last->x;
			ans = abs((A * Cx) + (B * Cy) + C) / (sqrt(pow(A, 2) + pow(B, 2)));
			if (ans < 10)
				return true;
		}
		last = next;
		i = i + 2;
	}
	return false;
}
