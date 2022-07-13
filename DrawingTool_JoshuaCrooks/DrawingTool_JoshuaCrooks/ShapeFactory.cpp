#include "ShapeFactory.h"

ShapeFactory* ShapeFactory::Factory = NULL;
ShapeFactory* ShapeFactory::GetFactory() {
	if (!Factory)
		Factory = new ShapeFactory();
	return Factory;
}

Shape* ShapeFactory::Create(InData data)
{
	if (data.type == REC) {
		Rec* tmpRec = (Rec*)malloc(sizeof(Rec));
		return new(tmpRec) Rec(data.x, data.y, data.w, data.h, data.fc, data.bc);
	}
	else if (data.type == CIR) {
		Circle* tmpCir = (Circle*)malloc(sizeof(Circle));
		return new(tmpCir) Circle(data.x, data.y, data.r, data.fc, data.bc);
	}
	else if (data.type == LINE) {
		Line* tmpLine = (Line*)malloc(sizeof(Line));
		return new(tmpLine) Line(data.x, data.y, data.ex, data.ey, data.bc);
	}
	else if (data.type == FH) {
		Freehand* tmpFH = (Freehand*)malloc(sizeof(Freehand));
		return new(tmpFH) Freehand(data.FHXY[0]->x, data.FHXY[0]->y, data.FHXY, data.bc);
	}
	else return NULL;
}