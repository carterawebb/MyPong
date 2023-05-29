#include "Menu.h"

Menu::Menu() {
  isSinglePlayer = false;
  setGameStateToMenu();

  if (TTF_Init() != 0) {
    return;
  }

  font = TTF_OpenFont("Assets/Space_Grotesk/SpaceGrotesk-VariableFont_wght.ttf",
                      100);
}

Menu::~Menu() {
  TTF_CloseFont(font);
  TTF_Quit();
}

void Menu::showMenu(SDL_Renderer *rend, TTF_Font *font, SDL_Color color) {
  startGameSurface = TTF_RenderText_Solid(font, "Start", color);
  quitGameSurface = TTF_RenderText_Solid(font, "Quit", color);
  singlePlayerSurface = TTF_RenderText_Solid(font, "Single Player", color);

  // play button
  startGameRect.w = startGameSurface->w;
  startGameRect.h = startGameSurface->h;
  startGameRect.x = (SCREEN_WIDTH / 2) - (startGameSurface->w / 2);
  startGameRect.y = (SCREEN_HEIGHT / 2) + 100 - (startGameSurface->h / 2);

  // quit button
  quitGameRect.w = quitGameSurface->w;
  quitGameRect.h = quitGameSurface->h;
  quitGameRect.x = (SCREEN_WIDTH / 2) - (quitGameSurface->w / 2);
  quitGameRect.y = (SCREEN_HEIGHT / 2) + 150 - (quitGameSurface->h / 2);

  // single player button
  singlePlayerRect.w = singlePlayerSurface->w;
  singlePlayerRect.h = singlePlayerSurface->h;
  singlePlayerRect.x = (SCREEN_WIDTH / 2) - (singlePlayerSurface->w / 2);
  singlePlayerRect.y = (SCREEN_HEIGHT / 2) + 200 - (singlePlayerSurface->h / 2);

  startGameTexture = SDL_CreateTextureFromSurface(rend, startGameSurface);
  quitGameTexture = SDL_CreateTextureFromSurface(rend, quitGameSurface);
  singlePlayerTexture = SDL_CreateTextureFromSurface(rend, singlePlayerSurface);

  SDL_RenderCopy(rend, startGameTexture, NULL, &startGameRect);
  SDL_RenderCopy(rend, quitGameTexture, NULL, &quitGameRect);
  SDL_RenderCopy(rend, singlePlayerTexture, NULL, &singlePlayerRect);
}

void Menu::update() {
  /*SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      int mouseX, mouseY;
      SDL_GetMouseState(&mouseX, &mouseY);
      if (event.button.button == SDL_BUTTON_LEFT) {
        if (mouseX >= startGameRect.x - startGameRect.w / 2 &&
            mouseX <= startGameRect.x + startGameRect.w / 2 &&
            mouseY >= startGameRect.y - startGameRect.h / 2 &&
            mouseY <= startGameRect.y + startGameRect.h / 2) {
          setGameStateToPending();
        } else if (mouseX >= quitGameRect.x - quitGameRect.w / 2 &&
                   mouseX <= quitGameRect.x + quitGameRect.w / 2 &&
                   mouseY >= quitGameRect.y - quitGameRect.h / 2 &&
                   mouseY <= quitGameRect.y + quitGameRect.h / 2) {
          SDL_QUIT;
        } else if (mouseX >= singlePlayerRect.x - singlePlayerRect.w / 2 &&
                   mouseX <= singlePlayerRect.x + singlePlayerRect.w / 2 &&
                   mouseY >= singlePlayerRect.y - singlePlayerRect.h / 2 &&
                   mouseY <= singlePlayerRect.y + singlePlayerRect.h / 2) {
          setIsSinglePlayer(true);
        }
      }
    }
  }*/

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            break;
          case SDLK_SPACE:
            setGameStateToPending();
            break;
          case SDLK_i:
            setIsSinglePlayer(true);
            break;
          default:
            break;
        }
      case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
          case SDL_BUTTON_LEFT:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= startGameRect.x - startGameRect.w / 2 &&
                mouseX <= startGameRect.x + startGameRect.w / 2 &&
                mouseY >= startGameRect.y - startGameRect.h / 2 &&
                mouseY <= startGameRect.y + startGameRect.h / 2) {
              setGameStateToPending();
            } else if (mouseX >= quitGameRect.x - quitGameRect.w / 2 &&
                       mouseX <= quitGameRect.x + quitGameRect.w / 2 &&
                       mouseY >= quitGameRect.y - quitGameRect.h / 2 &&
                       mouseY <= quitGameRect.y + quitGameRect.h / 2) {
              SDL_QUIT;
            } else if (mouseX >= singlePlayerRect.x - singlePlayerRect.w / 2 &&
                       mouseX <= singlePlayerRect.x + singlePlayerRect.w / 2 &&
                       mouseY >= singlePlayerRect.y - singlePlayerRect.h / 2 &&
                       mouseY <= singlePlayerRect.y + singlePlayerRect.h / 2) {
              setIsSinglePlayer(true);
            }
            break;
          default:
            break;
        }
      default:
        break;
    }
  }
}

void Menu::reset() {
  startGameSurface = nullptr;
  quitGameSurface = nullptr;
  singlePlayerSurface = nullptr;

  startGameRect.x = SCREEN_WIDTH / 2;
  startGameRect.y = SCREEN_HEIGHT / 2 + 100;
  quitGameRect.x = SCREEN_WIDTH;
  quitGameRect.y = SCREEN_HEIGHT;
  singlePlayerRect.x = SCREEN_WIDTH;
  singlePlayerRect.y = SCREEN_HEIGHT - 100;

  setGameStateToMenu();

  setIsSinglePlayer(false);
}
