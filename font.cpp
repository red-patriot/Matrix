#include <vector>

#include "matrix.h"
#include "font.h"

Font::Font(Matrix* m) :
  matrix(m) { }

Font::~Font() {

}


bool Font::load(const std::string& filename) {
  /* Load  the font in various different sizes. */
  std::vector<int> fontsizes{8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
                             30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60,
                             64, 68, 72};

  // attempt to load the font in each size
  for (auto& fontsize : fontsizes) {
    TTF_Font* font = TTF_OpenFont(filename.c_str(), fontsize);
    // test that loading was successful 
    if (font == nullptr) {
      SDL_Log("Failed to load font: %s", filename.c_str());
      return false;
    }
    // save the loaded font
    fonts.emplace(fontsize, font);
  }

  return true;
}

void Font::unload() {
  /* Unload all fonts. */
  for (auto& font : fonts) {
    TTF_CloseFont(font.second);
  }
}

SDL_Texture* Font::render_text(const char* text, const SDL_Color color,
                               const int size) const {
  /* Generate a Texture from text. */
  auto font = fonts.find(size);
  
  if (font != fonts.end()) {
    SDL_Surface* s = TTF_RenderText_Blended(font->second, text, color);
    SDL_Texture* t = SDL_CreateTextureFromSurface(matrix->get_renderer(), s);
    SDL_FreeSurface(s);
    return t;
  } else {
    SDL_Log("Unsupported font size: %d", size);
    return nullptr;
  }
}
