#pragma once

#include <SDL_mixer.h>

#include <algorithm>
#include <ctime>
#include <vector>

#include "Game.h"
#include "Paddle.h"

class Ball : Object {
 private:  // Private Variables
  int width;
  Vec2 pos;
  Vec2 vel;
  SDL_Rect rect;

  Mix_Chunk *hitSound1;
  Mix_Chunk *hitSound2;
  Mix_Chunk *hitSound3;
  Mix_Chunk *hitSound4;
  Mix_Chunk *losePoint;
  Mix_Chunk *scorePoint;

  std::clock_t start;
  std::clock_t current;
  float elapsed;

  class Game *game;

  struct TrailPoint {
    SDL_Rect trailRect;
    SDL_Color trailColor;
    int life;
  };
  SDL_Color trailColor = {255, 255, 255, 255};
  std::vector<TrailPoint> trail;

 private:  // Private Functions
  Mix_Chunk &randHitSound();

  void updateTrail();

 public:
  Ball(Game *g);
  void update(Paddle *lPaddle, Paddle *rPaddle);
  void setVel(float xPos, float yPos) {}

  void handleCollision(Paddle *paddle);
  bool topCollision(Paddle *paddle);
  bool bottomCollision(Paddle *paddle);

  SDL_Rect *getRect() override { return &rect; }

  int getPositionX() { return pos.xPos; }
  int getPositionY() { return pos.yPos; }
  int getDirX() { return vel.xPos; }
  int getDirY() { return vel.yPos; }
  float getSpeed() { return speed; }
  float getSpeedX();
  float getSpeedY();

  int getWidth() { return width; }

  void renderTrail(SDL_Renderer *rend);

  void reset();
  void clean();
};
