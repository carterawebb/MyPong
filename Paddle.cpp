#include "Paddle.h"

/* ----------========== PADDLE FUNCTIONS ==========---------- */
Paddle::Paddle(int i) {
  id = i;
  dir = 0;
  speed = 10.f;
  width = 20;
  height = 100;

  if (id == 0) {
    pos.xPos = 0;
  } else if (id == 1) {
    pos.xPos = SCREEN_WIDTH - width;
  }

  pos.yPos = SCREEN_HEIGHT / 2 - height / 2;

  rect.x = (int)pos.xPos;
  rect.y = (int)pos.yPos;
  rect.w = width;
  rect.h = height;
}

void Paddle::update() {
  // Set initial yPos
  pos.yPos = pos.yPos + speed * dir;

  // Keey yPos inbounds
  if (pos.yPos < 0) {
    pos.yPos = 0;
  } else if (pos.yPos + height > SCREEN_HEIGHT) {
    pos.yPos = SCREEN_HEIGHT - height;
  }

  rect.y = pos.yPos;
}

void Paddle::increaseScore() { score++; }

void Paddle::showScore(SDL_Renderer *rend, TTF_Font *font, SDL_Color color) {
  scoreSurface =
      TTF_RenderText_Solid(font, std::to_string(score).c_str(), color);

  if (id == 0) {
    scoreRect.x = SCREEN_WIDTH / 2 - 100;
  } else if (id == 1) {
    scoreRect.x = SCREEN_WIDTH / 2 + 100;
  }

  scoreRect.y = 25;
  scoreRect.w = scoreSurface->w;
  scoreRect.h = scoreSurface->h;

  scoreTexture = SDL_CreateTextureFromSurface(rend, scoreSurface);

  SDL_RenderCopy(rend, scoreTexture, NULL, &scoreRect);
}

void Paddle::resetScore() {
  score = 0;
  speed = 10.f;
}