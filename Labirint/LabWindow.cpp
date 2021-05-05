#include "LabWindow.h"
#include "Labirint.h"

void LabWindow::Init(size_t cellSize) {
	size_t labSize = SCREEN_HEIGHT/cellSize;
	cellSize = SCREEN_HEIGHT / labSize; //To make square cells and field
	labirint = new Labirint(labSize, cellSize);

	ClassXY butCoord(BUT_COORD_X, 0);
	ClassXY butSize(BUT_SIZE_X, BUT_SIZE_Y);
	std::string butText[BUT_COUNT] = { "Random", "NoWall", "Wall", "Begin", "End" };
	ECellType mode[BUT_COUNT] = { ECellType::RAND, ECellType::NOWALL, ECellType::WALL, ECellType::BEGIN, ECellType::END };

	for (size_t i = 0; i != BUT_COUNT; i++) {
		buts.push_back(Button(butSize, butCoord, butText[i], mode[i]));
		butCoord.y += BUT_SIZE_Y;
	}

	this->mode = ECellType::RAND;
}


bool LabWindow::Click(const ClassXY& clCoord) {
	for (const Button& but : buts) {
		if (but.CheckClick(clCoord)) {
			mode = but.GetMode();
			if (mode == ECellType::RAND) {
				labirint->MakeRandom();
				return REDRAW;
			}
			return NOT_REDRAW;
		}
	}

	labirint->Change(mode, clCoord);
	return REDRAW;
}

LabWindow::~LabWindow() {
	delete labirint;
	buts.clear();
}


bool Button::CheckClick(const ClassXY& clCoord) const{
	if (clCoord.x > coord.x && clCoord.x < coord.x + size.x)
		if (clCoord.y > coord.y && clCoord.y < coord.y + size.y)
			return true;
	return false;
}