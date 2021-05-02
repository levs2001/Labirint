#include "Labirint.h"
#include "LabWindow.h"
#include <random>

Labirint::Labirint(size_t sizeM, size_t cellSize) : sizeM(sizeM), cellSize(cellSize) {
	labM = new ECellType * [sizeM];
	for (size_t i = 0; i < sizeM; i++) {
		labM[i] = new ECellType[sizeM];
		for (size_t j = 0; j < sizeM; j++)
			labM[i][j] = ECellType::NOWALL;
	}
}

void Labirint::Change(ECellType mode, const ClassXY& clCoord) {
	if (mode == ECellType::RAND)
		return;

	ClassXY mCoord;
	try {
		mCoord = GetMatrixCoord(clCoord);
	}
	catch (std::string err) {
		return;
	}

	labM[mCoord.y][mCoord.x] = mode;
	if (mode == ECellType::BEGIN && !(mCoord == begin)) {
		labM[begin.y][begin.x] = ECellType::NOWALL;
		begin = mCoord;
	}
	else if (mode == ECellType::END && !(mCoord == begin)) {
		labM[end.y][end.x] = ECellType::NOWALL;
		end = mCoord;
	}

}

void Labirint::MakeRandom() {
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

ClassXY Labirint::GetMatrixCoord(const ClassXY& coord) const {
	ClassXY mCoord;
	mCoord.x = coord.x / cellSize;
	mCoord.y = coord.y / cellSize;

	if (mCoord.x > sizeM || mCoord.y > sizeM) {
		throw (std::string)"Not in labirint";
	}

	return mCoord;
}

Labirint::~Labirint() {
	for (size_t i = 0; i < sizeM; i++)
		delete[] labM[i];
	delete[] labM;
}