#include <algorithm>

#include "matrix.h"
#include "font.h"
#include "randomizer.h"
#include "letter.h"

Matrix::Matrix() :
  window(nullptr),
  renderer(nullptr),
  font(nullptr),
  randomizer(nullptr),
  is_running(true),
  ticks(0),
  updating_letters(false) { }

Matrix::~Matrix() { }

bool Matrix::init() {
  /* Initialize necessary systems for the Matrix to run. */
  // Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return false;
  }

  // initialize TTF
  if (TTF_Init() != 0) {
    SDL_Log("Failed to initialize TTF: %s", TTF_GetError());
    return false;
  }

  // get necessary information about the display
  SDL_GetDesktopDisplayMode(0, &display);
  SDL_Log("Screen dimensions: %d x %d", display.w, display.h);
  display.w = 1920;
  display.h = 1080;
  
  // create a fullscreen window
  window = SDL_CreateWindow("",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            display.w, display.h, 0);
  SDL_SetWindowFullscreen(window, 0);
  if (window == nullptr) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  // create a rendrer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  // load a font
  font = new Font(this);
  font->load("../cour.ttf");

  // create a random letter generator
  randomizer = new Randomizer(this);
  
  // return true if all systems intiailized successfully
  return true;
}

void Matrix::shutdown() {
  /* Shutdown: unload all data and shuts down all systems. */
  SDL_Quit();
}

void Matrix::add_letter(Letter* l) {
  if (!updating_letters) {
    letters.emplace_back(l);
  } else {
    pending_letters.emplace_back(l);
  }
}

void Matrix::remove_letter(Letter* l) {
  auto i = std::find(letters.begin(), letters.end(), l);
  if (i != letters.end()) {
    std::iter_swap(i, letters.end() - 1);
    letters.pop_back();
  }
  i = std::find(pending_letters.begin(), pending_letters.end(), l);
  if (i != pending_letters.end()) {
    std::iter_swap(i, pending_letters.end() - 1);
    pending_letters.pop_back();
  }
}

void Matrix::run_loop() {
  /* Run the main game loop. */
  while (is_running) {
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
  /* Update the Matrix world. */
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks + 66))
    ; // do nothing

  // get the time passed
  float delta_time = std::min((SDL_GetTicks() - ticks) / 1000.0, 0.05);
  ticks = SDL_GetTicks();

  // try to create new letters randomly
  if (randomizer->choice()) {
    Letter* l = new Letter(this); 
    l->set_random_char(); 
    l->set_position(randomizer->screen_position(), 0);
  }

  updating_letters = true;
  // update all objects
  for (auto letter : letters) {
    letter->update(delta_time);
  }
  updating_letters = false;

  // delete any inactive letters
  std::vector<Letter*> inactive_letters;
  for (auto letter : letters) {
    if (letter->get_state() == Letter::State::Inactive) {
      inactive_letters.emplace_back(letter);
    }
  }

  for (auto letter : inactive_letters) {
    delete letter;
  }
  

  // move pending letters to not pending
  for (auto letter : pending_letters) {
    letters.emplace_back(letter);
  }
  pending_letters.clear();
}

void Matrix::generate_output() {
  /* Render all objects to the screen. */
  // prepare rendering
  SDL_RenderSetLogicalSize(renderer, 800, 600);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // draw the text
  for (auto letter : letters) {
    letter->render();
  }

  
  // present the screen
  SDL_RenderPresent(renderer);
}
