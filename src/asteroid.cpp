#include "asteroid.h"

#include <SFML/Graphics/RenderTarget.hpp>

Asteroid::Asteroid(float mass,
                   const sf::Vector2f& initialPos,
                   const sf::Vector2f& initialVelocity,
                   const sf::Vector2f& initialAcceleration,
                   const sf::Color& color):
    velocity(initialVelocity),
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

void Asteroid::setMass(float newMass)
{
    mass = newMass;
    radius = std::sqrt(mass / M_PI);
    sprite.setRadius(radius);
    sprite.setOrigin(radius, radius);
}

void Asteroid::draw(sf::RenderTarget& rt,
                  sf::RenderStates states) const
{
    rt.draw(sprite, states);
}

void Asteroid::update(float dt)
{
    velocity += acceleration * dt;
    sprite.move(velocity * dt);
}

