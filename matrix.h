#ifndef matrix_h_INCLUDED
#define matrix_h_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

class Matrix {
public:
  Matrix();
  ~Matrix();

  bool init();
  void shutdown();

  void run_loop();

  SDL_Renderer* get_renderer() const { return renderer; }
  class Font* get_font() const { return font; }

  class Randomizer* get_randomizer() const { return randomizer; }

  void add_letter(class Letter* l);
  void remove_letter(class Letter* l);

  int get_display_height() const { return display.h; }
  int get_display_width() const { return display.w; }

private:
  SDL_DisplayMode display;
  SDL_Window* window;
  SDL_Renderer* renderer;
  class Font* font;
  class Randomizer* randomizer;

  bool is_running;
  Uint32 ticks;
  
  bool updating_letters;
  std::vector<class Letter*> letters;
  std::vector<class Letter*> pending_letters;
  
  void process_input();
  void update_world();
  void generate_output();
};

#endif
