#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

struct Planet: public sf::Drawable
{
    sf::Vector2f acceleration;
    float mass;
    float radius;
    sf::CircleShape sprite;
    bool immovable;

    Planet() {}
    Planet(float mass,
           const sf::Vector2f& initialPos,
           const sf::Vector2f& initialAcceleration = { 0.0f, 0.0f },
           const sf::Color& color = sf::Color::Red);

    void setMass(float newMass);

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;
};

