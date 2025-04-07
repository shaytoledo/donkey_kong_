#pragma once

#include <iostream>
#include "BarrelsManager.h"
#include "utils.h"
#include "Board.h"
#include "Entity.h"
#include "GhostsManager.h"
#include "Ghost.h"
#include "Barrel.h"
 
class BarrelsManager;
class GhostsManager;

class Mario : public Entity {

    // Constants for user input keys and their corresponding directions
    static constexpr char keys[] = { 's', 'x', 'd', 'a', 'w' };  // Movement keys for Mario
    static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);  // Number of valid keys
	char key = keys[0];  // Current key pressed by the user
    bool holdingHammer = false;
	bool usingHammer = false;
    int lives = 3;
    static constexpr int DEAD_LIVES = 0;  // Lives count indicating Mario is "dead"
    static constexpr char MARIO_SYMBOL = '@';
    static constexpr char HAMMER_SYMBOL = 'p';
    static constexpr Colors HAMMER_COLOR = Colors::PINK;
	static constexpr Direction DEFAULT_HAMMER_DIR = { 1, 0 }; // Default direction for the hammer
    static constexpr int HAMMER_HIT_RANGE = 2;
    static constexpr int maxFallHeight = 5;  // Maximum fall height before Mario is considered fallen

public:

    Mario(Board& board, Screen& screen, Coord marioStartPos) : Entity(board, screen, marioStartPos, MARIO_SYMBOL, maxFallHeight) {}

    // Check if Mario's lives have reached zero
    bool isDead() { return lives <= DEAD_LIVES; }

    // Decrease Mario's lives by 1
    void decreaseLives() { lives--; }

    // Add to Mario lives
    void addLives(int howMuch) { lives+= howMuch; }

    // Handle a key press event to update Mario's direction
    void keyPressed(char key);
	
    // Update Mario's direction based on the given key
    void updateDirection() override;

    // Move Mario based on his current direction
    void move() override;
	
    // Send Mario back to his starting position
    void sendToStart() { setPos(startPos); }

	// Get the key pressed by the user
	char getKey() const { return key; }

	// Set the key pressed by the user
    void setKey(char _key) { key = _key; }

    // Get the current number of lives Mario has
    int getLives() { return lives; }

	// get the hammer symbol
	char getHammerSymbol() const { return HAMMER_SYMBOL; }
    
	// get the hammer color
    Colors getHammerColor() const { return HAMMER_COLOR; }
    
	// get the hammer hit range
	int getHammerHitRange() const { return HAMMER_HIT_RANGE; }
    
	// get if mario has a hammer
	bool isHoldingHammer() const { return holdingHammer; }

	// set hasHammer
	void setHoldingHammer(bool _holdingHammer) { this->holdingHammer = _holdingHammer; }
	
    // get if mario is using the hammer
    bool isUsingHammer() const { return isHoldingHammer() && usingHammer; }
	
    // set useHammer
    void setUseingHammer(bool _usingHammer) { this->usingHammer = _usingHammer; }
	
    // get the default hammer direction
	Direction getDefaultHammerDir() const { return DEFAULT_HAMMER_DIR; }

    //Make mario get to the floor 
    void getDown() { draw(); Sleep(50); erase(); }

    //Make mario climb on the ladder
    void climbLadder();

    // Make mario jump two char up
    void handleMotionJump();
    	
	// erase Mario from the board
    void erase() const override;

    // draw Mario on the board
    void draw() const override;
	
    // draw the hammer on the board
    void drawHammer() const;
	
    // erase the hammer from the board
    void eraseHammer() const {
		Coord hammerPos = getHammerPos();
		gotoxy(hammerPos);
        pScreen->coloredCharPrint(pBoard->getChar(hammerPos), pScreen->getColor("board"));
    }

	// Get the hammer direction
    Direction getHammerDir() const;

    // Get the hammer position
    Coord getHammerPos() const {
		Direction currHammerDir = getHammerDir();
        return getPos() += currHammerDir;
    }
};
