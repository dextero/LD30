#include "sun.h"

#include "config.h"
#include "utils.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

sf::Texture sunTexture;
sf::Texture bhTexture;

inline sf::Color colorForMass(float mass)
{
    float g = 255.0f * clamp((mass - SUN_RED_GIANT_THRESHOLD) / (SUN_INITIAL_MASS - SUN_RED_GIANT_THRESHOLD), 0.0f, 1.0f);
    return { 255, (sf::Uint8)g, 0 };
}


Sun::Sun(float mass,
         const sf::Vector2f& initialPos):
    Asteroid(mass, initialPos, {}, {}),
    isRedGiant(false),
    redGiantExpandFactor(1.0f),
    blackHoleTimeout(-1.0f)
{
    if (sunTexture.getSize().x == 0) {
        if (!sunTexture.loadFromFile("data/sun.png")) {
            abort();
        }
    }
    if (bhTexture.getSize().x == 0) {
        if (!bhTexture.loadFromFile("data/bh.png")) {
            abort();
        }
    }

    sprite.setTexture(sunTexture);
    scaleToRadius();
    blackHoleSprite.setTexture(bhTexture);
    blackHoleSprite.setOrigin(sf::Vector2f(bhTexture.getSize()) / 2.0f);
    blackHoleSprite.setScale(0.0f, 0.0f);

    immovable = true;
}

void Sun::update(float dt)
{
    Asteroid::update(dt);

    blackHoleSprite.rotate(-180.0f * dt);

    if (blackHoleTimeout > 0.0f) {
        blackHoleTimeout -= dt;

        if (blackHoleTimeout <= 0.0f) {
            turnIntoRegularSun();
        } else {
            float scale = 2.0f * radius / bhTexture.getSize().x;
            float bhScale = scale * clamp(blackHoleTimeout, 0.0f, BLACK_HOLE_TRANSITION_TIME) / BLACK_HOLE_TRANSITION_TIME;
            
            blackHoleSprite.setScale(bhScale, bhScale);

            if (blackHoleTimeout > BLACK_HOLE_TIMEOUT - BLACK_HOLE_TRANSITION_TIME) {
                scale *= (blackHoleTimeout - (BLACK_HOLE_TIMEOUT - BLACK_HOLE_TRANSITION_TIME)) / BLACK_HOLE_TRANSITION_TIME;
            } else {
                scale -= bhScale;
            }

            sprite.setScale(scale, scale);
        }
    }

    if (isRedGiant) {
        radius += RED_GIANT_EXPAND_SPEED * dt * redGiantExpandFactor;
        redGiantExpandFactor *= 1.1f;

        scaleToRadius();
    }
}

void Sun::setMass(float newMass)
{
    if (isRedGiant) {
        return;
    }

    if (blackHoleTimeout > 0.0f) {
        Asteroid::setMass(mass < newMass ? newMass : mass);
    } else {
        sprite.setColor(colorForMass(newMass));
        Asteroid::setMass(newMass);
    }
}

void Sun::turnIntoRedGiant()
{
    isRedGiant = true;
    sprite.setColor(sf::Color::Red);
}

void Sun::turnIntoBlackHole(float timeout)
{
    blackHoleTimeout = timeout;
}

void Sun::turnIntoRegularSun()
{
    blackHoleTimeout = -1.0f;

    scaleToRadius();
    blackHoleSprite.setScale(0.0f, 0.0f);
}

void Sun::draw(sf::RenderTarget& rt,
               sf::RenderStates states) const
{
    rt.draw(blackHoleSprite, states);
    Asteroid::draw(rt, states);
}

