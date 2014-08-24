#include "menu_screen.h"

#include "game.h"

MenuScreen::MenuScreen(Game& game):
    Screen(game)
{
    if (!tutorialTex.loadFromFile("data/tutorial.png")) {
        abort();
    }

    tutorial.setTexture(tutorialTex);
    tutorial.setOrigin(sf::Vector2f(tutorialTex.getSize()) / 2.0f);
}

void MenuScreen::handleInput()
{
    sf::Event evt;
    
    while (wnd->pollEvent(evt)) {
        switch (evt.type) {
        case sf::Event::KeyReleased:
            switch (evt.key.code) {
            case sf::Keyboard::Escape:
                wnd->close();
                break;
            case sf::Keyboard::Space:
                game.setState(Game::State::Running);
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void MenuScreen::update(float /*dt*/)
{
}

void MenuScreen::draw() const
{
    wnd->clear(sf::Color::Black);
    wnd->setView(sf::View(viewRect));
    wnd->draw(tutorial);
    wnd->display();
}

