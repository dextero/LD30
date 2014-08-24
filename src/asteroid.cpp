#include "asteroid.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "utils.h"

sf::Texture asteroidTexture;

Asteroid::Asteroid(float mass,
                   const sf::Vector2f& initialPos,
                   const sf::Vector2f& initialVelocity,
                   const sf::Vector2f& initialAcceleration):
    velocity(initialVelocity),
    velocityLimit(-1.0f),
    acceleration(initialAcceleration),
    mass(0.0f),
    radius(0.0f),
    immovable(false),
    markedForDelete(false)
{
    if (asteroidTexture.getSize().x == 0) {
        if (!asteroidTexture.loadFromFile("data/asteroid.png")) {
            abort();
        }
    }

    sprite.setTexture(asteroidTexture);
    sprite.setPosition(initialPos);
    sprite.setRotation(rand_float(0.0f, 2.0f * M_PI));

    setMass(mass);
}

void Asteroid::setMass(float newMass)
{
    mass = newMass;
    radius = 2.0f * std::sqrt(mass / M_PI);
    scaleToRadius();
}

void Asteroid::draw(sf::RenderTarget& rt,
                    sf::RenderStates states) const
{
    rt.draw(sprite, states);
}

void Asteroid::update(float dt)
{
    //printf("velocity was %f, %f\n", velocity.x, velocity.y);
    velocity += acceleration * dt;
    //printf("velocity is %f, %f\n", velocity.x, velocity.y);
    if (lengthSq(velocity) < EPSILON) {
        return;
    }

    if (velocityLimit >= 0.0f) {
        velocity = normalized(velocity) * std::min(velocityLimit, length(velocity));
        //printf("velocity limited to %f\n", velocityLimit);
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
    //printf("acc was %f, %f\n", acceleration.x, acceleration.y);
    acceleration += dir * G * targetMass / lengthSq(delta) * dt;
    //printf("acc is %f, %f\n", acceleration.x, acceleration.y);
}

void Asteroid::scaleToRadius()
{
    float scale = 2.0f * radius / sprite.getTexture()->getSize().x;
    sprite.setScale(scale, scale);
    sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) / 2.0f);
}
