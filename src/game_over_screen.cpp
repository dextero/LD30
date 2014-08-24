#include "game_over_screen.h"

#include <sstream>
#include <fstream>
#include <algorithm>

#include "game.h"

GameOverScreen::GameOverScreen(Game& game,
                               ssize_t points):
    Screen(game),
    points(points)
{
    std::ifstream hiscoreFile("scores");
    if (!hiscoreFile.is_open()) {
        return;
    }

    while (hiscoreFile) {
        HiscoreEntry entry;
        hiscoreFile >> entry.nick >> entry.score;
        if (!hiscoreFile.eof()) {
            hiscore.push_back(entry);
        }
    }
}

void GameOverScreen::saveScores()
{
    std::ofstream file("scores");

    if (currName.empty()) {
        currName = "anonymous";
    }
    hiscore.push_back({ currName, points });

    std::sort(hiscore.begin(), hiscore.end());
    std::reverse(hiscore.begin(), hiscore.end());

    for (size_t i = 0; i < std::min(hiscore.size(), (size_t)10); ++i) {
        file << hiscore[i].nick << " " << hiscore[i].score << "\n";
    }
}

void GameOverScreen::handleInput()
{
    sf::Event evt;
    
    while (wnd->pollEvent(evt)) {
        switch (evt.type) {
        case sf::Event::TextEntered:
            {
                char c = (char)evt.text.unicode;
                if (isgraph(c)) {
                    currName += c;
                }
            }
            break;
        case sf::Event::KeyReleased:
            switch (evt.key.code) {
            case sf::Keyboard::Escape:
                wnd->close();
                break;
            case sf::Keyboard::Space:
                saveScores();
                game.setState(Game::State::Running);
                break;
            case sf::Keyboard::BackSpace:
                if (!currName.empty()) {
                    currName = currName.substr(0, currName.size() - 1);
                }
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

    std::vector<std::string> lines {
        "Game over!\n",
        "Your planet collided with a star\n",
        ((std::stringstream&)(std::stringstream() << "You scored: " << points << " points\n")).str(),
        "",
        "Type your name: " + currName,
        "Press SPACE to retry or ESC to exit",
        "",
        "Top players:",
        ""
    };

    for (size_t i = 0; i < std::min(hiscore.size(), (size_t)10); ++i) {
        lines.push_back(((std::stringstream&)(std::stringstream() << hiscore[i].nick << ": " << hiscore[i].score << "\n")).str());
    }

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

