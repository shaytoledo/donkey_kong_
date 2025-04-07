#pragma once

#include <iostream>
#include "utils.h"
#include "Board.h"
#include "Screen.h"
#include "Entity.h"

class Barrel : public Entity {
private:

	// Types of floors the barrel can interact with
	static constexpr char floors[] = { '=', ' ', '>', '<'};
	static constexpr size_t numFloors = sizeof(floors) / sizeof(floors[0]); // Number of floor types

	// Explosion properties
	static constexpr int explotionSpace = 2;  // Range of the explosion

	// Barrel's attributes
	 Coord barrelStartPos = { 11, 5 };  // Initial position of the barrel
	
	static constexpr char BARREL_SYMBOL = 'O';
	bool isFallSession = false;  // Flag to indicate if the barrel is currently falling
	static constexpr int maxFallHighet = 8;   // Maximum fall height before the barrel explodes


public:
    
    Barrel(Board& board, Screen& screen, Coord barrelStartPos) :barrelStartPos(barrelStartPos), Entity(board, screen, barrelStartPos, BARREL_SYMBOL, maxFallHighet) {}
    
    // Check if a position is within the blast range of the barrel
    bool isInBlastRange(Coord curPos) const { return (abs(pos.x - curPos.x) <= explotionSpace && abs(pos.y - curPos.y) <= explotionSpace); }

    // Get the explosion range of the barrel
    int getExplotionSpace() const { return explotionSpace; }

    // Check if the barrel hit the edge of the screen
    bool wasHitTheEdge() const { return (pBoard->isEdge({ pos.x, pos.y + 1 })); }

    // Determine if the barrel should explode
    bool isExploding() { return wasFellTooHigh() || wasHitTheEdge(); }

    // Update the direction of the barrel based on the type of floor it is on
    void updateDirection() override;

    // Move the barrel according to the floor type and direction
    void move() override;
};
