#pragma once

#include <SDL_ttf.h>

#include <string>

#include "Object.h"
#include "Utils.h"

class Paddle : Object {
 private:
  int id;

  Vec2 pos;
  SDL_Rect rect;

  int score = 0;
  SDL_Rect scoreRect;
  SDL_Surface *scoreSurface = nullptr;
  SDL_Texture *scoreTexture = nullptr;

 public:
  Paddle(int i);
  void update();

  void setDir(int d) { dir = d; }
  int getID() { return id; }
  Vec2 getPos() { return pos; }
  void setPosX(int x) { pos.xPos = x; }
  void setPosY(int y) { pos.yPos = y; }
  SDL_Rect *getRect() override { return &rect; }
  int getWidth() { return rect.w; }
  int getHeight() { return rect.h; }
  void setSpeed(float s) { speed = s; }

  void increaseScore();
  void showScore(SDL_Renderer *rend, TTF_Font *font, SDL_Color color);
  int getScore() { return score; }
  void resetScore();
};
