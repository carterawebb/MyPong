#include "Ball.h"

Ball::Ball(Game* g) {
  game = g;

  dir = 0;
  width = height = 20;
  speed = 2.5f;

  pos.xPos = SCREEN_WIDTH / 2 - width / 2;
  pos.yPos = SCREEN_HEIGHT / 2 - width / 2;

  std::srand(std::time(nullptr));
  int randX = (std::rand() % 2) == 1 ? 1 : -1;
  int randY = (std::rand() % 2) == 1 ? 1 : -1;

  vel.xPos = (float)randX;
  vel.yPos = (float)randY;

  rect.x = (int)pos.xPos;
  rect.y = (int)pos.yPos;
  rect.w = width;
  rect.h = width;

  hitSound1 = Mix_LoadWAV("Assets/hitObject1.wav");
  hitSound2 = Mix_LoadWAV("Assets/hitObject2.wav");
  hitSound3 = Mix_LoadWAV("Assets/hitObject3.wav");
  hitSound4 = Mix_LoadWAV("Assets/hitObject4.wav");
  losePoint = Mix_LoadWAV("Assets/losePoint.wav");
  scorePoint = Mix_LoadWAV("Assets/scorePoint.wav");

  start = std::clock();
}
/* -----===== PRIVATE FUNCTIONS =====----- */
Mix_Chunk& Ball::randHitSound() {
  int randNum = std::rand() % 4;

  switch (randNum) {
    case 0:
      return *hitSound1;
    case 1:
      return *hitSound2;
    case 2:
      return *hitSound3;
    case 3:
      return *hitSound4;
    default:
      return *hitSound1;
  }
}

void Ball::updateTrail() {
  int life = MAX_TRAIL_LENGTH - (int)(speed * 2.5);
  trail.push_back({rect, trailColor, life});

  if (trail.size() > MAX_TRAIL_LENGTH) {
    trail.erase(trail.begin());
  }

  for (auto& point : trail) {
    point.trailRect.x -= (int)(vel.xPos * speed);
    point.trailRect.y -= (int)(vel.yPos * speed);
    point.life--;
  }

  while (!trail.empty() && trail.front().life <= 0) {
    trail.erase(trail.begin());
  }
}

/* -----===== PUBLIC FUNCTIONS =====----- */
void Ball::update(Paddle* lPaddle, Paddle* rPaddle) {
  current = std::clock();
  elapsed = (current - start) / (float)CLOCKS_PER_SEC;
  speed = std::min(12.5f, elapsed * 0.1f + 1.5f);

  pos.xPos = pos.xPos + vel.xPos * speed;
  pos.yPos = pos.yPos + vel.yPos * speed;

  if (pos.xPos < 0) {
    Mix_PlayChannel(-1, losePoint, 0);
    rPaddle->increaseScore();
    reset();
  } else if (pos.xPos > SCREEN_WIDTH + width) {
    lPaddle->increaseScore();
    Mix_PlayChannel(-1, scorePoint, 0);
    reset();
  }

  handleCollision(lPaddle);
  handleCollision(rPaddle);

  // Keep ball inbounds
  if (pos.yPos < 0) {
    Mix_PlayChannel(-1, &randHitSound(), 0);
    pos.yPos = 0;
    vel.yPos *= -1;
  } else if (pos.yPos + width > SCREEN_HEIGHT) {
    Mix_PlayChannel(-1, &randHitSound(), 0);
    pos.yPos = SCREEN_HEIGHT - width;
    vel.yPos *= -1;
  }

  rect.x = (int)pos.xPos;
  rect.y = (int)pos.yPos;

  updateTrail();
}

