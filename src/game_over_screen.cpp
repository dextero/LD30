#include "game_over_screen.h"

#include "game.h"

GameOverScreen::GameOverScreen(Game& game):
    Screen(game)
{
}

void GameOverScreen::handleInput()
{
    sf::Event evt;
    
    while (wnd->pollEvent(evt)) {
        switch (evt.type) {
        case sf::Event::KeyReleased: game.setState(Game::State::Running); break;
        default: break;
        }
    }
}

void GameOverScreen::update(float dt)
{
    (void)dt;
}

void GameOverScreen::draw() const
{
}

