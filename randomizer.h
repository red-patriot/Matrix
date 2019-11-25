#ifndef randomizer_h_INCLUDED
#define randomizer_h_INCLUDED

#include <random>
#include <iostream>

#include "matrix.h"

class Randomizer {
public:
  Randomizer(Matrix* m) :
    matrix(m),
    rdev(),
    rng(rdev()),
    choice_dist(0.0, 1.0),
    char_dist(0, sizeof(char_options)/sizeof(*char_options) - 1),
    screen_width(0, matrix->get_screen_width()) { }
  
  ~Randomizer() { }

  [[nodiscard]] size_t screen_position() { return screen_width(rng); }
  [[nodiscard]] char rand_char() { return char_options[char_dist(rng)]; }
  bool choice(double chance=.50) { return choice_dist(rng) <= chance; }

private:
  Matrix* matrix;
  
  static constexpr char char_options[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+-=~`{}|[]\\:\";',./<>?";
  
  std::random_device rdev;
  std::mt19937 rng;

  std::uniform_real_distribution<double> choice_dist;
  std::uniform_int_distribution<std::mt19937::result_type> char_dist;
  std::uniform_int_distribution<std::mt19937::result_type> screen_width;
};

#endif
