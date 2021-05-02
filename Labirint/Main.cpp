#include <Windows.h>
#include "LabWindow.h"
#include "Drawing.h"

#define CELL_SIZE 100

char szClassName[] = "Window1";
HWND hWnd;

LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);
ATOM registerMyClass(HINSTANCE hInstance);
int createMyWindow(HINSTANCE hInstance, int nCmdShow);

LabWindow* labWindow;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	MSG msg;
	labWindow = new LabWindow();
	labWindow->Init(CELL_SIZE);

	createMyWindow(hInstance, nCmdShow);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE: {
		SetTimer(hWnd, ID_TIMER_DRAWING, PERIOD_TIMER_DRAWING, NULL);
		break;
	}
	case WM_TIMER: {
		switch (wParam) {
		case ID_TIMER_DRAWING:
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		break;
	}
	case WM_DESTROY: {
		KillTimer(hWnd, ID_TIMER_DRAWING);
		delete labWindow;
		PostQuitMessage(0);
		break;
	}
	case WM_LBUTTONDOWN: {
		ClassXY clCoord(LOWORD(lParam), HIWORD(lParam));
		labWindow->Click(clCoord);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		labWindow->Draw(hdc);
		EndPaint(hWnd, &ps);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int createMyWindow(HINSTANCE hInstance, int nCmdShow) {
	registerMyClass(hInstance);

	hWnd = CreateWindow(szClassName, "Labirint", WS_OVERLAPPEDWINDOW, 100, 30, SCREEN_WIDTH, SCREEN_HEIGHT + 50, NULL, NULL, hInstance, NULL);
	COLORREF bkcolor = RGB(255, 255, 255);
	HBRUSH bkbrush = CreateSolidBrush(bkcolor);
	SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)bkbrush);
	if (!hWnd) { return 0; }
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

ATOM registerMyClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	return RegisterClass(&wc);
}