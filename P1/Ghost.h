#pragma once
#include "Entity.h"
#include "Board.h"
#include "Screen.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>

class Ghost : public Entity {
private:
	// Types of floors the barrel can interact with
	static constexpr char floors[] = { '=', ' ', '>', '<' };
	static constexpr size_t numFloors = sizeof(floors) / sizeof(floors[0]); // Number of floor types
	
	static constexpr char GHOST_SYMBOL = 'x';
	static constexpr int maxFallHighet = 0;   // Maximum fall height before the ghost explodes
	bool inFall = false;

public:
	Ghost(Board& board, Screen& screen, Coord startPos) : Entity(board, screen, startPos, GHOST_SYMBOL, maxFallHeight) { setDir({ 1,0 }); } // arbitrary starting direction

	// update the direction of the ghost based on probability, mario or the end of the floor.
	void updateDirection() override;
};
