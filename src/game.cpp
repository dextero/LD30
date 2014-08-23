#include "game.h"

Game::Game()
{
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        abort();
    }
}

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
        std::shared_ptr<Screen> currScreen = screen;

        currScreen->handleInput();
        currScreen->update(clock.restart().asSeconds());
        currScreen->draw();
    }
}

