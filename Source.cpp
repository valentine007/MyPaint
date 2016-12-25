// ���������� ������������ ����� � ����������� ������������ ����
#include <Windows.h>
#include <vector>
#include <string>
#include "resource.h"
#include "Shape.h"
#include "LineShape.h"
#include "RectShape.h"
#include "EllipseShape.h"
using namespace std;

// ��������� � �������������� ���������� ����������
// ������ ������� �������� ������
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);

//===����� ����� ����� �������� ����������� ���������� ������ ������===//
//===��������:const COLORREF YELLOW = RGB(255, 255, 0);

//������ ����������� �������� ������� �����//
#define THIN_LINE 1
#define NORMAL_LINE 5	// �� ����� ����� ��������, ��������: #define NORMAL_LINE 7
#define THICK_LINE 9	// ��� ��� �������� �������� �����

//====================================//
// ��������� ����������� �������� ����, ����  � ����� ����� � ���������
HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;
HMENU ghMenu = 0;

HWND ghDlg = 0;
HWND ghRadio = 0;
HWND ghCombo = 0;

vector<Shape*> gShapes;
Shape* gShape = 0;

bool gMouseDown = false;
bool gCtrlDown = false;
// ������ ��������� �������� ������
int gCurrPrimSel = ID_PRIMITIVE_LINE;
int gCurrPenColSel = ID_PENCOLOR_BLACK;
int gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
int gCurrPenStyleSel = ID_PENSTYLE_SOLID;
int gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;
int gCurrLineSettings = ID_LINESETTINGS_THIN;

// ������� ���������� ������ � ������
LOGPEN gLogPen;
LOGBRUSH gLogBrush;

// �������
int HandleMenu(HWND hWnd, WPARAM wParam);

