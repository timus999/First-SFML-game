#include "game.h"
#include <cstdlib>

int main(){
  srand((unsigned)time(NULL));
  Game game("hello");
  game.run();
  return 0;
}
