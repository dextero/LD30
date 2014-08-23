#include "planet.h"

#include <SFML/Graphics/RenderTarget.hpp>

Planet::Planet(float mass,
               const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}, sf::Color::Red)
{
}

void Planet::update(float dt)
{
    sprite.move(acceleration * dt * dt);
}

