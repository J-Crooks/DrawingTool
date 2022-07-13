#include "DrawingToolUI.h"

DrawingToolUI::DrawingToolUI() 
	:Tool(-1)
{
	setImmediateDrawMode(false);

	const wchar_t* icons[16] = {
		L"",
		L"",
		L"",
		L".\\icons\\freehandIcon.bmp",
		L".\\icons\\deleteIcon.bmp",
		L".\\icons\\moveIcon.bmp",
		L".\\icons\\fillIcon.bmp",
		L".\\icons\\penIcon.bmp",
		L".\\icons\\saveIcon.bmp",
		L".\\icons\\loadIcon.bmp",
		L".\\icons\\fillRedIcon.bmp",
		L".\\icons\\fillGreenIcon.bmp",
		L".\\icons\\fillBlueIcon.bmp",
		L".\\icons\\penRedIcon.bmp",
		L".\\icons\\penGreenIcon.bmp",
		L".\\icons\\penBlueIcon.bmp"
	};

	int x = 10, y = 10;
	for (int i = 0; i < NOOFBUTTONS; i++) {
		if (i == 8)
			x = x + 365;
		if (i < 10) 
			x = x+60;
		else 
			y = y+60;

		Button* button = (Button*)malloc(sizeof(Button));
		new(button) Button(x, y, i, icons[i], clWhite, clBlack);
		buttons[i] = button;

		if (i == 9)
			x = 10;
	}
}

DrawingToolUI::~DrawingToolUI()
{

}

void DrawingToolUI::onDraw()
{
	clrscr(clWhite);
	if (!shapes.empty()) {
		for (int i = 0; i < shapes.size(); i++) {
			Rec* tmpRec = dynamic_cast<Rec*>(shapes[i]);
			if (tmpRec != NULL)
				dynamic_cast<Rec*>(shapes[i])->draw(this);
			else {
				Circle* tmpCir = dynamic_cast<Circle*>(shapes[i]);
				if (tmpCir != NULL)
					dynamic_cast<Circle*>(shapes[i])->draw(this);
				else {
					Line* tmpLine = dynamic_cast<Line*>(shapes[i]);
					if (tmpLine != NULL)
						dynamic_cast<Line*>(shapes[i])->draw(this);
					else {
						Freehand* tmpFH = dynamic_cast<Freehand*>(shapes[i]);
						if (tmpFH != NULL)
							dynamic_cast<Freehand*>(shapes[i])->draw(this);
					}
				}
			}
		}
	}

	setPenColour(clBlack, 1);
	selectBackColour(clBlack);
	drawRectangle(0, 0, 1050, 70, true);
	drawRectangle(0, 0, 70, 850, true);

	for (int i = 0; i < NOOFBUTTONS; i++) {
		dynamic_cast<Button*>(buttons[i])->draw(this);
	}

	switch (cursor.ID)
	{
	case(-1):
		break;
	case(0):
		drawRectangle(cursor.x + 15, cursor.y + 15, 15, 15, false);
		break;
	case(1):
		drawCircle(cursor.x + 23, cursor.y + 23, 10, false);
		break;
	case(2):
		drawLine(cursor.x + 30, cursor.y + 15, cursor.x + 15, cursor.y + 30);
		break;
	default:
		drawBitmap(buttons[cursor.ID]->getIcon(), cursor.x + 15, cursor.y + 15, 15, 15, true);
		break;
	}
	EasyGraphics::onDraw();
}


bool DrawingToolUI::bttnHit(int Cx, int Cy) {
	for (int i = 0; i < NOOFBUTTONS; i++) {
		if (buttons[i]->hitTest(Cx, Cy))
			return true;
	}
	return false;
}

