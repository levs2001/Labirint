#include "Labirint.h"
#include "LabWindow.h"
#include <random>
#include <queue>
#include <iostream>

static bool IsOutOfMatrix(ClassXY cell, size_t sizeM);
static bool IsOutOfMatrix(size_t x, size_t y, size_t sizeM);
static void PushNearCells(WayNode& wNode, std::queue<WayNode*>& plan, WayNode** wayM, Labirint* lab);
static void PrintWays(WayNode** wayM, size_t sizeM);
static void SetNearestIfNear(WayNode* checkingNode, WayNode*& nearest, const ClassXY& destination);
static ClassXY FindNearest(const ClassXY& begin, const ClassXY& destination, WayNode** wayM, size_t sizeM, bool& arrived);

Labirint::Labirint(size_t sizeM, size_t cellSize) : sizeM(sizeM), cellSize(cellSize) {
	labM = new ECellType * [sizeM];
	for (size_t i = 0; i < sizeM; i++) {
		labM[i] = new ECellType[sizeM];
		for (size_t j = 0; j < sizeM; j++)
			labM[i][j] = ECellType::NOWALL;
	}
	begin = ClassXY(-1, -1);
	end = ClassXY(-1, -1);
}

void Labirint::Change(ECellType mode, const ClassXY& clCoord) {
	if (mode == ECellType::RAND)
		return;

	ClassXY mCoord;
	bool notInMatrix = false;
	mCoord = GetMatrixCoord(clCoord, notInMatrix);
	if (notInMatrix)
		return;
	if (labM[mCoord.y][mCoord.x] != ECellType::WALL || mode == ECellType::NOWALL) {
		labM[mCoord.y][mCoord.x] = mode;

		bool newWay = false;
		if (mode == ECellType::BEGIN && !(mCoord == begin)) {
			if (!IsOutOfMatrix(begin, sizeM))
				labM[begin.y][begin.x] = ECellType::NOWALL;
			begin = mCoord;
			newWay = true;
		}
		else if (mode == ECellType::END && !(mCoord == begin)) {
			if (!IsOutOfMatrix(end, sizeM))
				labM[end.y][end.x] = ECellType::NOWALL;
			end = mCoord;
			newWay = true;
		}
		if (newWay && !IsOutOfMatrix(begin, sizeM) && !IsOutOfMatrix(end, sizeM)) {
			MakeNewWay();
		}
	}
}

void Labirint::PointWay(WayNode** wayM) {
	//For case without opportunity to make way
	if (wayM[end.y][end.x].steps == 0)
		return;

	bool arrived = false;
	ClassXY nearest = FindNearest(end, begin, wayM, sizeM, arrived);
	do {
		if (!arrived) {
			labM[nearest.y][nearest.x] = ECellType::WAY;
			way.push_back(nearest);
		}
		nearest = FindNearest(nearest, begin, wayM, sizeM, arrived);
	} while (!arrived);
}

void Labirint::MakeNewWay() {
	CleanWay();

	WayNode** wayM = new WayNode * [sizeM];
	for (size_t i = 0; i < sizeM; i++) {
		wayM[i] = new WayNode[sizeM];
		for (size_t j = 0; j < sizeM; j++) {
			wayM[i][j].steps = 0;
			wayM[i][j].visited = false;
			wayM[i][j].coord = ClassXY(j, i);
		}
	}

	std::queue<WayNode*> plan;
	PushNearCells(wayM[begin.y][begin.x], plan, wayM, this);

	while (!plan.size() == 0) {
		WayNode& wayNode = *plan.front();
		plan.pop();
		PushNearCells(wayNode, plan, wayM, this);
	}

	//PrintWays(wayM, sizeM);
	PointWay(wayM);

	for (size_t i = 0; i < sizeM; i++)
		delete[] wayM[i];
	delete[] wayM;
}

void Labirint::CleanWay() {
	for (ClassXY& cell : way) {
		if(labM[cell.y][cell.x] == ECellType::WAY)
		labM[cell.y][cell.x] = ECellType::NOWALL;
	}
	way.clear();
}

void Labirint::MakeRandom() {
	begin = ClassXY(-1, -1);
	end = ClassXY(-1, -1);

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> randRow(1, 10);

	for (size_t i = 0; i < sizeM; i++) {
		for (size_t j = 0; j < sizeM; j++) {
			size_t num = randRow(gen);
			labM[i][j] = num < 9 ? ECellType::NOWALL : ECellType::WALL;
		}
	}
}

