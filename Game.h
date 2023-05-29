#pragma once

#include "Ball.h"
#include "Paddle.h"
#include "AI.h"

class Game {
 private:  // Private variables
  bool isRunning = true;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  class Paddle *leftPaddle, *rightPaddle;
  class AI *ai;
  class Ball* ball;

  TTF_Font* font = nullptr;
  SDL_Color textColor = {255, 255, 255};

  int state;
  bool singlePlayer;

  Uint32 cooldownTime;

 private:  // Private functions
  void handleEvents();
  void update();
  void draw();

 public:
  Game();

  gameState getGameState() { return (gameState)state; }
  void setGameStatePending() { state = gameState::PENDING; }
  void setGameStatePlaying() { state = gameState::PLAYING; }

  bool init();
  void gameLoop();
  void clean();
};
