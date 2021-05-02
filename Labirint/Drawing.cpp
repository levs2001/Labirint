#include "Drawing.h"

static void DrawSquare(HDC hdc, ClassXY coord, ClassXY size, COLORREF color);

void LabWindow::Draw(HDC hdc) const {
	for (const Button& but : buts)
		but.Draw(hdc);
	labirint->Draw(hdc);
}

void Button::Draw(HDC hdc) const {
	DrawSquare(hdc, coord, size, PINK);
	TextOutA(hdc, coord.x, coord.y, text.c_str(), text.size());
}


void Labirint::Draw(HDC hdc) const {
	ClassXY size(cellSize, cellSize);

	for (size_t i = 0; i != sizeM; i++) {
		for (size_t j = 0; j != sizeM; j++) {
			ClassXY coord(j * cellSize, i * cellSize);
			switch (labM[i][j]) {
			case ECellType::BEGIN:
				DrawSquare(hdc, coord, size, GREEN);
				break;
			case ECellType::END:
				DrawSquare(hdc, coord, size, RED);
				break;
			case ECellType::WALL:
				DrawSquare(hdc, coord, size, BLACK);
				break;
			case ECellType::NOWALL:
				DrawSquare(hdc, coord, size, WHITE);
				break;
			case ECellType::WAY:
				DrawSquare(hdc, coord, size, YELLOW);
				break;
			}
 		}
	}
}

static void DrawSquare(HDC hdc, ClassXY coord, ClassXY size, COLORREF color) {
	int left = coord.x;
	int top = coord.y;
	int right = coord.x + size.x;
	int bottom = coord.y + size.y;

	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, left, top, right, bottom);
	DeleteObject(hBrush);
}
