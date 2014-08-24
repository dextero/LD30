#include "game.h"

#include "config.h"

Game::Game()
{
    if (!font.loadFromFile("data/DejaVuSans.ttf")) {
        printf("cannot load data/DejaVuSans.ttf\n");
        abort();
    }
}

void Game::setState(State state)
{
    switch (state) {
    case State::Menu:
        screen.reset(new MenuScreen(*this));
        break;
    case State::Running:
        screen.reset(new GameScreen(*this));
        break;
    case State::Over:
        {
            const GameScreen& prev = dynamic_cast<GameScreen&>(*screen);
            screen.reset(new GameOverScreen(*this, prev.getPoints()));
            break;
        }
    }
}

void Game::run()
{
    sf::VideoMode videoMode(WINDOW_WIDTH, WINDOW_HEGIHT);
    wnd.reset(new sf::RenderWindow(videoMode, WINDOW_TITLE));
    setState(State::Menu);

    sf::Clock clock;

    while (wnd->isOpen()) {
        std::shared_ptr<Screen> currScreen = screen;

        if (currScreen) {
            currScreen->handleInput();
            currScreen->update(clock.restart().asSeconds());
            currScreen->draw();
        }
    }
}