void Ball::handleCollision(Paddle* paddle) {
  if (bottomCollision(paddle) || topCollision(paddle)) {
    Mix_PlayChannel(-1, &randHitSound(), 0);
    return;
  }

  if (pos.xPos + width >=
          paddle->getPos().xPos &&  // Right ball intersect left paddle
      pos.xPos <=
          paddle->getPos().xPos +
              paddle->getRect()->w &&  // Left ball intersect right paddle
      pos.yPos + width >=
          paddle->getPos().yPos &&  // Bottom ball intersect top paddle
      pos.yPos <= paddle->getPos().yPos +
                      paddle->getRect()->h  // Top ball intersect bottom paddle
  ) {
    vel.xPos *= -1;

    // Bounce ball differently depending on where it hits the paddle
    int midPaddle = paddle->getPos().yPos + paddle->getRect()->h / 2;
    int midBall = pos.yPos + width / 2;
    int offsetY = midPaddle - midBall;

    vel.yPos -= offsetY * 0.05f;

    if (paddle->getID() == 0) {
      pos.xPos = paddle->getPos().xPos + paddle->getRect()->w;
    } else if (paddle->getID() == 1) {
      pos.xPos = paddle->getPos().xPos - width;
    }

    Mix_PlayChannel(-1, &randHitSound(), 0);
  }
}

bool Ball::topCollision(Paddle* paddle) {
  bool passBound = false;
  bool collided = false;

  if (paddle->getID() == 0) {
    passBound = pos.xPos < width;
  } else if (paddle->getID() == 1) {
    passBound = pos.xPos > SCREEN_WIDTH - width;
  }

  if (passBound && pos.yPos + width >= paddle->getRect()->y &&
      pos.yPos < paddle->getRect()->y && vel.yPos < 0) {
    pos.yPos = paddle->getRect()->y - width - 1;
    vel.yPos *= -1;
    rect.y = pos.yPos;

    collided = true;
  }

  return collided;
}

bool Ball::bottomCollision(Paddle* paddle) {
  bool passBound = false;
  bool collided = false;

  if (paddle->getID() == 0) {
    passBound = pos.xPos < width;
  } else if (paddle->getID() == 1) {
    passBound = pos.xPos > SCREEN_WIDTH - width;
  }

  if (passBound && pos.yPos <= paddle->getRect()->y + paddle->getRect()->h &&
      pos.yPos + width > paddle->getRect()->y + paddle->getRect()->h &&
      vel.yPos < 0) {
    vel.yPos *= -1;
    rect.y = pos.yPos;

    collided = true;
  }

  return collided;
}

float Ball::getSpeedX() { return vel.xPos * speed; }

float Ball::getSpeedY() { return vel.yPos * speed; }

void Ball::renderTrail(SDL_Renderer* rend) {
  for (auto& point : trail) {
    SDL_SetRenderDrawColor(rend, point.trailColor.r, point.trailColor.g,
                           point.trailColor.b, point.trailColor.a);
    SDL_RenderFillRect(rend, &point.trailRect);
  }
}

void Ball::reset() {
  dir = 0;
  width = height = 20;
  speed = 2.5f;

  pos.xPos = SCREEN_WIDTH / 2 - width / 2;
  pos.yPos = SCREEN_HEIGHT / 2 - width / 2;

  std::srand(std::time(nullptr));
  int randX = (std::rand() % 2) == 1 ? 1 : -1;
  int randY = (std::rand() % 2) == 1 ? 1 : -1;

  vel.xPos = (float)randX;
  vel.yPos = (float)randY;

  rect.x = (int)pos.xPos;
  rect.y = (int)pos.yPos;

  hitSound1 = Mix_LoadWAV("Assets/hitObject1.wav");
  hitSound2 = Mix_LoadWAV("Assets/hitObject2.wav");
  hitSound3 = Mix_LoadWAV("Assets/hitObject3.wav");
  hitSound4 = Mix_LoadWAV("Assets/hitObject4.wav");

  if (game->getGameState() == gameState::PLAYING) {
    start = std::clock();
  }

  game->setGameStatePending();
}

void Ball::clean() {
  Mix_FreeChunk(hitSound1);
  Mix_FreeChunk(hitSound2);
  Mix_FreeChunk(hitSound3);
  Mix_FreeChunk(hitSound4);
}