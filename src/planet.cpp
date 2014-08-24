#include "planet.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "utils.h"

Planet::Planet(float mass,
               const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}, sf::Color::Red),
    speedLimit(100.0f)
{
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
}

PlanetSystem::PlanetSystem(float mass1,
                           const sf::Vector2f& initialPos1,
                           float mass2,
                           const sf::Vector2f& initialPos2):
    first(mass1, initialPos1),
    second(mass2, initialPos2)
{
}

void PlanetSystem::update(float dt)
{
    first.update(dt);
    second.update(dt);
}

void PlanetSystem::draw(sf::RenderTarget& rt,
                        sf::RenderStates /*states*/) const
{
    sf::Vertex linePoints[2] {
        { first.sprite.getPosition() },
        { second.sprite.getPosition() }
    };

    rt.draw(linePoints, 2, sf::Lines);

    rt.draw(first);
    rt.draw(second);
}


bool PlanetSystem::ropeCollidesWith(const sf::Vector2f& center,
                                    float radius)
{
    sf::Vector2f v1 = center - first.sprite.getPosition();
    sf::Vector2f v2 = center - second.sprite.getPosition();
    sf::Vector2f first2second = second.sprite.getPosition() - first.sprite.getPosition();
    sf::Vector2f f2sNormalized = normalized(first2second);

    float dot1 = dot(normalized(v1), f2sNormalized);
    float dot2 = dot(normalized(v2), -f2sNormalized);

    if (dot1 < 0.0f || dot2 < 0.0f) {
        return false;
    }

    float dist = length(v1) * std::sqrt(1.0f - dot1 * dot1);
    return dist <= radius;
}

sf::Vector2f PlanetSystem::getRopeNormal() const
{
    sf::Vector2f first2second = second.sprite.getPosition() - first.sprite.getPosition();
    return normalized({ first2second.y, -first2second.x });
}

