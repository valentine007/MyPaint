#include "LineShape.h"

// ���������� ������������ ����� "�����"
LineShape::LineShape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb) : Shape(u,v,lp,lb) { }
// ��������������� ������ draw ��� ��������� ������
void LineShape::Draw(HDC hdc)
{
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);	//���������� ��������� �� ����� �������� ������, ��������� ������� ��������� ����� ������ � ���������� ��� �� ��������� ���������
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	//������ �����
	MoveToEx(hdc, mPt0.x, mPt0.y, 0);
	LineTo(hdc, mPt1.x, mPt1.y);

	// ������������ � ������ ������
	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}