ClassXY Labirint::GetMatrixCoord(const ClassXY& coord, bool& notInMatrix) const {
	notInMatrix = false;
	ClassXY mCoord;
	mCoord.x = coord.x / cellSize;
	mCoord.y = coord.y / cellSize;

	if (IsOutOfMatrix(mCoord, sizeM)) {
		notInMatrix = true;
	}

	return mCoord;
}

Labirint::~Labirint() {
	for (size_t i = 0; i < sizeM; i++)
		delete[] labM[i];
	delete[] labM;
}


static bool IsOutOfMatrix(ClassXY cell, size_t sizeM) {
	if (cell.x >= sizeM || cell.y >= sizeM || cell.x < 0 || cell.y < 0) {
		return true;
	}
	return false;
}

static bool IsOutOfMatrix(size_t x, size_t y, size_t sizeM) {
	if (x >= sizeM || y >= sizeM || x < 0 || y < 0) {
		return true;
	}
	return false;
}

static void PushNearCells(WayNode& wNode, std::queue<WayNode*>& plan, WayNode** wayM, Labirint* lab) {
	if (!wNode.visited) {
		if (wNode.coord.y - 1 >= 0 && !wayM[wNode.coord.y - 1][wNode.coord.x].visited
			&& (lab->GetCellType(wNode.coord.y - 1, wNode.coord.x) == ECellType::NOWALL || lab->GetCellType(wNode.coord.y - 1, wNode.coord.x) == ECellType::END)) {
			plan.push(&wayM[wNode.coord.y - 1][wNode.coord.x]);
			plan.back()->steps = wNode.steps + 1;
		}
		if (wNode.coord.x + 1 < lab->GetSize() && !wayM[wNode.coord.y][wNode.coord.x + 1].visited
			&& (lab->GetCellType(wNode.coord.y, wNode.coord.x + 1) == ECellType::NOWALL || lab->GetCellType(wNode.coord.y, wNode.coord.x + 1) == ECellType::END)) {
			plan.push(&wayM[wNode.coord.y][wNode.coord.x + 1]);
			plan.back()->steps = wNode.steps + 1;
		}
		if (wNode.coord.y + 1 < lab->GetSize() && !wayM[wNode.coord.y + 1][wNode.coord.x].visited
			&& (lab->GetCellType(wNode.coord.y + 1, wNode.coord.x) == ECellType::NOWALL || lab->GetCellType(wNode.coord.y + 1, wNode.coord.x) == ECellType::END)) {
			plan.push(&wayM[wNode.coord.y + 1][wNode.coord.x]);
			plan.back()->steps = wNode.steps + 1;
		}
		if (wNode.coord.x - 1 >= 0 && !wayM[wNode.coord.y][wNode.coord.x - 1].visited &&
			(lab->GetCellType(wNode.coord.y, wNode.coord.x - 1) == ECellType::NOWALL || lab->GetCellType(wNode.coord.y, wNode.coord.x - 1) == ECellType::END)) {
			plan.push(&wayM[wNode.coord.y][wNode.coord.x - 1]);
			plan.back()->steps = wNode.steps + 1;
		}
		wNode.visited = true;
	}
}

static void PrintWays(WayNode** wayM, size_t sizeM) {
	for (size_t i = 0; i < sizeM; i++) {
		for (size_t j = 0; j < sizeM; j++) {
			std::cout << wayM[i][j].steps << "\t";
		}
		std::cout << std::endl;
	}
}

static void SetNearestIfNear(WayNode* checkingNode, WayNode*& nearest, const ClassXY& destination) {
	if (checkingNode->steps == 0) {
		if (checkingNode->coord == destination) {
			throw (std::string)"Arrived to destination";
		}
	}
	else if (checkingNode->steps < nearest->steps) {
		nearest = checkingNode;
	}
}

static ClassXY FindNearest(const ClassXY& begin, const ClassXY& destination, WayNode** wayM, size_t sizeM, bool& arrived) {
	WayNode* nearest = &WayNode(UINT32_MAX);

	try {
		if (!IsOutOfMatrix(begin.x, begin.y - 1, sizeM))
			SetNearestIfNear(&wayM[begin.y - 1][begin.x], nearest, destination);
		if (!IsOutOfMatrix(begin.x + 1, begin.y, sizeM))
			SetNearestIfNear(&wayM[begin.y][begin.x + 1], nearest, destination);
		if (!IsOutOfMatrix(begin.x, begin.y + 1, sizeM))
			SetNearestIfNear(&wayM[begin.y + 1][begin.x], nearest, destination);
		if (!IsOutOfMatrix(begin.x - 1, begin.y, sizeM))
			SetNearestIfNear(&wayM[begin.y][begin.x - 1], nearest, destination);
	}
	catch (std::string arrivedStr) {
		arrived = true;
		return nearest->coord;
	}

	return nearest->coord;
}
