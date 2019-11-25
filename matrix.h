#ifndef matrix_h_INCLUDED
#define matrix_h_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>

struct Letter {
  char character;
  int x{0};
  int y{0};
  SDL_Color color{255, 255, 255, 255};
  bool create_new{true};

  void update(float delta_time);
};

class Matrix {
public:
  Matrix();

  bool init();
  void shutdown();

  void run_loop();
  
  [[nodiscard, gnu::const]] size_t get_screen_width() const { return window_size.w; }

private:
  SDL_Window* window;
  SDL_Rect window_size;
  SDL_Renderer* renderer;

  TTF_Font* font;
  int fontsize;

  bool updating_letters;
  std::list<Letter> letters;
  std::list<Letter> pending_letters;
  char letter_character[2];
  
  bool is_running;
  Uint32 ticks;

  class Randomizer* randomizer;

  // run_loop() helper functions
  void handle_input();
  void update_world();
  [[gnu::hot]] void generate_output();

  // Letter management functions
  [[gnu::hot]] void create_new_letter(int new_x, int new_y);
  [[gnu::hot]] void render_letter(const Letter& letter);
};

#endif
