#pragma once
#include "EasyGraphics.h"
#include <vector>

using namespace std;

struct InData
{
	int type = NULL;
	int x = NULL;
	int y = NULL;
	int r = NULL;
	int w = NULL;
	int h = NULL;
	int ex = NULL;
	int ey = NULL;
	int bc = NULL;
	int fc = NULL;
	vector<POINT*> FHXY;
};
