#include "randomizer.h"
#include "matrix.h"

/// character_options is the set of all possible characters that a Letter may store.
const char Randomizer::char_options[] = "aáâãàbcçdeéèêẽfghiíìîĩjklmnoóòôõpqrstuúùûũvwxyzABCÇDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+~`{}|[]\\:\";',./<>?";

Randomizer::Randomizer(Matrix* m):
  matrix(m),
  choice_threshold(3),
  rdev(),
  rng(rdev()),
  choice_dist(0, 10),
  char_dist(0, sizeof(char_options)/sizeof(*char_options)-1),
  screen_width(0, m->get_display_width()) { }

Randomizer::~Randomizer() { }

size_t Randomizer::screen_position() {
  /* Get a random number. */
  return screen_width(rng);
}

char Randomizer::rand_char() {
  /* Return a random character*/
  return char_options[char_dist(rng)];
}

bool Randomizer::choice() {
  /* Return a number used to indicate a choice. */
  return choice_dist(rng) <= choice_threshold;
}
