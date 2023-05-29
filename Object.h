#pragma once

#include "Utils.h"

class Object {
 protected:  // Protected variables
  int dir;
  int width;
  int height;
  float speed;

 public:
  virtual SDL_Rect *getRect() = 0;
};