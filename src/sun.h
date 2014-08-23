#pragma once

#include "planet.h"

struct Sun: public Planet
{
    bool isRedGiant;
    bool isBlackHole;

    Sun() {}
    Sun(float mass,
        const sf::Vector2f& initialPos);

    void update(float dt);
    void setMass(float newMass);

    void turnIntoRedGiant();
    void turnIntoBlackHole();
};

