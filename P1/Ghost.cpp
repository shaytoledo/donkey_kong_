#include "Ghost.h"

void Ghost::updateDirection() { 
	Direction currDir = getDir();
    Coord currPos = getPos();
	Coord nextFloor = { currPos.x + currDir.x, currPos.y + currDir.y + 1 };
	Coord nextPos = currPos + currDir;
	Coord oneBelow = currPos + Direction::DOWN;
	Coord oneRight = currPos + Direction::RIGHT;
	Coord oneLeft = currPos + Direction::LEFT;


	if (pBoard->isAir(oneBelow)) { // apply gravitation
		setDir(Direction::DOWN);
		inFall = true;
	}
	else if (inFall) {
		if (!pBoard->isEdge(oneRight) && !pBoard->isFloor(oneRight)) {
			setDir(Direction::RIGHT);
		}
		else if (!pBoard->isEdge(oneLeft) && !pBoard->isFloor(oneLeft)) {
			setDir(Direction::LEFT);
		}
		else {
			setDir(Direction::STAY);
		}
		inFall = false;
	}
	else {
		if ((rand() % 100) < 5 || pBoard->isAir(nextFloor) || pBoard->isEdge(nextPos)) {
			Direction nextDir = { currDir.x * -1, currDir.y };
			setDir(nextDir);
		}
	}
}