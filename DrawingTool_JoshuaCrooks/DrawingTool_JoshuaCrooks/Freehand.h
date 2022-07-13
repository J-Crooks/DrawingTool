#pragma once
#include "Shape.h"
#include "EasyGraphics.h"
#include "Global.h"
#include <vector>

using namespace std;

class Freehand :
	public Shape
{
public:
	Freehand(int x, int y, int Pcolour);
	Freehand(int x, int y, vector<POINT*> data, int Pc);
	~Freehand();
	void AddXY(int x, int y);
	void draw(EasyGraphics* canvas);
	void edit(int Cy, int Cx, int Pc);
	bool hitTest(int Cy, int Cx);
	vector<POINT*> getPointData();
	int getBorderColour();

private:
	vector<POINT*> data;
	int Pcolour;
};

inline void Freehand::AddXY(int x, int y) { data.push_back(new POINT{ x,y }); }
inline vector<POINT*> Freehand::getPointData() { return data; }
inline int Freehand::getBorderColour() { return Pcolour;  }