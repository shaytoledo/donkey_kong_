#include "Game.h"
#include "LoadedGame.h"
#include "SaveGame.h"

int main(int argc, char** argv) {
	bool loadMode = false, saveMode = false, silentMode = false;
	if (argc >= 2 && argc <= 3) {
		loadMode = std::string(argv[1]) == "-load";
		saveMode = std::string(argv[1]) == "-save";
		silentMode = loadMode && argc == 3 && std::string(argv[2]) == "-silent";
		if ((!loadMode && !saveMode) || (loadMode && argc == 3 && !silentMode) || (saveMode && argc > 2)) {
			std::cerr << "Bad input";
			return 1;
		}
	}
	
	if (saveMode) {
		SaveGame game;
		game.start();
	}
	else if (loadMode) {
		LoadedGame game(silentMode);
		game.start();
	}
	else {
		Game game;
		game.start();
	}
	return 0;
}