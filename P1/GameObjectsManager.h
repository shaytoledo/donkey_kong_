#pragma once
#include <vector>
#include "Utils.h"
#include "Board.h"
#include "Screen.h"
#include "optional"
// Section: "Where can I put the definition of a template class?"
// Reference: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl


// Class that manages all the game objects in the game
template <typename T>
class GameObjectsManager {
private:
    Board& board;
    Screen& screen;
    std::vector<std::shared_ptr<T>> objects;
    int maxNumOfObjects;
    std::vector<Coord>& startPositions;
    Coord startPos;

protected:
	
    // Getters
    Board& getBoard() const { return this->board; }
    Screen& getScreen() const {  return this->screen; }
    
	
	// Setters
	void setBoard(Board& newBoard) { this->board = newBoard; }
	void setScreen(Screen& newScreen) { this->screen = newScreen; }
	
public:

    GameObjectsManager(Board& board, Screen& screen, int maxNumOfObjects, std::vector<Coord>& startPositions) : 
        board(board),
        screen(screen),
        maxNumOfObjects(maxNumOfObjects),
		startPositions(startPositions),
        startPos(startPositions.at(0)) {
        objects.reserve(maxNumOfObjects);

    }
    
    // Virtual destructor
    virtual ~GameObjectsManager() = default;

	// Get the vector of objects
    std::vector<std::shared_ptr<T>>& getObjects()  { return this->objects; }
	
	// Set start positions
    void setStartPos(Coord newStartPositions) { this->startPos = newStartPositions; }

    // Get start positions
    Coord getStartPos() const { return startPos; }

    // set start positions
    void setStartPositions(std::vector<Coord>& newStartPositions) { this->startPositions = newStartPositions; }
	
    // Get start positions
    std::vector<Coord>& getStartPositions() { return this->startPositions; }

	// Set the maximum number of objects
    void setMaxNumOfObjects(int size) { increaseReservedSize(size);  maxNumOfObjects = size; }
	
    // Get the maximum number of objects
    int getMaxNumOfObjects() const { return this->maxNumOfObjects; }

    // Add an object
    void addObject() {
        if (isAddingObjectEnabled()) {
            auto newObject = std::make_shared<T>(getBoard(), getScreen(), getStartPos());
            getObjects().push_back(newObject);
        }
    }
	
    // Check if adding an object is enabled
	virtual bool isAddingObjectEnabled() {
		return static_cast<int>(getObjects().size()) < getMaxNumOfObjects();
	}

    // Reset all objects, clearing the vector and restoring their states
    virtual void resetAllObjects() {
        objects.clear();
    }

    // Draw all objects in the vector
    void drawObjects() const {
        for (const auto& obj : objects) {            
            obj->draw();
        }
    }

    // Move all active objects currently in play
    virtual void moveObjects() {
        for (auto& obj : objects) {
            obj->erase();
            obj->updateDirection();
            obj->move();
        }
    }

    // Increase the reserved size of the objects vector
    void increaseReservedSize(int newMaxNumOfObjects) {
        if (newMaxNumOfObjects > maxNumOfObjects) {
            objects.reserve(newMaxNumOfObjects);
            maxNumOfObjects = newMaxNumOfObjects;
        }
    }
};

