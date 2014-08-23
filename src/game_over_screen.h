#pragma once

#include "screen.h"

class Game;

class GameOverScreen:
    public Screen
{
public:
    GameOverScreen(Game& game);

    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;
};
