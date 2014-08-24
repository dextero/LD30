#include "explosion.h"

#include "utils.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Explosion::Explosion(const sf::Vector2f& pos,
                     size_t numCircles,
                     float scale):
    pos(pos)
{
    //circles.push_back({ 0.0f, 3.0f, 10.0f, { 0.0f, 0.0f }, sf::Color::Yellow });
    circles.reserve(numCircles);
    for (size_t i = 0; i < numCircles; ++i) {
        float tts = rand_float(0.0f, MAX_EXPLOSION_START_TIME);
        float tth = rand_float(tts, MAX_EXPLOSION_TIME);
        float maxRadius = rand_float(MIN_EXPLOSION_RADIUS, MAX_EXPLOSION_RADIUS);
        sf::Vector2f offset(rand_float(-MAX_EXPLOSION_OFFSET, MAX_EXPLOSION_OFFSET),
                            rand_float(-MAX_EXPLOSION_OFFSET, MAX_EXPLOSION_OFFSET));
        sf::Color color(255, (sf::Uint8)rand_float(0.0, 255.0), 0);

        circles.push_back({ tts, tth, maxRadius * scale, offset * scale, color });
    }
}

void Explosion::update(float dt)
{
    for (Circle& c: circles) {
        c.timeToShow -= dt;
        c.timeToHide -= dt;
    }
}

void Explosion::draw(sf::RenderTarget& rt,
                     sf::RenderStates states) const
{
    sf::CircleShape shape;
    shape.setPosition(pos);

    for (const Circle& c: circles) {
        if (c.timeToShow > 0.0f || c.timeToHide <= 0.0f) {
            continue;
        }

        float progress = (MAX_EXPLOSION_TIME - c.timeToHide) / MAX_EXPLOSION_TIME;
        float radius = c.maxRadius * progress;
        sf::Uint8 alpha = 255 - (sf::Uint8)(255.0f * progress * progress * progress * progress);

        shape.setRadius(radius);
        shape.setOrigin(c.offset.x + radius, c.offset.y + radius);
        shape.setFillColor(sf::Color(c.color.r, c.color.g, c.color.b, alpha));

        rt.draw(shape, states);
    }
}

