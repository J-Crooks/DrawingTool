#pragma once
#include "EasyGraphics.h"
#include "Button.h"
#include "Rec.h"
#include "Circle.h"
#include "Line.h"
#include "Freehand.h"
#include "Shape.h"
#include "ShapeFactory.h"
#include "Global.h"
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class DrawingToolUI : public EasyGraphics
{
public:
	DrawingToolUI();
	~DrawingToolUI();
	void onClose();

private:
	void onDraw();
	void onLButtonDown(UINT nFlags, int Cx, int Cy);
	void onLButtonUp(UINT nFlags, int Cx, int Cy);
	void onMouseMove(UINT nFlags, int Cx, int Cy);
	bool bttnHit(int Cx, int Cy);
	void bttnFunc(int Cx, int Cy);
	void recolourBttn(int bttnID, int colour, int BS);
	void recolourShape(int Cx, int Cy);
	void swapTool(int newTool);
	void swapColour(int newColour, int newID);
	void saveCanvas();
	void openCanvas();


	Button* buttons[16];
	vector<Shape*> shapes;

	struct Cursor
	{
		int x, y, ID = -1;
	};

	Cursor cursor;
	InData data;

	bool mouseHeld = false;
	bool buttonHit = false;
	const int NOOFBUTTONS = 16;
	int Fill[2] = { clWhite, -1 };
	int Border[2] = { clBlack, -1 };
	int Tool;
	int MDown[2];
};

inline void DrawingToolUI::recolourBttn(int BttnID, int colour, int BS) {
	if (BttnID != -1)
		dynamic_cast<Button*>(buttons[BttnID])->edit(colour, BS);
}




