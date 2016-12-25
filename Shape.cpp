#include "Shape.h"

// реализация парент класса "фигура"
Shape::Shape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb)
{	// координаты рисования 
	// начальные координаты х у
	mPt0.x = u.x; 
	mPt0.y = u.y;
	// координаты завершения фигуры х у
	mPt1.x = v.x;
	mPt1.y = v.y;
	// создаем объекты кистей и перьек
	mhPen = CreatePenIndirect(&lp);
	mhBrush = CreateBrushIndirect(&lb);
	mhOldPen = 0;
	mhOldBrush = 0;
}

Shape::~Shape() //Осовбождаем ресурсы выделенные для объектов pen и brush
{
	DeleteObject(mhPen);
	DeleteObject(mhBrush);
}
// метод установки начальной точки
void Shape::SetStartPt(const POINT & p0)
{
	mPt0 = p0;
}
// метод установки конечной точки
void Shape::SetEndPt(const POINT & p1)
{
	mPt1 = p1;
}
// метод рисования
void Shape::Draw(HDC hdc)
{
	return;
}
// метод перегрузки операторов класса
void Shape::operator=(Shape & rhs)
{
	mPt0 = rhs.mPt0;
	mPt1 = rhs.mPt1;
	mhPen = rhs.mhPen;
	mhBrush = rhs.mhBrush;
	mhOldPen = 0;
	mhOldBrush = 0;
}