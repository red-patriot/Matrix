#ifndef randomizer_h_INCLUDED
#define randomizer_h_INCLUDED

/* random_letter.h
 * A class which generates random letters. 
 */

#include <random>

class Randomizer {
public:
  Randomizer(class Matrix* m);
  ~Randomizer();

  size_t screen_position();
  char rand_char();
  bool choice();

private:
  class Matrix* matrix;
  static const char char_options[];

  unsigned int choice_threshold;
  
  std::random_device rdev;
  std::mt19937 rng;

  std::uniform_int_distribution<std::mt19937::result_type> choice_dist;
  std::uniform_int_distribution<std::mt19937::result_type> char_dist;
  std::uniform_int_distribution<std::mt19937::result_type> screen_width;
};

#endif
