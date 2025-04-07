#pragma once
#include "Entity.h"

// Draw the explotion of the entity on the screen
void Entity::drawExplotion() const {
	Coord firstExplotionLine = { pos.x - 1, pos.y };
	Coord secondExplotionLine = { pos.x - 1, pos.y - 1 };

	char oldChars[explotionHeighet][explotionWidth + 1] = { "   ", "   " };
	erase();

	// save the old chars 
	gotoxy(firstExplotionLine);
	for (int j = 0; j < explotionWidth; j++) { oldChars[0][j] = pBoard->getChar({ pos.x - 1 + j, pos.y }); }
	oldChars[0][explotionWidth] = '\0';

	gotoxy(secondExplotionLine);
	for (int j = 0; j < explotionWidth; j++) { oldChars[1][j] = pBoard->getChar({ pos.x - 1 + j, pos.y - 1 }); }
	oldChars[1][explotionWidth] = '\0';

	gotoxy(firstExplotionLine);
	pScreen->coloredStrPrint(explotionDraw[0], pScreen->getColor("explotion"));

	gotoxy(secondExplotionLine);
	pScreen->coloredStrPrint(explotionDraw[1], pScreen->getColor("explotion"));

	Sleep(50);

	// return the old chars 
	gotoxy(firstExplotionLine);
	pScreen->coloredStrPrint(oldChars[0], pScreen->getColor("board"));

	gotoxy(secondExplotionLine);
	pScreen->coloredStrPrint(oldChars[1], pScreen->getColor("board"));
}