#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#include "planet.h"

struct Sun: public Asteroid
{
    bool isRedGiant;

    Sun() {}
    Sun(float mass,
        const sf::Vector2f& initialPos);

    virtual void update(float dt);
    virtual void setMass(float newMass);
    sf::Vector2f getPosition() const { return circle.getPosition(); }

    void turnIntoRedGiant();
    void turnIntoBlackHole(float timeout);

    bool isBlackHole() { return blackHoleTimeout > 0.0f; }

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;

private:
    sf::Sprite blackHoleSprite;
    sf::CircleShape circle;
    float redGiantExpandFactor;
    float blackHoleTimeout;

    void turnIntoRegularSun();
};

