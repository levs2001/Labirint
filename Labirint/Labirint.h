#pragma once
#include "LabWindow.h"

struct WayNode {
	WayNode() : visited(false), steps(0) {}
	WayNode(size_t steps) {
		this->steps = steps;
		visited = false;
		coord = { -1, -1 };
	};

	
	bool visited;
	size_t steps;
	ClassXY coord;
};


class Labirint {
public:
	Labirint(size_t sizeM, size_t cellSize);
	void Draw(HDC hdc) const;
	void Change(ECellType mode, const ClassXY& clCoord);
	void MakeRandom();

	ECellType GetCellType(size_t i, size_t j) {
		return labM[i][j];
	}

	size_t GetSize() {
		return sizeM;
	}

	~Labirint();

private:
	ECellType** labM;
	std::vector<ClassXY> way;
	size_t sizeM;
	size_t cellSize;
	ClassXY begin;
	ClassXY end;

	void MakeNewWay();
	void PointWay(WayNode** wayM);
	void CleanWay();
	ClassXY GetMatrixCoord(const ClassXY& coord, bool& notInMatrix) const;

};