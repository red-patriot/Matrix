#ifndef randomizer_h_INCLUDED
#define randomizer_h_INCLUDED

#include <random>

#include "matrix.h"

class Randomizer {
public:
  Randomizer(Matrix* m) :
    matrix(m),
    choice_threshold(3) ,
    rdev(),
    rng(rdev()),
    choice_dist(0, 10),
    char_dist(0, sizeof(char_options)/sizeof(*char_options) -1),
    screen_width(0, matrix->get_window_width()) { }
  
  ~Randomizer() { }

  size_t screen_position() { return screen_width(rng); }
  char rand_char() { return char_options[char_dist(rng)]; }
  bool choice() { return choice_dist(rng) <= choice_threshold; }

private:
  Matrix* matrix;
  
  static constexpr char char_options[] = "aáâãàbcçdeéèêẽfghiíìîĩjklmnoóòôõpqrstuúùûũvwxyzABCÇDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+-=~`{}|[]\\:\";',./<>?";
  unsigned int choice_threshold;
  
  std::random_device rdev;
  std::mt19937 rng;

  std::uniform_int_distribution<std::mt19937::result_type> choice_dist;
  std::uniform_int_distribution<std::mt19937::result_type> char_dist;
  std::uniform_int_distribution<std::mt19937::result_type> screen_width;
};

#endif
