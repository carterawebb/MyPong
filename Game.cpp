#include "Game.h"

bool Game::init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    return false;
  }

  window =
      SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!window) {
    return false;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    return false;
  }

  if (TTF_Init() != 0) {
    return false;
  }

  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  // Initialization of paddles
  leftPaddle = new Paddle(0);
  rightPaddle = new Paddle(1);

  // Initialize ball
  ball = new Ball(this);

  font = TTF_OpenFont("Assets/Space_Grotesk/SpaceGrotesk-VariableFont_wght.ttf",
                      40);

  cooldownTime = 0;

  // Initialize the AI
  int diff = 2;  // 0 = easy, 1 = medium, 2 = hard, 3 = impossible

  if (diff == 3) {
    rightPaddle->setSpeed(50);
  }

  ai = new AI(diff, rightPaddle, ball);

  return true;
}

/* -----===== PRIVATE FUNCTIONS =====----- */
void Game::handleEvents() {
  bool pauseKeyHasBeenPressed = false;

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      isRunning = false;
    }
  }

  const Uint8* keystates = SDL_GetKeyboardState(NULL);

  if (cooldownTime > SDL_GetTicks()) return;

  if (keystates[SDL_SCANCODE_ESCAPE]) {
    isRunning = false;
  }

  if (state == gameState::PENDING) {
    if (keystates[SDL_SCANCODE_SPACE]) {
      if (!pauseKeyHasBeenPressed) {
        state = gameState::PLAYING;
        pauseKeyHasBeenPressed = true;
        cooldownTime = SDL_GetTicks() + COOLDOWN_DELAY;
      }
    }

    leftPaddle->setDir(0);
    rightPaddle->setDir(0);
    return;
  } else if (state == gameState::PLAYING) {
    if (keystates[SDL_SCANCODE_SPACE]) {
      if (!pauseKeyHasBeenPressed) {
        state = gameState::PENDING;
        pauseKeyHasBeenPressed = true;
        cooldownTime = SDL_GetTicks() + COOLDOWN_DELAY;
      }
    }
  }

  // Left Paddle movement
  leftPaddle->setDir(GO_UP * keystates[SDL_SCANCODE_W] +
                     GO_DOWN * keystates[SDL_SCANCODE_S]);

  // Right Paddle movement
  //rightPaddle->setDir(GO_UP * keystates[SDL_SCANCODE_UP] +
  //                    GO_DOWN * keystates[SDL_SCANCODE_DOWN]);
  rightPaddle->setDir(ai->calculateMove());
}

void Game::update() {
  if (state == gameState::PENDING) {
    return;
  }

  leftPaddle->update();
  rightPaddle->update();

  if (leftPaddle->getScore() == 7 || rightPaddle->getScore() == 7) {
    ball->reset();
    leftPaddle->resetScore();
    rightPaddle->resetScore();
  }

  ball->update(leftPaddle, rightPaddle);
}

void Game::draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // draw paddles
  SDL_SetRenderDrawColor(renderer, 45, 255, 71, 179);
  SDL_RenderFillRect(renderer, leftPaddle->getRect());
  SDL_SetRenderDrawColor(renderer, 255, 87, 60, 255);
  SDL_RenderFillRect(renderer, rightPaddle->getRect());

  // draw ball
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, ball->getRect());

  // draw trail
  //ball->renderTrail(renderer);

  // draw score
  leftPaddle->showScore(renderer, font, textColor);
  rightPaddle->showScore(renderer, font, textColor);

  SDL_RenderPresent(renderer);
}

/* -----===== PUBLIC FUNCTIONS =====----- */
Game::Game() {
  isRunning = true;
  state = gameState::PENDING;
}

void Game::gameLoop() {
  while (isRunning) {
    handleEvents();
    update();
    draw();
  }
}

void Game::clean() {
  ball->clean();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  Mix_CloseAudio();
  SDL_Quit();
}