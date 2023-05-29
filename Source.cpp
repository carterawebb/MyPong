#include "Game.h"

int main(int argc, char* argv[]) {
  Game game;

  if (game.init ()) {
	game.gameLoop();
  }

  game.clean();

  return 0;
}
