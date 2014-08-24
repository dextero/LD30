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

void Asteroid::rebound(const sf::Vector2f& normal)
{
    velocity -= 2.0f * normal * dot(normal, velocity);
    acceleration -= 2.0f * normal * dot(normal, acceleration);
}

void Asteroid::attractTo(const sf::Vector2f& target,
                         float targetMass,
                         float dt)
{
    sf::Vector2f delta = target - sprite.getPosition();
    sf::Vector2f dir = normalized(delta);
    acceleration += dir * G * (mass * targetMass) / lengthSq(delta) / mass * dt;
}