// ��� 1: ���������� � ������������� ������� ���������
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �������� ��������� ��� ���������
	HDC hdc = 0;
	PAINTSTRUCT ps;
	
	POINT p0, p1;

	switch (msg)
	{
	case WM_CREATE: //	�������� ������� ���� �� ���������
		CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_LINESETTINGS_THIN, MF_CHECKED);
		return 0;

	case WM_COMMAND:
		HandleMenu(hWnd, wParam);
		return 0;

	case WM_LBUTTONDOWN:
		// ������ ����, ���� ���� ������ ���� �������� ���������� �������
		SetCapture(hWnd);
		gMouseDown = true;

		// �������� ���������� ������� ����
		p0.x = LOWORD(lParam);
		p0.y = HIWORD(lParam);

		// ���� �� �� ����� ������������� ��������� - ������������� �� � ����
		p1.x = 0;
		p1.y = 0;

		// ������������� ��������� �������� ��������� ��������� ����� �������� �������������� ������ Switch
		switch (gCurrPrimSel)
		{
		case ID_PRIMITIVE_LINE:
			gShape = new LineShape(p0, p1, gLogPen, gLogBrush);
			break;

		case ID_PRIMITIVE_RECTANGLE:
			gShape = new RectShape(p0, p1, gLogPen, gLogBrush);
			break;

		case ID_PRIMITIVE_ELLIPSE:
			gShape = new EllipseShape(p0, p1, gLogPen, gLogBrush);
			break;
		}

		return 0;
	// ��������� � ����������� �������
	case WM_MOUSEMOVE:
		if (gMouseDown)
		{
			p1.x = LOWORD(lParam);
			p1.y = HIWORD(lParam);

			gShape->SetEndPt(p1);
			// ����������� ����
			InvalidateRect(hWnd, 0, true);
		}
		return 0;
	// ��������� � ���, ��� �������� ������ ����
	case WM_LBUTTONUP:
		ReleaseCapture();
		gMouseDown = false;
		
		p1.x = LOWORD(lParam);
		p1.y = HIWORD(lParam);
		gShape->SetEndPt(p1);

		gShapes.push_back(gShape);

		InvalidateRect(hWnd, 0, true);
		return 0;
	// ���������� ���� � ���, ��� ��� ���� ������� ���������
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (gMouseDown)
			gShape->Draw(hdc);
		for (int i = 0; i < gShapes.size(); i++)
			gShapes[i]->Draw(hdc);

		EndPaint(hWnd, &ps);
	// ���������� ������� ������
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(ghMainWnd);
			break;
		case VK_CONTROL:
			gCtrlDown = true;
			break;
		case 0x5A:
			if (gCtrlDown && gShapes.size() != 0)
			{
				Shape* shape = gShapes[gShapes.size() - 1];
				gShapes.pop_back();
				delete shape;
				InvalidateRect(hWnd, 0, true);
			}
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_CONTROL:
			gCtrlDown = false;
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
// ������� �� ������ �� ��� ����, ����� ���� �����������
BOOL CALLBACK
AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0/*return code*/);
			return true;
		}
	}
	return false;
}
// �������� ������� - ������ int main() � ���������� ����������:
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	ghAppInst = hInstance;
	
	// ��������� WNDCLASS �������� �������� ������ ����, ������� ���������������� �������� RegisterClass
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"WNDCLASS";
	// ������� RegisterClassEx ������������ ����� ���� ��� ������������ ������������� ��� ������ ������� CreateWindow
	// ��� CreateWindowEx.
	RegisterClass(&wc);

	ghMenu = LoadMenu(ghAppInst, MAKEINTRESOURCE(IDR_MENU1));
	ghMainWnd = CreateWindow(L"WNDCLASS", L"MyPaint", WS_OVERLAPPEDWINDOW, 200, 200, 640, 480, 0, ghMenu, ghAppInst, 0);

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"Create Window - Failed", 0, 0);
		return false;
	}
	// �� ������ �������� ���� ��� ����, �� ���� ����� �� ������������
	// ������ ��� �������
	ShowWindow(ghMainWnd, showCmd);
	UpdateWindow(ghMainWnd);

	// ����� ��������� ��������� MSG ��� ��������� ���������
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, 0, 0, 0))
	{
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg) &&
		    ghRadio == 0 || !IsDialogMessage(ghRadio, &msg) &&
			ghCombo == 0 || !IsDialogMessage(ghCombo, &msg))
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (int i = 0; i < gShapes.size(); i++)
	{
		delete gShapes[i];
	}
	// ���������� ��� ������ � ��
	return (int)msg.wParam;
}

