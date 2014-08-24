#pragma once

#include "screen.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Game;

class MenuScreen:
    public Screen
{
public:
    MenuScreen(Game& game);

    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;

private:
    sf::Texture tutorialTex;
    sf::Sprite tutorial;
};

