#ifndef font_h_INCLUDED
#define font_h_INCLUDED

#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

class Font {
public:
  Font(class Matrix* m);
  ~Font();

  bool load(const std::string& filename);
  void unload();

  SDL_Texture* render_text(const char* text, const SDL_Color color,
                           const int size) const;
  

private:
  std::unordered_map<int, TTF_Font*> fonts;

  class Matrix* matrix;
  TTF_Font* font;
};

#endif
