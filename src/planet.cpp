#include "planet.h"

#include <SFML/Graphics/RenderTarget.hpp>

Planet::Planet(float mass,
       const sf::Vector2f& initialPos,
       const sf::Vector2f& initialAcceleration,
       const sf::Color& color):
    acceleration(initialAcceleration),
    mass(0.0f),
    radius(0.0f),
    sprite(sf::CircleShape(radius)),
    immovable(false)
{
    setMass(mass);

    sprite.setPosition(initialPos);
    sprite.setOrigin(radius, radius);
    sprite.setFillColor(color);
}

void Planet::setMass(float newMass)
{
    mass = newMass;
    radius = std::sqrt(mass / M_PI);
    sprite.setRadius(radius);
    sprite.setOrigin(radius, radius);
}

void Planet::draw(sf::RenderTarget& rt,
                  sf::RenderStates states) const
{
    rt.draw(sprite, states);
}

