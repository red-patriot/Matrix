#include <SDL2/SDL.h>

#include "matrix.h"

int main() {
  Matrix matrix;

  if (!matrix.init()) {
    SDL_Log("Failed to create a Matrix system.");
    // shutdown the matrix
    matrix.shutdown();
    return -1;
  }

  matrix.run_loop();

  return 0;
}

