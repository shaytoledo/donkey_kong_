#pragma once

#include <iostream>
#include "utils.h"
#include "Board.h"
#include "Screen.h"
#include "string"

class Entity {
protected:

	Board* pBoard = nullptr;
	Screen* pScreen = nullptr; 

	// Movement attributes
	static constexpr Direction directions[] = { {0, 0}, {0, 1}, {1, 0}, {-1, 0} , {0, -1} };  // Directions: stay, down, right, left, up
	Coord startPos;
	Coord pos; 
	Direction dir;
	Direction lastDir;
	int fallCounter = 0;
	int maxFallHeight;

	// appearance attributes
	char symbol = '\0';
	Colors color = Colors::WHITE;
	
	// Explosion properties
	static constexpr int explotionHeighet = 2;
	static constexpr int explotionWidth = 3;
	static constexpr char explotionDraw[][explotionWidth + 1] = { "-*-", "\\|/" };

public:
	Entity() = default;

	Entity(Board& board, Screen& screen, Coord startPos, char symbol, int maxFallHeight) :
		startPos(startPos), pos(startPos), dir(Direction::STAY), lastDir(dir), maxFallHeight(maxFallHeight) {
		setBoard(board);
		setSymbol(symbol);
		setColors(screen);
		setStartPos(startPos);
	}

	virtual ~Entity() = default;

	// Get the x coordinates of the Entity
	int getX() const { return this->pos.x; }
	
	// Get the y coordinates of the Entity
	int getY() const { return this->pos.y; }
	
	// Set the x coordinates of the Entity
	void setX(int x) { this->pos.x = x; }
	
	// set the y coordinates of the Entity
	void setY(int y) { this->pos.y = y; }
	
	// Get the position of the Entity
	Coord getPos() const { return this->pos; }

	// Set the Position of the Entity
	void setPos(Coord newPos) { this->pos = newPos; }

	// Get the current direction of Mario
	Direction getDir() const { return this->dir; }
	
	// Get the x direction of Entity
	int getDirX() const { return this->dir.x; }
	
	// Get the y direction of Entity
	int getDirY() const { return this->dir.y; }
	
	// Set the direction of the Entity
	void setDir(Direction nextDir) { this->dir = nextDir; }

	// Get the last Direction of Entity
	Direction getLastDir() const { return this->lastDir; }

	// Set the board reference that mario will interact with
	void setBoard(Board& board) { this->pBoard = &board; }
	
	// Get a pointer to the Board
	Board* getBoard() const { return this->pBoard; }
	
	// Set the color of the entity
	void setColors(Screen& screen) { 
		this->pScreen = &screen;
		this->color = pScreen->getColor(this->getSymbol());
	}

	// Get the point color
	Colors getColor() const { return this->color; }
	
	// Set the starting position of the Entity
	void setStartPos(Coord newStartPos) { this->startPos = newStartPos; }
	
	// Set the symbol of the Entity
	void setSymbol(char newSymbol) { this->symbol = newSymbol; }

	// Get the symbol of the Entity
	char getSymbol() const { return this->symbol; }
	
	// Get the fall counter of the Entity
	int getFallCounter() const { return this->fallCounter; }

	// Set the fall counter of the Entity
	void setFallCounter(int newFallCounter) { this->fallCounter = newFallCounter; }
	
	// Initialize the fall counter of the Entity
	void initFallCounter() { this->setFallCounter(0); }
	
	// Get the maximum fall height of the Entity
	int getMaxFallHeight() const { return this->maxFallHeight; }

	// Check if the Entity is at a given position
	bool at(Coord givenPos) const { return (givenPos.x - getX()) == 0 && (givenPos.y - getY()) == 0; }

	// Update direction
	virtual void updateDirection() = 0;

	// Move the Entity according to the direction
	virtual void move() {
		setPos(this->pos + this->dir);
	}
	
	// Check if the entity fell from too much height
	bool wasFellTooHigh() {
		Coord currPos = getPos();
		Coord oneBelow = { currPos.x, currPos.y + 1 };
		int test = getFallCounter();
		return (test >= getMaxFallHeight() && ((pBoard->isFloor(oneBelow)) || (pBoard->isEdge(oneBelow))));
	}

	// Draw the Entity on the board at his current position
	void virtual draw() const {
		gotoxy(getPos());
		pScreen->coloredCharPrint(getSymbol(), getColor());
	}

	// Erase the Entity from the board by redrawing the underlying board character
	virtual void erase() const { // not const function so Mario could override it and use setUseHammer(false)
		Coord currPos = getPos();
		gotoxy(currPos);
		pScreen->coloredCharPrint(pBoard->getChar(currPos), pScreen->getColor("board"));
	}

	// Draw the Entity explotion on the board
	void drawExplotion() const ;
};