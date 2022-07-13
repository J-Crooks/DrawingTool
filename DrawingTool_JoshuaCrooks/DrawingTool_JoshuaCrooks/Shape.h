#pragma once

class Shape
{
public:
	Shape(int x, int y);
	~Shape();
	virtual void edit();
	virtual void draw();
	virtual void hitTest();
	int getX();
	int getY();


protected:
	int x, y;
};

inline void Shape::draw() {}
inline void Shape::edit() {}
inline void Shape::hitTest() {}
inline int Shape::getX() { return x; }
inline int Shape::getY() { return y; }

