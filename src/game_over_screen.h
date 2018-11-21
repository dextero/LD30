#pragma once

#include <vector>
#include <string>

#include "screen.h"

#include <SFML/Graphics/Sprite.hpp>

class Game;

class GameOverScreen:
    public Screen
{
public:
    GameOverScreen(Game& game,
                   ssize_t points);
    virtual ~GameOverScreen() {}

    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;

private:
    struct HiscoreEntry
    {
        std::string nick;
        ssize_t score;

        bool operator <(const HiscoreEntry& e) const {
            return score < e.score;
        }
    };

    const ssize_t points;
    std::vector<HiscoreEntry> hiscore;
    std::string currName;

    sf::Sprite planetSprite;
    sf::Sprite sunSprite;
    sf::Sprite asteroidSprite;

    void saveScores();
};
