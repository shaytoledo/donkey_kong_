#pragma once
#include <vector>
#include <unordered_map>
#include "Ghost.h"
#include "SuperGhost.h"
#include "Utils.h"
#include "GameObjectsManager.h"
class Mario; // forward decleration

// Class that manages all the Ghosts in the game
class GhostsManager : public GameObjectsManager<Ghost> {
private:

	bool wereAllObjectsAdded = false;
	std::unordered_map<Coord, Ghost*> nextPositions;
	std::vector<Coord>& superGhostsStartPos;

protected:

	// Get the next position of a ghost (used for collision detection)
	std::unordered_map<Coord, Ghost*>& getNextPositions() { return nextPositions; }
	
	// Get super ghost start position 
	std::vector<Coord>& getSuperGhostsStartPos() const { return superGhostsStartPos; }
	
	// Set the next position of a ghost (used for collision detection)
	void  setNextPositions(const std::unordered_map<Coord, Ghost*>& newPositions) { nextPositions = newPositions; }

public:
	GhostsManager(Board& board, Screen& screen, std::vector<Coord>& startPositions, std::vector<Coord>& _superGhostsStartPos) :
		GameObjectsManager<Ghost>(board, screen, static_cast<int>(startPositions.size()), startPositions), superGhostsStartPos(_superGhostsStartPos) {}

	// Get the flag that indicates if all objects were added
	bool getWereAllObjectsAdded() const { return wereAllObjectsAdded; }

	// Set the flag that indicates if all objects were added
	void setWereAllObjectsAdded(bool flag) { wereAllObjectsAdded = flag; }

	// Add all objects to the vector
	void addAllObjects(Entity& mario);
	
	// Add a new super ghost
	void addSuperGhosts(Entity& mario);

	// Move all active objects currently in play
	void moveObjects() override;

	// Handle the collision of ghosts with other ghosts
	bool handleGhostsCollision(Ghost& ghost);

	// Handle the demage of the ghost corresponds to mario
	bool handleGhostDemaged(Mario& mario);

};