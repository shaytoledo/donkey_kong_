#pragma once
#include "Ghost.h"
#include "Mario.h"
class Mario; // forward decleration

class SuperGhost : public Ghost {

private:

	Entity* target = nullptr; // the object fie the ghost to follow on
	bool _isFollowingTarget = false;
	static constexpr char SUPER_GHOST_SYMBOL = 'X';

public:

	SuperGhost(Board& board, Screen& screen, Coord startPos) : Ghost(board, screen, startPos) {
		setSymbol(SUPER_GHOST_SYMBOL);
	}
	
	// Set the mario to follow after
	void setTarget(Entity& trackingTarget) { this->target = &trackingTarget; }

	// Get the position of the target on the board
	Coord getTargetPos() const { return target->getPos(); }
	
	// Check if the super ghost should follow the target (i.e if the height of the target >= height of the super ghost)
	bool isFollowingTarget() const { return this->getY() >= target->getY(); }

	// Set the flag that indicates if the super ghost should follow the target
	void setIsFollowingTarget(bool flag) { this->_isFollowingTarget = flag; }
	
	// Update the direction of the super ghost
	void updateDirection() override;

	// Update the direction of the super ghost based on the target
	void updateDirectionByTarget();
	
	// Move the super ghost
	void move() override;
};