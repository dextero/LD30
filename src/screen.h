#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

class Game;

class Screen
{
public:
    Screen(Game& game);
    virtual ~Screen() {}

    virtual void handleInput() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() const = 0;

protected:
    Game& game;
    std::shared_ptr<sf::RenderWindow> wnd;
    sf::FloatRect viewRect;
};

