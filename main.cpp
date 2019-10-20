#include <SDL2/SDL.h>
#include <iostream>

#include "matrix.h"
#include "letter.h"

int main(int, char**) {
  std::cout << sizeof(Letter) << '\n'
            << sizeof(char) << '\n';
  Matrix matrix = Matrix();
  
  // attempt to initialize SDL
  if (!matrix.init()) {
    std::cout << "Initialization failed.\n";
    return -1;
  }

  matrix.run_loop();

  return 0;
}

/*
 * TODO: Make letters go black faster;
 * TODO: figure out some way for the Matrix::letters stay a constant size
 */
