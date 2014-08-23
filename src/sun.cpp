#include "sun.h"

#include "config.h"

inline sf::Color colorForMass(float mass)
{
    float g = 255.0f;
    if (mass > SUN_INITIAL_MASS) {
        g -= 255.0f * (mass - SUN_INITIAL_MASS) / (SUN_BLACK_HOLE_THRESHOLD - SUN_INITIAL_MASS);
    } else {
        g -= 255.0f * (1.0f - (mass - SUN_RED_GIANT_THRESHOLD) / (SUN_INITIAL_MASS - SUN_RED_GIANT_THRESHOLD));
    }
    return { 255, (sf::Uint8)g, 0 };
}


Sun::Sun(float mass,
         const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}, colorForMass(mass)),
    isRedGiant(false),
    isBlackHole(false)
{
}

void Sun::update(float dt)
{
    Asteroid::update(dt);

    if (!isRedGiant) {
        return;
    }

    radius += RED_GIANT_EXPAND_SPEED * dt;
    sprite.setRadius(radius);
    sprite.setOrigin(radius, radius);
}

void Sun::setMass(float newMass)
{
    if (isRedGiant || isBlackHole) {
        return;
    }

    Asteroid::setMass(newMass);
    sprite.setFillColor(colorForMass(newMass));
}

void Sun::turnIntoRedGiant()
{
    isRedGiant = true;
    sprite.setFillColor(sf::Color::Red);
}

void Sun::turnIntoBlackHole()
{
    isBlackHole = true;
    mass = 1000000.0f;
    radius = 0.0f;
    sprite.setRadius(BLACK_HOLE_RADIUS);
    sprite.setOrigin(BLACK_HOLE_RADIUS, BLACK_HOLE_RADIUS);
    sprite.setFillColor(sf::Color::Black);
}

