#pragma once

#include "screen.h"

class Game;

class GameOverScreen:
    public Screen
{
public:
    GameOverScreen(Game& game,
                   ssize_t points);

    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;

private:
    const ssize_t points;
};
