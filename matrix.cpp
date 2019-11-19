#include <algorithm>

#include "matrix.h"
#include "randomizer.h"

void Letter::update(float) {
  color.r = std::max(0, color.r - 85);
  color.b = std::max(0, color.r - 85);
  color.g = std::max(0, color.g - 5);
}

Matrix::Matrix() :
  window(nullptr),
  renderer(nullptr),
  font(nullptr),
  fontsize(16),
  updating_letters(false),
  is_running(true),
  ticks(0),
  randomizer(nullptr) {
  // ensure that the characters are always null-terminated
  letter_character[1] = '\0';
}

bool Matrix::init() {
  /* Initialize necessary systems. */
  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return false;
  }

  // initialize TTF
  if (TTF_Init() != 0) {
    SDL_Log("Failed to initialize TTF: %s", TTF_GetError());
    return false;
  }

  // Create a window
  window = SDL_CreateWindow("Matrix",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            0, 0, SDL_WINDOW_FULLSCREEN);
  if (window == nullptr) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }
  SDL_GetWindowSize(window, &window_size.w, &window_size.h);
  SDL_GetWindowPosition(window, &window_size.x, &window_size.y);
  
  //  Create a renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  // load a font
  const char* fontname = "../cour.ttf";
  font = TTF_OpenFont(fontname, fontsize);
  if (font == nullptr) {
    SDL_Log("Failed to load font: %s, %s", fontname, TTF_GetError());
    return false;
  }

  // create a randomizer
  randomizer = new Randomizer(this);

  // hide the mouse
  SDL_ShowCursor(SDL_DISABLE);

  // only return true if all systems initialized successfully
  return true;
}

void Matrix::shutdown() {
  /* Shutdown all necessary systems. */
  // delete window and renderer
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  // Show the cursor again
  SDL_ShowCursor(SDL_ENABLE);

  // Quit TTF and SDL2
  TTF_Quit();
  SDL_Quit();
}

void Matrix::run_loop() {
  while (is_running) {
    handle_input();
    update_world();
    generate_output();
  }
}

void Matrix::handle_input() {
  /* Process user input. */
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    }   
  }

  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  if (keystate[SDL_SCANCODE_ESCAPE]) {
    is_running = false;
  }
}

void Matrix::update_world() {
  /* Update the world and a function of time. */
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks + 100))
    ; // do nothing
  
  // get the time passed
  float delta_time = std::min((SDL_GetTicks() - ticks) / 1000.0, 0.05);
  ticks = SDL_GetTicks();

  updating_letters = true;
   // cascade letters downwards
  for (auto& letter : letters) {
    letter.update(delta_time);
    if (letter.create_new &&
        letter.y + fontsize < window_size.h &&
        letter.color.g != 0) {
      letter.create_new = false;
      create_new_letter(letter.x, letter.y + fontsize);
    }
    if (randomizer->choice(.05)) {
      letter.character = randomizer->rand_char();
    }
  }

  // Create new cascades
  if (randomizer->choice(.50)) {
    create_new_letter(randomizer->screen_position(), 0);
  }
  
  updating_letters = false;

  // remove old letters from letters
  std::list<Letter>::iterator i = letters.begin();
  while (i != letters.end()) {
    if (i->color.g == 0) {
      i = letters.erase(i);
    } else {
      ++i;
    }
  }

  // add pending_letters to letters
  for (auto& letter : pending_letters) {
    letters.push_back(letter);
  }

  pending_letters.clear();
}

void Matrix::generate_output() {
  /* Generate the display. */
  // clear the old screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // render each letter
  for (auto& letter : letters) {
    render_letter(letter);
  }

  // present the rendered view
  SDL_RenderPresent(renderer);
}

void Matrix::create_new_letter(int new_x, int new_y) {
  /* Create a new letter at the specified position. */
  Letter l;
  l.x = new_x;
  l.y = new_y;
  l.character = randomizer->rand_char();

  if (updating_letters) {
    pending_letters.push_back(l);
  } else {
    letters.push_back(l);
  }
}

void Matrix::render_letter(const Letter& letter) {
  /* Render the letter to the screen. */
  // copy in the letter's char
  letter_character[0] = letter.character;

  // create a surface and texture
  SDL_Surface* s = TTF_RenderText_Blended(font, letter_character, letter.color);
  SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);

  // size the texture appropriately
  SDL_Rect letter_rect;
  SDL_QueryTexture(t, NULL, NULL, &letter_rect.w, &letter_rect.h);
  letter_rect.x = letter.x;
  letter_rect.y = letter.y;

  // render the image created
  SDL_RenderCopy(renderer, t, nullptr, &letter_rect);

  // Free the surface and texture
  SDL_FreeSurface(s);
  SDL_DestroyTexture(t);
}
