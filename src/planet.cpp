#include "planet.h"

#include <cassert>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "utils.h"

Planet::Planet(float mass,
               const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}),
    speedLimit(100.0f)
{
    static sf::Texture planetTexture;
    if (planetTexture.getSize().x == 0) {
        if (!planetTexture.loadFromFile("data/planet.png")) {
            abort();
        }
    }

    sprite.setTexture(planetTexture);
    sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) / 2.0f);
}

void Planet::update(float dt)
{
    sf::Vector2f delta = acceleration * dt;
    float speed = length(delta) / dt;

    if (speed == 0.0f) {
        return;
    }

    if (speed > speedLimit) {
        speed = speedLimit;
        speedLimit = speedLimit + 100.0f;
    }
    if (velocityLimit >= 0.0f) {
        speed = std::min(speed, velocityLimit);
    }

    delta = normalized(delta) * speed;

    sprite.move(delta);

    acceleration = { 0.0f, 0.0f };

    updateAttached(dt);
}

void Planet::attach(Asteroid&& asteroid)
{
    assert(!attached);

    sf::Vector2f delta = asteroid.sprite.getPosition() - sprite.getPosition();
    attachedRadius = length(delta);
    attachedAngle = std::atan2(delta.y, delta.x);
    attachedLinearSpeed = length(asteroid.velocity);

    attached.reset(new Asteroid(asteroid));
}

Asteroid Planet::detach()
{
    assert(attached);

    Asteroid ret = *attached;
    attached.reset();
    return ret;
}

void Planet::draw(sf::RenderTarget& rt,
                  sf::RenderStates states) const
{
    if (attached) {
        sf::Vertex linePoints[2] {
            { sprite.getPosition() },
            { attached->sprite.getPosition() }
        };

        rt.draw(linePoints, 2, sf::Lines);
        rt.draw(*attached);
    }

    Asteroid::draw(rt, states);
}

void Planet::updateAttached(float dt)
{
    if (!attached) {
        return;
    }

    attachedAngle += attachedLinearSpeed / attachedRadius * dt;

    sf::Vector2f offset(std::cos(attachedAngle), std::sin(attachedAngle));
    offset *= attachedRadius;

    attached->sprite.setPosition(sprite.getPosition() + offset);
    attached->acceleration = { 0.0f, 0.0f };

    attached->velocity = { -offset.y, offset.x };
}

