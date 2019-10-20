#include <algorithm>

#include "letter.h"
#include "matrix.h"
#include "font.h"
#include "randomizer.h"

Letter::Letter(Matrix* m) :
  matrix(m),
  state(State::Active),
  color{255, 255, 255, 255},
  texture(nullptr),
  pt_size(30),
  bottom(true) {
    m->add_letter(this);
    
    // initialize the character
    character[0] = 'a';
    character[1] = '\0';
    create_char_image();
}

Letter::~Letter() {
  matrix->remove_letter(this);
}

void Letter::set_character(const char c) {
  /* Change the character of this letter and set the texture accordingly. */
  character[0] = c;
  create_char_image();
}

void Letter::set_random_char() {
  /* Change the character of this letter to a random letter and set the texture. */
  character[0] = matrix->get_randomizer()->rand_char();
  create_char_image();
}

void Letter::update(float delta_time) {
  /* Update this letter as a function of delta_time. */  
  // fade the letter to black
  if (color.r > 0 && color.b > 0) {
    color.r = std::max(color.r - 5*delta_time, 0.0f);
    color.b = std::max(color.b - 5*delta_time, 0.0f);
  } else if (color.g > 0) {
    color.g = std::max(color.g - 5*delta_time, 0.0f);
  }
  if (color.g > 0) {
    create_char_image();
  } else {
    state = State::Inactive;
  }

  // create a new letter below this one if this is the bottom letter
  // and the next one would be on screen 
  if (rect.y > matrix->get_display_height()) {
    not_bottom();
  }
  
  if (is_bottom() && (rect.y + rect.h) < matrix->get_display_height()) {
    Letter* next_letter = new Letter(matrix);
    next_letter->set_random_char();
    next_letter->set_position(rect.x, rect.y + rect.h);

    not_bottom();
  }
}

void Letter::render() {
  /* Render the letter to the screen. */
  SDL_RenderCopy(matrix->get_renderer(), texture, nullptr, &rect);
}

bool Letter::create_char_image() {
  /* Create the texture for the character. */
  Font* font = matrix->get_font();
  if (font == nullptr) {
    SDL_Log("No font has been loaded");
    return false;
  }
  texture = font->render_text(character, color, pt_size);
  // if no texture was created, log it and mark this letter for deletion
  if (texture == nullptr) {
    SDL_Log("Texture is NULL: %s", &character[0]);
    state = State::Inactive;
    return false;
  }
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
  rect.w *= .2;
  rect.h *= .2;
  return true;
}



