#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

struct Asteroid: public sf::Drawable
{
    sf::Vector2f velocity;
    float velocityLimit;
    sf::Vector2f acceleration;
    float mass;
    float radius;
    sf::CircleShape sprite;
    bool immovable;
    bool markedForDelete;

    Asteroid() {}
    Asteroid(float mass,
             const sf::Vector2f& initialPos,
             const sf::Vector2f& initialVelocity = { 0.0f, 0.0f },
             const sf::Vector2f& initialAcceleration = { 0.0f, 0.0f },
             const sf::Color& color = sf::Color::Red);

    virtual void setMass(float newMass);
    virtual void update(float dt);

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;
};

