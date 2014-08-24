#include <fenv.h>

#include "game.h"

int main(int /*argc*/,
         char** /*argv*/)
{
    //feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW);

    Game game;
    game.run();

    return 0;
}

