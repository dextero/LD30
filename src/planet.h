#pragma once

#include "asteroid.h"

struct Planet: public Asteroid
{
    Planet() {}
    Planet(float mass,
           const sf::Vector2f& initialPos);

    virtual void update(float dt);

private:
    float speedLimit;
};

struct PlanetSystem:
    public sf::Drawable
{
    Planet first;
    Planet second;

    PlanetSystem(float mass1,
                 const sf::Vector2f& initialPos1,
                 float mass2,
                 const sf::Vector2f& initialPos2);
    void update(float dt);

    bool ropeCollidesWith(const sf::Vector2f& center,
                          float radius);

    sf::Vector2f getRopeNormal() const;

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;
};

