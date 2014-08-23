#include "game.h"

void Game::setState(State state)
{
    switch (state) {
    case State::Menu:    screen.reset(new MenuScreen(*this)); break;
    case State::Running: screen.reset(new GameScreen(*this)); break;
    case State::Over:    screen.reset(new GameOverScreen(*this)); break;
    }
}

void Game::run()
{
    sf::VideoMode videoMode(1024, 768);
    wnd.reset(new sf::RenderWindow(videoMode, "LD30"));
    setState(State::Menu);

    sf::Clock clock;

    while (wnd->isOpen()) {
        screen->handleInput();
        screen->update(clock.restart().asSeconds());
        screen->draw();
    }
}

