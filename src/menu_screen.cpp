#include "menu_screen.h"

#include "game.h"

MenuScreen::MenuScreen(Game& game):
    Screen(game),
    tutorial(tutorialTex)
{
    if (!tutorialTex.loadFromFile("data/tutorial.png")) {
        abort();
    }

    tutorial.setTexture(tutorialTex, true);
    tutorial.setOrigin(sf::Vector2f(tutorialTex.getSize()) / 2.0f);
}

void MenuScreen::handleInput()
{
    while (const std::optional event = wnd->pollEvent()) {
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            switch (keyReleased->code) {
            case sf::Keyboard::Key::Escape:
                wnd->close();
                break;
            case sf::Keyboard::Key::Space:
                game.setState(Game::State::Running);
                break;
            default:
                break;
            }
        }
    }
}

void MenuScreen::update(float /*dt*/)
{
}

void MenuScreen::draw() const
{
    wnd->clear(BACKGROUND_COLOR);
    wnd->setView(sf::View(viewRect));
    wnd->draw(tutorial);
    wnd->display();
}

