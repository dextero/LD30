#pragma once

#include <random>
#include <cassert>
#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "config.h"

inline float rand_float(float min,
                        float max)
{
    static std::random_device random_device;
    static std::mt19937 random_gen;

    return std::uniform_real_distribution<float>(min, max)(random_gen);
}

inline int rand_int(int min,
                    int max)
{
    static std::random_device random_device;
    static std::mt19937 random_gen;

    return std::uniform_int_distribution<int>(min, max)(random_gen);
}

inline sf::Vector2f rand_vector()
{
    float angle = rand_float(0.0f, M_PI);
    return { std::cos(angle), std::sin(angle) };
}

constexpr inline float dot(const sf::Vector2f& a,
                           const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

constexpr inline float lengthSq(const sf::Vector2f& v)
{
    return dot(v, v);
}

constexpr inline float length(const sf::Vector2f& v)
{
    return std::sqrt(lengthSq(v));
}

inline float distance(const sf::Vector2f& a,
                      const sf::Vector2f& b)
{
    return std::abs(length(a - b));
}

inline sf::Vector2f normalized(const sf::Vector2f& v)
{
    return v / length(v);
}

template<typename T>
inline T clamp(T value, T min, T max)
{
    return std::min(max, std::max(value, min));
}

inline sf::FloatRect moveRect(const sf::FloatRect& rect,
                              const sf::Vector2f& offset)
{
    return sf::FloatRect(rect.left + offset.x, rect.top + offset.y,
                         rect.width, rect.height);
}

inline sf::Vector2f rotate(const sf::Vector2f& v,
                           float angle)
{
    return sf::Transform().rotate(angle * 180.0f / M_PI).transformPoint(v);
}

template<typename T>
inline void swapAndPop(std::vector<T>& container,
                       size_t idx)
{
    assert(idx < container.size());

    if (idx != container.size() - 1) {
        container[idx] = std::move(container.back());
    }
    container.pop_back();
}