int HandleMenu(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_FILE_EXIT:
		DestroyWindow(ghMainWnd);
		return 0;
	case ID_FILE_ABOUT:
		DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
		return 0;

		// ����������� ���������:
	case ID_PRIMITIVE_LINE: // �������� ����� � ������� ������� �� ��������� �� ������ ������ ������
		CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
		gCurrPrimSel = ID_PRIMITIVE_LINE;
		return 0;

	case ID_PRIMITIVE_RECTANGLE:
		CheckMenuItem(ghMenu, ID_PRIMITIVE_RECTANGLE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
		gCurrPrimSel = ID_PRIMITIVE_RECTANGLE;
		return 0;

	case ID_PRIMITIVE_ELLIPSE:
		CheckMenuItem(ghMenu, ID_PRIMITIVE_ELLIPSE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
		gCurrPrimSel = ID_PRIMITIVE_ELLIPSE;
		return 0;
		// ����� ������:
	case ID_PENCOLOR_BLACK:
		CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_BLACK;
		gLogPen.lopnColor = BLACK;
		return 0;

	case ID_PENCOLOR_WHITE:
		CheckMenuItem(ghMenu, ID_PENCOLOR_WHITE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_WHITE;
		gLogPen.lopnColor = WHITE;
		return 0;

	case ID_PENCOLOR_RED:
		CheckMenuItem(ghMenu, ID_PENCOLOR_RED, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_RED;
		gLogPen.lopnColor = RED;
		return 0;

	case ID_PENCOLOR_GREEN:
		CheckMenuItem(ghMenu, ID_PENCOLOR_GREEN, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_GREEN;
		gLogPen.lopnColor = GREEN;
		return 0;

	case ID_PENCOLOR_BLUE:
		CheckMenuItem(ghMenu, ID_PENCOLOR_BLUE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_BLUE;
		gLogPen.lopnColor = BLUE;
		return 0;
		// ����� ������: 
	case ID_BRUSHCOLOR_BLACK:
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
		gLogBrush.lbColor = BLACK;
		return 0;

	case ID_BRUSHCOLOR_WHITE:
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_WHITE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_WHITE;
		gLogBrush.lbColor = WHITE;
		return 0;

	case ID_BRUSHCOLOR_RED:
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_RED, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_RED;
		gLogBrush.lbColor = RED;
		return 0;

	case ID_BRUSHCOLOR_GREEN:
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_GREEN, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_GREEN;
		gLogBrush.lbColor = GREEN;
		return 0;

	case ID_BRUSHCOLOR_BLUE:
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLUE, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_BLUE;
		gLogBrush.lbColor = BLUE;
		return 0;
		// ����� ������: 
	case ID_PENSTYLE_SOLID:
		CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
		gCurrPenStyleSel = ID_PENSTYLE_SOLID;
		gLogPen.lopnStyle = PS_SOLID;
		return 0;

	case ID_PENSTYLE_DOTTED:
		CheckMenuItem(ghMenu, ID_PENSTYLE_DOTTED, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
		gCurrPenStyleSel = ID_PENSTYLE_DOTTED;
		gLogPen.lopnStyle = PS_DOT;
		return 0;

	case ID_PENSTYLE_DASHED:
		CheckMenuItem(ghMenu, ID_PENSTYLE_DASHED, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
		gCurrPenStyleSel = ID_PENSTYLE_DASHED;
		gLogPen.lopnStyle = PS_DOT;
		return 0;
		// ����� ������: 
	case ID_BRUSHSTYLE_SOLID:
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;
		gLogBrush.lbStyle = BS_SOLID;
		return 0;

	case ID_BRUSHSTYLE_EMPTY:
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_EMPTY, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_EMPTY;
		gLogBrush.lbStyle = BS_NULL;
		return 0;

	case ID_BRUSHSTYLE_DIAGONAL:
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_DIAGONAL, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_DIAGONAL;
		gLogBrush.lbStyle = BS_HATCHED;
		gLogBrush.lbHatch = HS_BDIAGONAL;
		return 0;

	case ID_BRUSHSTYLE_CROSS:
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_CROSS, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_CROSS;
		gLogBrush.lbStyle = BS_HATCHED;
		gLogBrush.lbHatch = HS_CROSS;
		return 0;

	case ID_LINESETTINGS_THIN:
		CheckMenuItem(ghMenu, ID_LINESETTINGS_THIN, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrLineSettings, MF_UNCHECKED);
		gCurrLineSettings = ID_LINESETTINGS_THIN;
		gLogPen.lopnWidth.x = THIN_LINE;
		return 0;

	case ID_LINESETTINGS_NORMAL:
		CheckMenuItem(ghMenu, ID_LINESETTINGS_NORMAL, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrLineSettings, MF_UNCHECKED);
		gCurrLineSettings = ID_LINESETTINGS_NORMAL;
		gLogPen.lopnWidth.x = NORMAL_LINE;
		return 0;

	case ID_LINESETTINGS_THICK:
		CheckMenuItem(ghMenu, ID_LINESETTINGS_THICK, MF_CHECKED);
		CheckMenuItem(ghMenu, gCurrLineSettings, MF_UNCHECKED);
		gCurrLineSettings = ID_LINESETTINGS_THICK;
		gLogPen.lopnWidth.x = THICK_LINE;
		return 0;
	}
}