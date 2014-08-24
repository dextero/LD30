#pragma once

#include "utils.h"

struct Powerup
{
    enum class Type
    {
        BlackHole,

        _Count,
    };

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
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    float radius;
};

