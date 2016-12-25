#pragma once
#include "Shape.h"
// производный класс фигуры "линия"
class LineShape : public Shape
{
public:
	LineShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);
	void Draw(HDC hdc);
};