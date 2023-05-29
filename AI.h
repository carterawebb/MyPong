#pragma once

#include "Paddle.h"
#include "Ball.h"

class AI {
 private:          // Private variables
  int difficulty;  // 0 is easy, 1 is medium, 2 is hard, 3 is impossible
  int id = 1;

  class Paddle *controlledPaddle;
  class Ball *watchedBall;

 private:  // Private functions
  int easyAI();
  int mediumAI();
  int hardAI();
  int impossibleAI();

 public:
  AI(int d, Paddle *cPaddle, Ball *wBall);
  void aiUpdate();
  int calculateMove();
};