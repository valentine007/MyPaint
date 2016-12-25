#pragma once
#include "Shape.h"
// производный класс фигуры "овал"
class EllipseShape :
	public Shape
{
public:
	EllipseShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);
	void Draw(HDC hdc);
};
