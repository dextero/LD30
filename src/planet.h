#pragma once

#include <memory>

#include "asteroid.h"

struct Planet: public Asteroid
{
    Planet() {}
    Planet(float mass,
           const sf::Vector2f& initialPos);

    virtual void update(float dt);

    void attach(Asteroid&& asteroid);
    Asteroid detach();

    bool hasAttachedAsteroid() const { return !!attached; }

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;

private:
    std::unique_ptr<Asteroid> attached;
    float attachedLinearSpeed;
    float attachedRadius;
    float attachedAngle;

    float speedLimit;

    void updateAttached(float dt);
};