void DrawingToolUI::onMouseMove(UINT nFlags, int Cx, int Cy)
{

	if (mouseHeld && !shapes.empty() && !buttonHit) {

		if (Tool == 0) 
			dynamic_cast<Rec*>(shapes[shapes.size() - 1])->edit(Cy, Cx, Fill[0], Border[0], Tool);
		
		if (Tool == 1)
			dynamic_cast<Circle*>(shapes[shapes.size() - 1])->edit(Cy, Cx, Fill[0], Border[0], Tool);

		if (Tool == 2)
			dynamic_cast<Line*>(shapes[shapes.size() - 1])->edit(Cy, Cx, Border[0], Tool);

		if (Tool == 3)
			dynamic_cast<Freehand*>(shapes[shapes.size() - 1])->edit(Cy, Cx, Border[0]);

		if (Tool == 5) {
			bool hit = false;
			int i = shapes.size() - 1;
			while (i >= 0 && !hit && !buttonHit) {
				Rec* tmpRec = dynamic_cast<Rec*>(shapes[i]);
				if (tmpRec != NULL) {
					if (dynamic_cast<Rec*>(shapes[i])->hitTest(Cy, Cx)) {
						dynamic_cast<Rec*>(shapes[i])->edit(Cy - MDown[1], Cx- MDown[0], Fill[0], Border[0], Tool);
						hit = !hit;
						MDown[0] = Cx;
						MDown[1] = Cy;
					}
				}
				else {
					Circle* tmpCir = dynamic_cast<Circle*>(shapes[i]);
					if (tmpCir != NULL) {
						if (dynamic_cast<Circle*>(shapes[i])->hitTest(Cy, Cx)) {
							dynamic_cast<Circle*>(shapes[i])->edit(Cy - MDown[1], Cx - MDown[0], Fill[0], Border[0], Tool);
							hit = !hit;
							MDown[0] = Cx;
							MDown[1] = Cy;
						}
					}
					else {
						Line* tmpCir = dynamic_cast<Line*>(shapes[i]);
						if (tmpCir != NULL) {
							if (dynamic_cast<Line*>(shapes[i])->hitTest(Cy, Cx)) {
								dynamic_cast<Line*>(shapes[i])->edit(Cy - MDown[1], Cx - MDown[0], Border[0], Tool);
								hit = !hit;
								MDown[0] = Cx;
								MDown[1] = Cy;
							}
						}					
					}
				}
				i--;
			}
		}
	}

	cursor.x = Cx;
	cursor.y = Cy;
	onDraw();
}


void DrawingToolUI::onLButtonUp(UINT nFlags, int Cx, int Cy)
{
	mouseHeld = !mouseHeld;
	if (Tool == 0 && !shapes.empty() && !buttonHit) {
		Rec* tmpRec = dynamic_cast<Rec*>(shapes[shapes.size() - 1]);
		if (tmpRec != NULL) 
			dynamic_cast<Rec*>(shapes[shapes.size() - 1])->recalibrateShape();

	}
	else {
		if (Tool == 2 && !shapes.empty() && !buttonHit) {
			Line* tmpCir = dynamic_cast<Line*>(shapes[shapes.size() - 1]);
			if (tmpCir != NULL)
				dynamic_cast<Line*>(shapes[shapes.size() - 1])->recalibrateShape();
		}
	}
	buttonHit = false;
}

void DrawingToolUI::swapTool(int newTool)
{
	recolourBttn(Tool, clBlack, 1);
	Tool = newTool;
	recolourBttn(Tool, clGrey, 4);
}

void DrawingToolUI::swapColour(int newColour, int newID)
{
	if (newID > 12) {
		recolourBttn(Border[1], clBlack, 1);
		Border[0] = newColour;
		Border[1] = newID;
		recolourBttn(Border[1], clGrey, 4);
	}
	else {
		recolourBttn(Fill[1], clBlack, 1);
		Fill[0] = newColour;
		Fill[1] = newID;
		recolourBttn(Fill[1], clGrey, 4);
	}
}
void DrawingToolUI::onLButtonDown(UINT nFlags, int Cx, int Cy)
{
	mouseHeld = !mouseHeld;
	bool bttnHit = false;
	MDown[0] = Cx;
	MDown[1] = Cy;
	for (int i = 0; i < NOOFBUTTONS; i++)
	{
		if (buttons[i]->hitTest(Cx, Cy)) {
			if (buttons[i]->getID() < 8)
				swapTool(buttons[i]->getID());
			else if (buttons[i]->getID() == 8)
				saveCanvas();
			else if (buttons[i]->getID() == 9)
				openCanvas();
			else 
				switch (buttons[i]->getID())
				{
				case(10):
					swapColour(clRed, 10);
					break;
				case(11):
					swapColour(clGreen, 11);
					break;
				case(12):
					swapColour(clBlue, 12);
					break;
				case(13):
					swapColour(clRed, 13);
					break;
				case(14):
					swapColour(clGreen, 14);
					break;
				case(15):
					swapColour(clBlue, 15);
					break;
				default:
					break;	
				}
			bttnHit = true;
			buttonHit = true;
			cursor.ID = Tool;
		}
	}

	if (!bttnHit)
		bttnFunc(Cx, Cy);
	onDraw();
}


