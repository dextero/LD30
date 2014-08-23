#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "planet.h"
#include "sun.h"
#include "explosion.h"
#include "screen.h"

class GameScreen:
    public Screen
{
public:
    GameScreen(Game& game);
    
    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;

private:
    float screenShakeFactor;
    sf::Vector2f shakeOffset;

    sf::Vector2f planetMoveDir;
    Sun sun;
    Planet planet;
    std::vector<Planet> asteroids;
    std::vector<Explosion> explosions;

    float accelerationLimit;

    void onKeyPressed(const sf::Event& evt);
    void onKeyReleased(const sf::Event& evt);
    void onMouseMoved(const sf::Event& evt);

    void updateForces(const std::vector<Planet*> allObjects,
                      float dt);
    void shakeScreen(float factor);
    void gameOver();

    void handleCollision(Planet* first,
                         Planet* second,
                         const sf::Vector2f& collisionPos);
    void checkCollisions(const std::vector<Planet*> allObjects);

    void spawnAsteroids(float dt);
    void removeOutOfBounds();

    void doUpdateStep(float dt);
    void updateShake(float dt);
};

