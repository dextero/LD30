#pragma once

#include "screen.h"

class Game;

class MenuScreen:
    public Screen
{
public:
    MenuScreen(Game& game);

    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;
};

