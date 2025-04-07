#include "Barrel.h"

// Update the direction of the barrel based on the type of floor it is on
void Barrel::updateDirection() { // change direction by the floor type
	char currFloor = pBoard->getChar({ pos.x, pos.y + 1 });
	for (size_t i = 0; i < numFloors; i++) {
		if (currFloor == floors[i]) {
			if (directions[i].x) { // remember the last direction in case the barrel falling or moving on flat floor.
				lastDir = directions[i];
				initFallCounter();
			}
			else { fallCounter++; } // count highet fall
			dir = directions[i];
			return;
		}
	}
}

// Move the barrel according to the floor type and direction
void Barrel::move() {
	int newX = pos.x + dir.x;
	int newY = pos.y + dir.y;

	if (pBoard->isStrightFloor({ pos.x, pos.y + 1 })) {
		pos.x += lastDir.x;
	}
	else {
		pos.x = newX;
		pos.y = newY;
	}
}