void DrawingToolUI::bttnFunc(int Cx, int Cy) {
	switch (Tool)
	{
	case(0):
		{	
		Rec* tmpRec = (Rec*)malloc(sizeof(Rec));
		new(tmpRec) Rec(Cx, Cy, 0, 0, 0, 0);
		shapes.push_back(tmpRec);
	
		}
		break;
	case(1):
		{
		Circle* tmpCir = (Circle*)malloc(sizeof(Circle));
		new(tmpCir) Circle(Cx, Cy, 0, Fill[0], Border[0]);
		shapes.push_back(tmpCir);
		}
		break;
	case(2):
		{
		Line* tmpLine = (Line*)malloc(sizeof(Line));
		new(tmpLine) Line(Cx, Cy, Cx, Cy, Border[0]);
		shapes.push_back(tmpLine);
		}
		break;
	case(3):
		{
		Freehand* tmpFH = (Freehand*)malloc(sizeof(Freehand));
		new(tmpFH) Freehand(Cx, Cy, Border[0]);
		shapes.push_back(tmpFH);
		}
		break;
	case(4):
	{
		bool hit = false;
		int i = shapes.size() - 1;
		while (i >= 0 && !hit) {
			Rec* tmpRec = dynamic_cast<Rec*>(shapes[i]);
			if (tmpRec != NULL) {
				if (dynamic_cast<Rec*>(shapes[i])->hitTest(Cy, Cx)) {
					delete(shapes[i]);
					shapes.erase(shapes.begin() + i);
					hit = !hit;
				}
			}
			else {
				Circle* tmpCir = dynamic_cast<Circle*>(shapes[i]);
				if (tmpCir != NULL) {
					if (dynamic_cast<Circle*>(shapes[i])->hitTest(Cy, Cx)) {
						delete(shapes[i]);
						shapes.erase(shapes.begin() + i);
						hit = !hit;
					}
				}
				else {
					Line* tmpCir = dynamic_cast<Line*>(shapes[i]);
					if (tmpCir != NULL) {
						if (dynamic_cast<Line*>(shapes[i])->hitTest(Cy, Cx)) {
							delete(shapes[i]);
							shapes.erase(shapes.begin() + i);
							hit = !hit;
						}
					}
					else {
						Freehand* tmpFH = dynamic_cast<Freehand*>(shapes[i]);
						if (tmpFH != NULL) {
							if (dynamic_cast<Freehand*>(shapes[i])->hitTest(Cy, Cx)) {
								delete(shapes[i]);
								shapes.erase(shapes.begin() + i);
								hit = !hit;
							}
						}
					}
				}
			}
			i--;
		}
		}
		break;
	case(6):
		recolourShape(Cx, Cy);
		break;
	case(7):
		recolourShape(Cx, Cy);
	default:
		break;
	}
}

void DrawingToolUI::recolourShape(int Cx, int Cy) {
	bool hit = false;
	int i = shapes.size() - 1;
	int tmpFill = Fill[0];
	int tmpBorder = Border[0];

	if (Tool == 7)
		tmpFill = NULL;
	else
		tmpBorder = NULL;

	while (i >= 0 && !hit) {
		Rec* tmpRec = dynamic_cast<Rec*>(shapes[i]);
		if (tmpRec != NULL) {
			if (dynamic_cast<Rec*>(shapes[i])->hitTest(Cy, Cx)) {
				dynamic_cast<Rec*>(shapes[i])->edit(NULL, NULL, tmpFill, tmpBorder, Tool);
				hit = !hit;
			}
		}
		else {
			Circle* tmpCir = dynamic_cast<Circle*>(shapes[i]);
			if (tmpCir != NULL) {
				if (dynamic_cast<Circle*>(shapes[i])->hitTest(Cy, Cx)) {
					dynamic_cast<Circle*>(shapes[i])->edit(NULL, NULL, tmpFill, tmpBorder, Tool);
					hit = !hit;
				}
			}
			else {
				Line* tmpCir = dynamic_cast<Line*>(shapes[i]);
				if (tmpCir != NULL) {
					if (dynamic_cast<Line*>(shapes[i])->hitTest(Cy, Cx)) {
						dynamic_cast<Line*>(shapes[i])->edit(NULL, NULL, tmpBorder, Tool);
						hit = !hit;
					}
				}
			}
		}
		i--;
	}
}

