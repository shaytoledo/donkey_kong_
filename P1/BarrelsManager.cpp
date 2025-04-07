#include "BarrelsManager.h"


// Handle the explosion of barrels and manage damage to nearby objects or characters
bool BarrelsManager::handleBarrelDemaged(Mario& mario) {
	auto& barrels = getObjects();
	Coord marioPos = mario.getPos();

	for (auto barrel = barrels.begin(); barrel != barrels.end(); ) {
		// check if mario hit ghost with hammer and if the ghost is in the range of it
		if (mario.isUsingHammer()) {
			int distanceFromMario = marioPos.x - (*barrel)->getX();
			if (marioPos.y == (*barrel)->getY() &&
				(abs(distanceFromMario) <= mario.getHammerHitRange()) &&
				mario.getHammerDir().x * distanceFromMario <= 0) { // if the hammer aimed to the barrel
				barrel = barrels.erase(barrel);
				continue;
			}
		}
		if ((*barrel)->at(marioPos)) {
			(*barrel)->drawExplotion();
			barrel = barrels.erase(barrel);
			return true;
		}
		else if ((*barrel)->isExploding()) {
			(*barrel)->drawExplotion();
			bool isInBlastRange = (*barrel)->isInBlastRange(marioPos);
			barrel = barrels.erase(barrel);
			if (isInBlastRange) {
				return true;
			}
		}
		else {
			++barrel;
		}
	}
	return false;
}