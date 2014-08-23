#pragma once

#include "asteroid.h"

struct Planet: public Asteroid
{
    Planet() {}
    Planet(float mass,
           const sf::Vector2f& initialPos);

    virtual void update(float dt);
};

