#include "EllipseShape.h"
// реализация производного класс "овал"
EllipseShape::EllipseShape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb) : Shape(u, v, lp, lb) { }
// переопределение метода draw для рисования прямой
void EllipseShape::Draw(HDC hdc)
{
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	//рисуем овал
	Ellipse(hdc, mPt0.x, mPt0.y, mPt1.x, mPt1.y);

	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}