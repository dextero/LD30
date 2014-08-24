#include "sun.h"

#include "config.h"
#include "utils.h"

#include <SFML/Graphics/Texture.hpp>

inline sf::Color colorForMass(float mass)
{
    float g = 255.0f * clamp((mass - SUN_RED_GIANT_THRESHOLD) / (SUN_INITIAL_MASS - SUN_RED_GIANT_THRESHOLD), 0.0f, 1.0f);
    return { 255, (sf::Uint8)g, 0 };
}


Sun::Sun(float mass,
         const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}),
    isRedGiant(false),
    isBlackHole(false),
    redGiantExpandFactor(1.0f)
{
    static sf::Texture sunTexture;
    if (sunTexture.getSize().x == 0) {
        if (!sunTexture.loadFromFile("data/sun.png")) {
            abort();
        }
    }

    sprite.setTexture(sunTexture);
    scaleToRadius();

    immovable = true;
}

void Sun::update(float dt)
{
    Asteroid::update(dt);

    if (!isRedGiant) {
        return;
    }

    radius += RED_GIANT_EXPAND_SPEED * dt * redGiantExpandFactor;
    redGiantExpandFactor *= 1.1f;

    scaleToRadius();
}

void Sun::setMass(float newMass)
{
    if (isRedGiant) {
        return;
    }

    if (isBlackHole) {
        sprite.setColor(sf::Color::Black);
        Asteroid::setMass(mass < newMass ? newMass : mass);
    } else {
        sprite.setColor(colorForMass(newMass));
        Asteroid::setMass(newMass);
    }
}

void Sun::turnIntoRedGiant()
{
    isRedGiant = true;
    sprite.setColor(sf::Color::Red);
}

