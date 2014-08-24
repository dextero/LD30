#pragma once

#include "utils.h"

#include <SFML/Graphics/Drawable.hpp>

struct Powerup: public sf::Drawable
{
    enum class Type
    {
        BlackHole,

        _Count,
    };

    Powerup(Type type,
            const sf::Vector2f& pos,
            const sf::Vector2f& velocity,
            const sf::Sprite& sprite,
            float radius):
        type(type),
        velocity(velocity),
        sprite(sprite),
        radius(radius)
    {
        this->sprite.setPosition(pos);
        this->sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) / 2.0f);
    }

    static sf::Sprite spriteForType(Type type)
    {
        static std::vector<sf::Texture> textures;
        if (textures.size() == 0) {
            for (const std::string& path: { "data/powerup1.png" }) {
                textures.push_back({});

                if (!textures.back().loadFromFile(path)) {
                    abort();
                }
            }
        }

        if ((size_t)type >= textures.size()) {
            abort();
        }

        return sf::Sprite(textures[(size_t)type]);
    }

    static Powerup createRandom(const sf::Vector2f& initialPos,
                                const sf::Vector2f& initialVelocity)
    {
        int value = rand_int(0, (int)Type::_Count - 1);
        Type type = (Type)value;

        return { type, initialPos, initialVelocity, spriteForType(type), POWERUP_RADIUS };
    }

    Type type;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    float radius;

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const
    {
        rt.draw(sprite, states);
    }
};

