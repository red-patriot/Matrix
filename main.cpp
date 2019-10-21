#include <iostream>

#include "matrix.h"

int main() {
  Matrix matrix = Matrix();
  if(!matrix.init()) {
    std::cout << "Failure.\n";
    return -1;
  }

  matrix.run_loop();
  
  return 0;
}

/*
 * TODO: 
 */
