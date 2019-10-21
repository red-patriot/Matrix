#ifndef matrix_h_INCLUDED
#define matrix_h_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>

struct Letter {
  /* A Representation of a single letter in the Matrix. */
  char letter;
  SDL_Color color{255, 255, 255, 255};
  int x{0};
  int y{0};
  bool create_next{true};

  void update(float delta_time);
};

class Matrix {
public:
  Matrix();
  ~Matrix();

  bool init();

  void run_loop();

  size_t get_window_width() const { return window_size.w; }

private:
  SDL_Window* window;
  SDL_Rect window_size;
  SDL_Renderer* renderer;

  TTF_Font* font;
  int fontsize;

  class Randomizer* randomizer;

  std::list<Letter> letters;
  SDL_Texture* letter_img;
  SDL_Rect letter_rect;
  char character[2];

  bool running;
  Uint32 ticks;

  void load_data();
  void shutdown();

  // run_loop() helpers
  void process_input();
  void update_world();
  void generate_output();

  // letter functions
  void create_letter(int x, int y);
  void cascade_letter(Letter& let);
  void render_letter(const Letter& let);
};

#endif
