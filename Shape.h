#pragma once			// подключать один раз
#include<Windows.h>		// основной заголовочный файл для Win32 API
// базовый класс фигура
class Shape
{// открытия поля и методы класса
public:
	Shape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);
	virtual ~Shape();

	void SetStartPt(const POINT& p0);	// устанавливаем точку начала рисования
	void SetEndPt(const POINT& p1);		// точка завершения рисования

	virtual void Draw(HDC hdc);			// виртуальный метод рисования, который будет переопределен
										// для каждой производной фигуры
	void operator=(Shape& rhs);
// защищенные методы и поля класса
protected:
	// дескрипторы точек, перьев и кистей
	POINT mPt0;
	POINT mPt1;
	HPEN mhPen;
	HBRUSH mhBrush;

	HPEN mhOldPen;
	HBRUSH mhOldBrush;
};