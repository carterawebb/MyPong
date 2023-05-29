#pragma once

#include <SDL.h>

enum gameState {
  PENDING,
  PLAYING,
};

struct Vec2 {
  float xPos;
  float yPos;
};

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

const int COOLDOWN_DELAY = 1000; // 1 second

const int GO_UP = -1;
const int GO_DOWN = 1;

static const int MAX_TRAIL_LENGTH = 30;