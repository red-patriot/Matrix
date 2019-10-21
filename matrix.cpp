#include <algorithm>

#include "matrix.h"
#include "randomizer.h"

void Letter::update(float delta_time) {
  /* Update this letter as a fucntion of time. */
  if (color.r > 0 && color.b > 0) {
    color.r = std::max(color.r - 500*delta_time, 0.0f);
    color.b = std::max(color.b - 500*delta_time, 0.0f);
  } else {
    color.g = std::max(color.g - 150*delta_time, 0.0f);
  }
}

Matrix::Matrix() :
  window(nullptr),
  renderer(nullptr),
  font(nullptr),
  fontsize(24),
  running(true),
  ticks(0) {
  // ensure that each character is null-terminated
  character[1] = '\0';
}

Matrix::~Matrix() {
  shutdown();
}

bool Matrix::init() {
  /* Initialize all necessary systems. */
  // initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Failed to initialize SDL2: %s", SDL_GetError());
    return false;
  }

  // intialize TTF
  if (TTF_Init() != 0) {
    SDL_Log("Failed to initialize TTF: %s", TTF_GetError());
    return false;
  }

  // create a window
  window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                   0, 0, SDL_WINDOW_FULLSCREEN);
  if (window == nullptr) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }
  SDL_GetWindowSize(window, &window_size.w, &window_size.h);
  SDL_GetWindowPosition(window, &window_size.x, &window_size.y);

  // create a renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  // load a font
  const char* fontname = "../cour.ttf";
  font = TTF_OpenFont(fontname, fontsize);
  if (font == nullptr) {
    SDL_Log("Unable to load font %s : %s", fontname, TTF_GetError());
  }

  // load a randomizer
  randomizer = new Randomizer(this);

  // load all other necessary data
  load_data();

  // return true if all systems initialized correctly
  return true;
}

void Matrix::load_data() {
  /* Load all necessary data. */
  Letter l;
  l.letter = randomizer->rand_char();
  l.x = 400;

  letters.push_front(l);
}

void Matrix::shutdown() {
  /* Shutdown all systems. */
  TTF_CloseFont(font);
}

void Matrix::run_loop() {
  /* Run the main loop. */
  while (running) {
    process_input();
    update_world();
    generate_output();
  }
}

void Matrix::process_input() {
  /* Process player input. */
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = false;
      break;
    }   
  }

  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  if (keystate[SDL_SCANCODE_ESCAPE]) {
    running = false;
  }
}

void Matrix::update_world() {
  /* Update the world as a function of time. */
  // limit the framerate to 10 fps
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks + 100))
    ; // do nothing

  // get the time passed
  float delta_time = std::min((SDL_GetTicks() - ticks) / 1000.0, 0.05);
  ticks = SDL_GetTicks();

  // update all letters and delete old letters
  std::list<Letter>::iterator i = letters.begin();
  while (i != letters.end()) {
    if (i->color.g == 0) {
      i = letters.erase(i);
    } else {
      i->update(delta_time);
      if (i->create_next) {
        cascade_letter(*i);
      }
      ++i;
    }
  }

  // randomly create new letters
  if (randomizer->choice()) {
    create_letter(randomizer->screen_position(), 0);
  }
}

void Matrix::generate_output() {
  /* Generate the display. */
  // clear the old screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // render each letter
  for (auto l : letters) {
    render_letter(l);
  }

  SDL_RenderPresent(renderer);
}

void Matrix::create_letter(int x,  int y) {
  /* Create a new letter. */
  Letter l;
  l.letter = randomizer->rand_char();
  l.x = x;
  l.y = y + fontsize;

  letters.push_front(l);
}

void Matrix::cascade_letter(Letter& let) {
  /* Cascade the letters down. */
  create_letter(let.x, let.y);
  
  let.create_next = false;
}

void Matrix::render_letter(const Letter& let) {
  /* Create an image for the given letter. */
  // make sure the font is loaded
  if (font == nullptr) {
    SDL_Log("No font loaded.");
    letter_img = nullptr;
    return;
  }

  // create an image for the given letter
  character[0] = let.letter;
  SDL_Surface* s = TTF_RenderText_Blended(font, character, let.color);
  letter_img = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  SDL_QueryTexture(letter_img, NULL, NULL, &letter_rect.w, &letter_rect.h);
  letter_rect.x = let.x;
  letter_rect.y = let.y;

  // render the image created
  SDL_RenderCopy(renderer, letter_img, nullptr, &letter_rect);
}
