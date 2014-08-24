#include "game_over_screen.h"

#include <sstream>

#include "game.h"

GameOverScreen::GameOverScreen(Game& game,
                               ssize_t points):
    Screen(game),
    points(points)
{
}

void GameOverScreen::handleInput()
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

void GameOverScreen::update(float /*dt*/)
{
}

void GameOverScreen::draw() const
{
    static constexpr float TEXT_HEIGHT = 25.0f;
    static constexpr float LINE_SPACING = 3.0f;

    const std::vector<std::string> lines {
        "Game over!\n",
        "Your planet collided with a star\n",
        ((std::stringstream&)(std::stringstream() << "You scored: " << points << " points\n")).str(),
        "",
        "Press SPACE to retry or ESC to exit"
    };

    float yPos = -((float)lines.size() * (TEXT_HEIGHT + LINE_SPACING)) / 2.0f;

    wnd->clear();

    for (const std::string& line: lines) {
        sf::Text text(line, game.font, (unsigned)TEXT_HEIGHT);
        sf::FloatRect rect = text.getLocalBounds();

        text.setOrigin(rect.left + rect.width / 2.0f,
                       rect.top + rect.height / 2.0f);
        text.setPosition(0.f, yPos);
        wnd->draw(text);

        yPos += TEXT_HEIGHT + LINE_SPACING;
    }

    wnd->display();
}

