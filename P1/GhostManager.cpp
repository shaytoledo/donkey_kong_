#pragma once
#include "GhostsManager.h"


// Add all ghosts to the vector
void GhostsManager::addAllObjects(Entity& mario) {
	if (!getWereAllObjectsAdded()) {
		
		std::vector<Coord>& _startPositions = getStartPositions();
		std::vector<Coord>& _superGhostsStartPos = getSuperGhostsStartPos();

		for (int i = 0; i < getMaxNumOfObjects(); i++) {
			Coord _startPos = _startPositions.at(i);
			setStartPos(_startPos);
			
			auto it = find(_superGhostsStartPos.begin(), _superGhostsStartPos.end(), _startPos);
			if (it != _superGhostsStartPos.end()) {
				addSuperGhosts(mario);
				continue;
			}
			addObject();
		}
	}
	setWereAllObjectsAdded(true);
}

void GhostsManager::addSuperGhosts(Entity& mario) {
	if (isAddingObjectEnabled()) {
		std::shared_ptr<SuperGhost> newObject = std::make_shared<SuperGhost>(getBoard(), getScreen(), getStartPos());
		newObject->setTarget(mario);
		getObjects().push_back(newObject);
	}
}

// handle the demage of the ghost corresponds to mario
bool GhostsManager::handleGhostDemaged(Mario& mario) {
	auto& ghosts = getObjects();
	Coord marioPos = mario.getPos();

	for (auto ghost = ghosts.begin(); ghost != ghosts.end(); ) {
		// check if mario hit ghost with hammer and if the ghost is in the range of it
		if (mario.isUsingHammer()) {
			int distanceFromMario = marioPos.x - (*ghost)->getX();
			if (marioPos.y == (*ghost)->getY() && 
				(abs(distanceFromMario) <= mario.getHammerHitRange()) &&
				mario.getHammerDir().x * distanceFromMario <= 0) { // if the hammer aimed to the ghost
				ghost = ghosts.erase(ghost);
				continue;
			}
		}
		if ((*ghost)->at(marioPos)) {
			(*ghost)->drawExplotion();
			(*ghost)->drawExplotion();
			ghost = ghosts.erase(ghost);
			return true;
		}
		else {
			++ghost;
		}
	}
	return false;
}

// Move all active objects currently in play
void GhostsManager::moveObjects() {
	auto& ghosts = getObjects();
	getNextPositions().clear(); // Clear the dictionary at the start of each iteration

	for (auto& ghost : ghosts) {
		if (handleGhostsCollision(*ghost)) {
			ghost->updateDirection();
		}
	}

	for (auto& ghost : ghosts) {
		ghost->erase();
		ghost->move();
	}
}

// handle the collostion between ghosts
bool GhostsManager::handleGhostsCollision(Ghost& ghost) {
	Coord currPos = ghost.getPos();
	Direction currDir = ghost.getDir();
	Direction oppositeDir = { currDir.x * -1, currDir.y };
	Coord nextPos = { currPos.x + currDir.x, currPos.y + currDir.y };
	
	Board* pBoard = &getBoard();
	if (pBoard->isGhost(nextPos)) { // in case of two ghosts that will swap their positions in next move
		ghost.setDir(oppositeDir);
		return false; // Skip updating direction for this ghost in moveObjects()

	}
	else { // in case of two ghosts that their next positions is the same
		auto& _nextPositions = getNextPositions();
		auto it = _nextPositions.find(nextPos);

		if (it != _nextPositions.end()) { // check if there is a ghost in the next position
			ghost.setDir(oppositeDir);// set the opposite direction of cuurent ghoat to the current ghost

			Ghost* otherGhost = it->second;
			otherGhost->setDir(currDir); // set the original direction of the current ghost to the direction of the other ghost
			return false; // Skip updating direction for this ghost in moveObjects()
		}
		else {
			// No collision, add to nextPositions
			_nextPositions[nextPos] = &ghost;
			return true; // Continue updating direction for this ghost in moveObjects()
		}
	}
}
