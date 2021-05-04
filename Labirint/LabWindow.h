#pragma once

#include <Windows.h>
#include <String>
#include <Vector>
//#include "Labirint.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 900
#define BUT_COUNT 5
#define BUT_SIZE_X 100
#define BUT_SIZE_Y 20
#define BUT_COORD_X 900

enum class ECellType {
	NOWALL,
	WALL,
	BEGIN,
	END,
	WAY,
	RAND
};

class ClassXY {
public:
	int x;
	int y;
	ClassXY(int x, int y) :x(x), y(y) {}
	ClassXY(const ClassXY& size) :x(size.x), y(size.y) {};
	ClassXY() : x(0), y(0) {};
	friend bool operator== (const ClassXY& c1, const ClassXY& c2) {
		return (c1.x == c2.x && c1.y == c2.y);
	}

	friend bool operator!= (const ClassXY& c1, const ClassXY& c2) {
		return (c1.x != c2.x || c1.y != c2.y);
	}

};

class Button {
public:
	Button(ClassXY size, ClassXY coord, std::string text, ECellType getMode) : size(size), coord(coord), text(text), getMode(getMode) {}
	void Draw(HDC hdc) const;
	bool CheckClick(const ClassXY& clCoord) const;
	ECellType GetMode() const {
		return getMode;
	}

private:
	ClassXY size;
	ClassXY coord;
	std::string text;
	ECellType getMode;
};

class Labirint;

class LabWindow {
public:
	void Init(size_t cellSize);
	void Draw(HDC hdc) const;
	void Click(const ClassXY& clCoord);
	~LabWindow();

private:
	std::vector<Button> buts;
	Labirint* labirint;
	ECellType mode;
};

