#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "planet.h"
#include "sun.h"
#include "explosion.h"
#include "screen.h"
#include "popup_messages.h"
#include "powerup.h"

class GameScreen:
    public Screen
{
public:
    GameScreen(Game& game);
    virtual ~GameScreen() {}
    
    virtual void handleInput();
    virtual void update(float dt);
    virtual void draw() const;

    ssize_t getPoints() const { return points; }

private:
    float screenShakeFactor;
    sf::Vector2f shakeOffset;

    Sun sun;

    sf::Vector2f planetMoveDir;
    Planet planet;
    std::vector<Asteroid> asteroids;
    std::vector<Explosion> explosions;
    std::vector<Powerup> powerups;
    ssize_t selectedAsteroid;

    float crosshairAngle;
    float crosshairMoveDir;
    sf::Texture crosshairTexture;
    sf::Sprite crosshair;

    ssize_t points;

    PopupMessages messages;
    float gameOverDelay;
    float followTimer;

    ssize_t findClosestTo(const sf::Vector2f& pos);
    void attractSelected();
    void setCrosshairMoveDir(int dir);

    void onKeyPressed(const sf::Event& evt);
    void onKeyReleased(const sf::Event& evt);
    void onMouseMoved(const sf::Event& evt);

    void updateForces(const std::vector<Asteroid*> allObjects,
                      float dt);
    void shakeScreen(float factor);
    void gameOver();

    void handleCollision(Asteroid* first,
                         Asteroid* second,
                         const sf::Vector2f& collisionPos);
    void checkCollisions(const std::vector<Asteroid*> allObjects);
    void checkPowerupCollisions();

    void spawnAsteroids(float dt);
    void applyPowerup(Powerup& powerup);
    void updatePowerups(float dt);
    void spawnPowerups(float dt);
    void removeOutOfBounds();

    void doUpdateStep(float dt);
    void updateShake(float dt);

    void addPoints(ssize_t delta,
                   const sf::Vector2f& source);
};

