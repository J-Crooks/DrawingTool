#pragma once
#include "EasyGraphics.h"
#include "Shape.h"
#include "Circle.h"
#include "Rec.h"
#include "Line.h"
#include "Freehand.h"
#include "Global.h"

class ShapeFactory
{
public:
	static ShapeFactory* GetFactory();
	Shape* Create(InData data);

private:
	static ShapeFactory* Factory;
	const int REC = 1, CIR = 2, LINE = 3, FH = 4;
};

