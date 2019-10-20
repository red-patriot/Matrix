#ifndef letter_h_INCLUDED
#define letter_h_INCLUDED

#include <SDL2/SDL_ttf.h>

/* Letter: 
 * An object which holds a sincle letter and its necesssary graphical resources.
 */

class Letter {
public:
  enum State {
              Active,
              Inactive,
  };
  
  Letter(class Matrix* m);
  ~Letter();

  void set_character(const char c);
  char get_character() const { return character[0]; }
  void set_random_char();

  void set_position(int x, int y) { rect.x = x; rect.y = y; }
  int get_x() const { return rect.x; }
  int get_y() const { return rect.y; }

  State get_state() const { return state; }

  int get_height() const { return rect.h; }
  int get_width() const {return rect.w; }

  bool is_bottom() const { return bottom; }
  void not_bottom() { bottom = false; }

  void update(float delta_time);

  void render();

private:
  class Matrix* matrix;
  State state;

  char character[2];
  SDL_Color color;
  SDL_Texture* texture;
  SDL_Rect rect;
  
  int pt_size;
  bool bottom;

  bool create_char_image();
};

#endif
