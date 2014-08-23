#include "game_screen.h"

#include <sstream>

#include <SFML/Graphics/Text.hpp>

#include "utils.h"
#include "game.h"

GameScreen::GameScreen(Game& game):
    Screen(game),
    points(0)
{
    planet = Planet(PLANET_MASS, sf::Vector2f(-100.f, 0.f));

    sun = Sun(SUN_INITIAL_MASS, {});
    sun.immovable = true;

    accelerationLimit = 100.0f;
}

void GameScreen::handleInput()
{
    sf::Event evt;
    
    while (wnd->pollEvent(evt)) {
        switch (evt.type) {
        case sf::Event::KeyPressed: onKeyPressed(evt); break;
        case sf::Event::KeyReleased: onKeyReleased(evt); break;
        //case sf::Event::MouseMoved: onMouseMoved(evt); break;
        default:
            break;
        }
    }
}

void GameScreen::update(float dt)
{
    static float timeAccumulator = 0.0f;

    timeAccumulator += dt;
    while (timeAccumulator > UPDATE_STEP_S) {
        timeAccumulator -= UPDATE_STEP_S;
        doUpdateStep(UPDATE_STEP_S);

        planet.sprite.move(planetMoveDir);
        sun.setMass(sun.mass - SUN_VAPORIZE_SPEED * dt);

        if (sun.mass <= SUN_RED_GIANT_THRESHOLD) {
            sun.turnIntoRedGiant();
        } else if (sun.mass >= SUN_BLACK_HOLE_THRESHOLD) {
            sun.turnIntoBlackHole();
        }

        sun.update(dt);
        messages.update(dt);
        updateShake(dt);
    }
}

void GameScreen::draw() const
{
    wnd->clear(sf::Color(0, 0, 50));
    wnd->setView(sf::View(moveRect(viewRect, shakeOffset)));

    wnd->draw(planet);
    wnd->draw(sun);
    for (const Asteroid& a: asteroids) {
        //printf("asteroid @ %f %f\n", a.sprite.getPosition().x, a.sprite.getPosition().y);
        wnd->draw(a);
    }

    for (const Explosion& e: explosions) {
        wnd->draw(e);
    }

    std::stringstream ss;
    ss << points;
    sf::Text pointsText(ss.str(), game.font);
    pointsText.setPosition(viewRect.left + 5, viewRect.top + 5);
    wnd->draw(pointsText);

    wnd->draw(messages);

    wnd->display();
}

