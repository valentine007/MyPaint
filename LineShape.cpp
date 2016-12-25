#include "LineShape.h"

// реализаци€ производного класс "лини€"
LineShape::LineShape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb) : Shape(u,v,lp,lb) { }
// переопределение метода draw дл€ рисовани€ пр€мой
void LineShape::Draw(HDC hdc)
{
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);	//¬озвращаем указатель на ранее выбраный объект, считаетс€ хорошей практикой брать объект и возвращать его по окончанию рисовани€
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	//рисуем линию
	MoveToEx(hdc, mPt0.x, mPt0.y, 0);
	LineTo(hdc, mPt1.x, mPt1.y);

	// возвращаемс€ к старым кист€м
	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}