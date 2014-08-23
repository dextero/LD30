#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "config.h"

struct Explosion: public sf::Drawable
{
    struct Circle
    {
        float timeToShow;
        float timeToHide;
        float maxRadius;
        sf::Vector2f offset;
        sf::Color color;
    };

    std::vector<Circle> circles;
    sf::Vector2f pos;

    Explosion(const sf::Vector2f& pos,
              size_t numCircles = EXPLOSION_CIRCLES);

    void update(float dt);

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;
};

