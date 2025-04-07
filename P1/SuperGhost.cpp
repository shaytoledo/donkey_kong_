#pragma once
#include "SuperGhost.h"

// Update the direction of the ghost
void SuperGhost::updateDirection() {
	
	if (isFollowingTarget()) {
		updateDirectionByTarget();
	}
	else {
		Ghost::updateDirection();
	}
}

// Update the direction of the ghost based on the target position
void SuperGhost::updateDirectionByTarget() {
	Coord targetRelatedPos = getTargetPos() - getPos();
	
	// mario above the ghost
	if ((targetRelatedPos.y < 0) && (pBoard->isLadder(getPos()))) {
		setDir(Direction::UP);
	}

	// mario at the same height level of the ghost
	else {
		if (targetRelatedPos.x > 0) {
			setDir(Direction::RIGHT);
		}
		else if (targetRelatedPos.x < 0) {
			setDir(Direction::LEFT);
		}
		else {
			setDir(Direction::STAY);
		}
	}
}

// move the ghost
void SuperGhost::move() {
	Coord newPos = getPos() + getDir();
	
	if (getDir() == Direction::UP && pBoard->isFloor(newPos)) { // if next pos is the end of the ladder
		newPos += Direction::UP; // skip over the ceilling 
	}
	setPos(newPos);
}