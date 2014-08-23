#include "menu_screen.h"

#include "game.h"

MenuScreen::MenuScreen(Game& game):
    Screen(game)
{
}

void MenuScreen::handleInput()
{
}

void MenuScreen::update(float dt)
{
    (void)dt;

    game.setState(Game::State::Running);
}

void MenuScreen::draw() const
{
}

