#include "asteroid.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "utils.h"

Asteroid::Asteroid(float mass,
                   const sf::Vector2f& initialPos,
                   const sf::Vector2f& initialVelocity,
                   const sf::Vector2f& initialAcceleration,
                   const sf::Color& color):
    velocity(initialVelocity),
    velocityLimit(-1.0f),
    acceleration(initialAcceleration),
    mass(0.0f),
    radius(0.0f),
    sprite(sf::CircleShape(radius)),
    immovable(false),
    markedForDelete(false)
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
    if (lengthSq(velocity) < EPSILON) {
        return;
    }

    if (velocityLimit >= 0.0f) {
        velocity = normalized(velocity) * velocityLimit;
    }

    sprite.move(velocity * dt);
}