void DrawingToolUI::saveCanvas() {

	ofstream savefile;
	savefile.open("Data.txt", ios::out);

	if (!shapes.empty() && !savefile.fail()) {
		for (int i = 0; i < shapes.size(); i++) {
			Rec* tmpRec = dynamic_cast<Rec*>(shapes[i]);
			if (tmpRec != NULL) {
				savefile << 1 << "  "
					<< dynamic_cast<Rec*>(shapes[i])->getX() << "  "
					<< dynamic_cast<Rec*>(shapes[i])->getY() << "  "
					<< dynamic_cast<Rec*>(shapes[i])->getWidth() << "  "
					<< dynamic_cast<Rec*>(shapes[i])->getHeight() << "  "
					<< dynamic_cast<Rec*>(shapes[i])->getFillColour() << "  "
					<< dynamic_cast<Rec*>(shapes[i])->getBorderColour()
					<< endl;
			}
			else {
				Circle* tmpCir = dynamic_cast<Circle*>(shapes[i]);
				if (tmpCir != NULL) {
					savefile << 2 << "  "
						<< dynamic_cast<Circle*>(shapes[i])->getX() << "  "
						<< dynamic_cast<Circle*>(shapes[i])->getY() << "  "
						<< dynamic_cast<Circle*>(shapes[i])->getRadius() << "  "
						<< dynamic_cast<Circle*>(shapes[i])->getFillColour() << "  "
						<< dynamic_cast<Circle*>(shapes[i])->getBorderColour()
						<< endl;
				}
				else {
					Line* tmpLine = dynamic_cast<Line*>(shapes[i]);
					if (tmpLine != NULL) {
						savefile << 3 << "  "
							<< dynamic_cast<Line*>(shapes[i])->getX() << "  "
							<< dynamic_cast<Line*>(shapes[i])->getY() << "  "
							<< dynamic_cast<Line*>(shapes[i])->getEndX() << "  "
							<< dynamic_cast<Line*>(shapes[i])->getEndY() << "  "
							<< dynamic_cast<Line*>(shapes[i])->getBorderColour()
							<< endl;
					}
					else {
						Freehand* tmpFH = dynamic_cast<Freehand*>(shapes[i]);
						if (tmpFH != NULL) {
							vector<POINT*> data = dynamic_cast<Freehand*>(shapes[i])->getPointData();
							savefile << 4 << "  " << dynamic_cast<Freehand*>(shapes[i])->getBorderColour() << "  ";
							for (int i = 1; i < data.size(); i++) {
								savefile << data[i]->x << "  " << data[i]->y << "  ";
							}
							savefile << -1 <<endl;
						}
					}
				}
			}
		}
	}
	savefile.close();
}

void DrawingToolUI::openCanvas() 
{
	const int REC = 1, CIR = 2, LINE = 3, FH = 4;
	ifstream datafile("Data.txt", ios::in);

	for (int i = 0; i < shapes.size(); i++) {
		delete(shapes[i]);
	}
	shapes.clear();
		
	while (datafile && !datafile.fail() && datafile >> data.type) {
		if (data.type == REC)
			datafile >> data.x >> data.y >> data.w >> data.h >> data.fc >> data.bc;
		else if (data.type == CIR)
			datafile >> data.x >> data.y >> data.r >> data.fc >> data.bc;
		else if (data.type == LINE)
			datafile >> data.x >> data.y >> data.ex >> data.ey >> data.bc;
		else if (data.type == FH) {
			datafile >> data.bc;
			int temp, x, y;
			bool  whichCo = 0;
			datafile >> temp;
			while (temp != -1) {
				if (whichCo == 0) {
					x = temp;
					whichCo = !whichCo;
				}
				else {
					y = temp;
					whichCo = !whichCo;
					data.FHXY.push_back(new POINT{ x,y });
				}
				datafile >> temp;
			}
		}
		shapes.push_back(ShapeFactory::GetFactory()->Create(data));
	}
	datafile.close();
}


void DrawingToolUI::onClose() {

	for (int i = 0; i < shapes.size(); i++) {
		Freehand* tmpFH = dynamic_cast<Freehand*>(shapes[i]);
		if (tmpFH != NULL) 
			dynamic_cast<Freehand*>(shapes[i])->~Freehand();
		delete(shapes[i]);
	}

	for (int i = 0; i < NOOFBUTTONS; i++)
		delete(buttons[i]);

}