void GameScreen::onKeyPressed(const sf::Event& evt)
{
    switch (evt.key.code) {
    case sf::Keyboard::Escape:
        wnd->close();
        break;
    case sf::Keyboard::Left:  planetMoveDir.x = clamp(planetMoveDir.x - PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    case sf::Keyboard::Right: planetMoveDir.x = clamp(planetMoveDir.x + PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    case sf::Keyboard::Up:    planetMoveDir.y = clamp(planetMoveDir.y - PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    case sf::Keyboard::Down:  planetMoveDir.y = clamp(planetMoveDir.y + PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    default:
        break;
    }
}

void GameScreen::onKeyReleased(const sf::Event& evt)
{
    switch (evt.key.code) {
    case sf::Keyboard::Escape:
        wnd->close();
        break;
    case sf::Keyboard::Left:  planetMoveDir.x = clamp(planetMoveDir.x + PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    case sf::Keyboard::Right: planetMoveDir.x = clamp(planetMoveDir.x - PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    case sf::Keyboard::Up:    planetMoveDir.y = clamp(planetMoveDir.y + PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    case sf::Keyboard::Down:  planetMoveDir.y = clamp(planetMoveDir.y - PLANET_SPEED, -PLANET_SPEED, PLANET_SPEED); break;
    default:
        break;
    }
}

void GameScreen::onMouseMoved(const sf::Event& evt)
{
    //printf("onMouseMoved %d %d\n", evt.mouseMove.x, evt.mouseMove.y);
    planet.sprite.setPosition((float)evt.mouseMove.x + viewRect.left,
                              (float)evt.mouseMove.y + viewRect.top);
}

void GameScreen::updateForces(const std::vector<Asteroid*> allObjects,
                              float dt)
{
    for (Asteroid* a1: allObjects) {
        if (a1->immovable) {
            continue;
        }

        sf::Vector2f totalForce(0.0f, 0.0f);

        for (Asteroid* a2: allObjects) {
            if (a1 == a2) {
                continue;
            }

            sf::Vector2f delta = a2->sprite.getPosition() - a1->sprite.getPosition();
            sf::Vector2f dir = normalized(delta);
            totalForce += dir * G * (a1->mass * a2->mass) / lengthSq(delta);
        }

        a1->acceleration += totalForce * dt;
        if (a1 == &planet && !sun.isBlackHole) {
            a1->acceleration = normalized(a1->acceleration) * std::sqrt(length(a1->acceleration));
        }

        if (sun.isBlackHole) {
            accelerationLimit = distance(a1->sprite.getPosition(), sun.sprite.getPosition()) / UPDATE_STEP_S;
        }
        a1->acceleration = normalized(a1->acceleration) * clamp(length(a1->acceleration), 0.0f, accelerationLimit);
        //printf("acceleration = %f %f\n", a1->acceleration.x, a1->acceleration.y);
    }
}

void GameScreen::shakeScreen(float factor)
{
    screenShakeFactor = std::max(screenShakeFactor, clamp(factor, 0.0f, 100.0f));
}

void GameScreen::gameOver()
{
    game.setState(Game::State::Over);
}

void GameScreen::handleCollision(Asteroid* first,
                                 Asteroid* second,
                                 const sf::Vector2f& collisionPos)
{
    if (first->immovable && second->immovable) {
        printf("uh oh. TODO\n");
        abort();
        return;
    }

    if (second->immovable) {
        std::swap(first, second);
    }

    if (first == &sun && second == &planet) {
        gameOver();
    } else if (first == &planet) {
        shakeScreen(std::exp(second->mass / 100.0f));

        addPoints(-(ssize_t)second->mass, collisionPos);
    } else if (first == &sun) {
        sun.setMass(sun.mass + second->mass);

        addPoints((ssize_t)second->mass, collisionPos);
    } else {
        float newMass = first->mass + second->mass;
        first->acceleration = (first->acceleration * first->mass + second->acceleration * second->mass) / newMass;
        first->setMass(newMass);

        addPoints((ssize_t)std::sqrt(newMass), collisionPos);
    }

    // FIXME: remove `second` in a 'prettier' way
    second->sprite.setPosition(100000.0f, 100000.0f);

    if (!sun.isBlackHole) {
        explosions.push_back(Explosion(collisionPos));
        shakeScreen(first->mass / 4.0f / distance(collisionPos, planet.sprite.getPosition()));
    }
}

void GameScreen::checkCollisions(const std::vector<Asteroid*> allObjects)
{
    for (size_t i = 0; i < allObjects.size(); ++i) {
        for (size_t j = i + 1; j < allObjects.size(); ++j) {
            Asteroid* a1 = allObjects[i];
            Asteroid* a2 = allObjects[j];

            const sf::Vector2f& pos1 = a1->sprite.getPosition();
            const sf::Vector2f& pos2 = a2->sprite.getPosition();

            if (distance(pos1, pos2) < a1->radius + a2->radius) {
                sf::Vector2f collisionPos = pos1 + normalized(pos2 - pos1) * a1->radius;
                handleCollision(a1, a2, collisionPos);
            }
        }
    }
}

void GameScreen::spawnAsteroids(float dt)
{
    static float timeAccumulator = 0.0f;

    timeAccumulator += dt;

    while (timeAccumulator > ASTEROID_SPAWN_DELAY_S) {
        timeAccumulator -= ASTEROID_SPAWN_DELAY_S;

        float initialAngle = rand_float(0, 2 * M_PI);
        float initialDistance = 300.0f;
        sf::Vector2f initialPos =
                sf::Vector2f(std::cos(initialAngle) * initialDistance,
                             std::sin(initialAngle) * initialDistance);
        sf::Vector2f initialVelocity = rand_vector()
                * rand_float(MIN_ASTEROID_INITIAL_VELOCITY,
                             MAX_ASTEROID_INITIAL_VELOCITY);
        float mass = rand_float(MIN_ASTEROID_MASS, MAX_ASTEROID_MASS);

        asteroids.push_back({ mass, initialPos, initialVelocity, {},
                              sf::Color::Blue });
    }
}

void GameScreen::removeOutOfBounds()
{
    //printf("viewRect is %f %f - %f %f\n", viewRect.left, viewRect.top, viewRect.width, viewRect.height);

    for (size_t i = 0; i < asteroids.size();) {
        const sf::Vector2f& pos = asteroids[i].sprite.getPosition() / 2.0f;

        if (!viewRect.contains(pos)) {
            if (i != asteroids.size() - 1) {
                asteroids[i] = std::move(asteroids.back());
            }
            asteroids.pop_back();
        } else {
            ++i;
        }
    }
}

void GameScreen::doUpdateStep(float dt)
{
    std::vector<Asteroid*> allObjects { &sun, &planet };
    for (Asteroid& a: asteroids) {
        allObjects.push_back(&a);
    }

    checkCollisions(allObjects);
    updateForces(allObjects, dt);

    for (Asteroid* p: allObjects) {
        p->update(dt);
    }
    for (Explosion& e: explosions) {
        e.update(dt);
    }

    removeOutOfBounds();
    spawnAsteroids(dt);
}

void GameScreen::updateShake(float dt)
{
    static float upTo2PiAccumulator = 0.0f;
    upTo2PiAccumulator = std::fmod(upTo2PiAccumulator + dt, (float)M_PI * 2.0f);
    shakeOffset = {
        std::sin(upTo2PiAccumulator * 34.0f) * screenShakeFactor,
        std::cos(upTo2PiAccumulator * 43.0f) * screenShakeFactor
    };

    screenShakeFactor *= 0.97;
    if (screenShakeFactor < 1.0f) {
        screenShakeFactor = 0.0f;
    }
}

void GameScreen::addPoints(ssize_t delta,
                           const sf::Vector2f& source)
{
    sf::Color color = sf::Color::Green;
    if (delta < 0) {
        color = sf::Color::Red;
    }

    points += delta;

    std::stringstream ss;
    ss << delta;
    messages.add(ss.str(), source, color);
}

