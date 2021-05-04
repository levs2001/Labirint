#pragma once
#include "LabWindow.h"


class Labirint {
public:
	Labirint(size_t sizeM, size_t cellSize);
	void Draw(HDC hdc) const;
	void Change(ECellType mode, const ClassXY& clCoord);
	void MakeRandom();
	void MakeNewWay();
	
	ECellType GetCellType(size_t i, size_t j) {
		return labM[i][j];
	}

	size_t GetSize() {
		return sizeM;
	}

	~Labirint();

private:
	ECellType** labM;
	size_t sizeM;
	size_t cellSize;
	ClassXY begin;
	ClassXY end;

	ClassXY GetMatrixCoord(const ClassXY& coord, bool& notInMatrix) const;
};