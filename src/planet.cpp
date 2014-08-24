#include "planet.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "utils.h"

Planet::Planet(float mass,
               const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}, sf::Color::Red)
{
}

void Planet::update(float dt)
{
    sf::Vector2f delta = acceleration * dt;
    float speed = length(delta) / dt;

    if (speed == 0.0f) {
        return;
    }

    if (speed > speedLimit) {
        speed = speedLimit;
        speedLimit = speedLimit + 100.0f;
    }
    if (velocityLimit >= 0.0f) {
        speed = std::min(speed, velocityLimit);
    }

    delta = normalized(delta) * speed;

    sprite.move(delta);

    acceleration = { 0.0f, 0.0f };
}

