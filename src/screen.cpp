#include "screen.h"

#include "game.h"

Screen::Screen(Game& game):
    game(game),
    wnd(game.wnd),
    viewRect(-(float)wnd->getSize().x / 2, -(float)wnd->getSize().y / 2,
             (float)wnd->getSize().x, (float)wnd->getSize().y)
{}

