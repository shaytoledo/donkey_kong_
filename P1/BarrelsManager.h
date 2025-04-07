#pragma once
#include <vector>
#include "Barrel.h"
#include "Mario.h"
#include "Utils.h"
#include "GameObjectsManager.h"

class Mario; // forward decleration

// Class that manages all the barrels in the game
class BarrelsManager : public GameObjectsManager<Barrel> {
private:
    static constexpr int maxNumOfBarrels = 5;
    static constexpr int linesGapBetweenBarrels = 6; // Gap in lines (vertical distance) between barrels to control the intensity of the game

public:
    BarrelsManager(Board& board, Screen& screen) :
		GameObjectsManager<Barrel>(board, screen, maxNumOfBarrels, board.getBarrelsStartPos()) {}
	
	// Get the gap between barrels
    int getBarrelsGap() const { return linesGapBetweenBarrels; }
    
	// Check if the manager can add a new barrel
    bool isAddingObjectEnabled() override {
        return getObjects().empty() || (getObjects().size() <= getMaxNumOfObjects()) && (getObjects().back()->getY() > getBarrelsGap());
    }

    // Handle the explosion of barrels and manage damage to nearby objects or characters
    bool handleBarrelDemaged(Mario& mario);
	
    

};
