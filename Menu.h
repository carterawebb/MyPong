#pragma once

#include <SDL_ttf.h>

#include "Utils.h"

class Menu {
 private:
  bool isSinglePlayer;

  gameState state = gameState::MENU;

  TTF_Font *font = nullptr;
  SDL_Color textColor = {255, 255, 255};

  SDL_Rect startGameRect;
  SDL_Surface *startGameSurface = nullptr;
  SDL_Texture *startGameTexture = nullptr;

  SDL_Rect quitGameRect;
  SDL_Surface *quitGameSurface = nullptr;
  SDL_Texture *quitGameTexture = nullptr;

  SDL_Rect singlePlayerRect;
  SDL_Surface *singlePlayerSurface = nullptr;
  SDL_Texture *singlePlayerTexture = nullptr;

 public:
  Menu();
  ~Menu();

  void showMenu(SDL_Renderer *rend, TTF_Font *font, SDL_Color color);

  void update();

  SDL_Rect getStartGameRect() const { return startGameRect; }
  SDL_Rect getQuitGameRect() const { return quitGameRect; }
  SDL_Rect getSinglePlayerRect() const { return singlePlayerRect; }

  bool getIsSinglePlayer() const { return isSinglePlayer; }
  void setIsSinglePlayer(bool isSinglePlayer) { this->isSinglePlayer = isSinglePlayer; }

  void setGameStateToMenu() { state = gameState::MENU; }
  void setGameStateToPending() { state = gameState::PENDING; }
  void setGameStateToPlaying() { state = gameState::PLAYING; }
  gameState getGameState() const { return state; }

void reset();
}
;
