#include "game_screen.h"

#include <sstream>

#include <SFML/Graphics/Text.hpp>

#include "utils.h"
#include "game.h"

GameScreen::GameScreen(Game& game):
    Screen(game),
    screenShakeFactor(0.0f),
    sun(SUN_INITIAL_MASS, {}),
    planet(PLANET_MASS, sf::Vector2f(-300.f, -100.f)),
    selected(nullptr),
    points(0),
    gameOverDelay(-1.0f)
{
    if (!crosshairTexture.loadFromFile("data/crosshair.png")) {
        printf("cannot load data/crosshair.png\n");
        abort();
    }

    crosshair.setTexture(crosshairTexture);
    crosshair.setOrigin(sf::Vector2f(crosshairTexture.getSize()) / 2.0f);
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

        if (gameOverDelay > 0.0f) {
            gameOverDelay -= UPDATE_STEP_S;
            if (gameOverDelay <= 0.0f) {
                game.setState(Game::State::Over);
            }
        } else {
            planet.sprite.move(planetMoveDir * UPDATE_STEP_S);
            crosshair.move(crosshairMoveDir * UPDATE_STEP_S);

            if (selected) {
                selected->attractTo(planet.getPosition(), ATTRACT_MASS, UPDATE_STEP_S);
            }
        }

        sun.setMass(sun.mass - SUN_VAPORIZE_SPEED * UPDATE_STEP_S);

        if (sun.mass <= SUN_RED_GIANT_THRESHOLD) {
            sun.turnIntoRedGiant();
        } else if (sun.mass >= SUN_BLACK_HOLE_THRESHOLD) {
            sun.turnIntoBlackHole();
        }

        sun.update(UPDATE_STEP_S);
        messages.update(UPDATE_STEP_S);
        updateShake(UPDATE_STEP_S);
    }
}

void GameScreen::draw() const
{
    wnd->clear(sf::Color(0, 0, 50));
    wnd->setView(sf::View(moveRect(viewRect, shakeOffset)));

    wnd->draw(planet);
    for (const Asteroid& a: asteroids) {
        //printf("asteroid @ %f %f\n", a.sprite.getPosition().x, a.sprite.getPosition().y);
        wnd->draw(a);
    }
    wnd->draw(sun);

    for (const Explosion& e: explosions) {
        wnd->draw(e);
    }

    std::stringstream ss;
    ss << points;
    sf::Text pointsText(ss.str(), game.font);
    pointsText.setPosition(viewRect.left + 5, viewRect.top + 5);
    wnd->draw(pointsText);

    wnd->draw(messages);
    wnd->draw(crosshair);

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
    case sf::Keyboard::A: crosshairMoveDir.x = clamp(crosshairMoveDir.x - CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::D: crosshairMoveDir.x = clamp(crosshairMoveDir.x + CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::W: crosshairMoveDir.y = clamp(crosshairMoveDir.y - CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::S: crosshairMoveDir.y = clamp(crosshairMoveDir.y + CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::Space:
        if (!selected) {
            selected = findClosestTo(crosshair.getPosition());
            if (selected) {
                selected->sprite.setFillColor(SELECTED_ASTEROID_COLOR);
            }
        }
        break;
    default:
        break;
    }
}

Asteroid* GameScreen::findClosestTo(const sf::Vector2f& pos)
{
    if (asteroids.size() == 0) {
        return nullptr;
    }

    size_t closestIdx = 0;
    float closestDistance = distance(pos, asteroids[0].sprite.getPosition());
    for (size_t i = 1; i < asteroids.size(); ++i) {
        float dist = distance(pos, asteroids[i].sprite.getPosition());
        if (dist < closestDistance) {
            closestIdx = i;
            closestDistance = dist;
        }
    }

    if (closestDistance > MAX_SELECT_DISTANCE) {
        return nullptr;
    }

    return &asteroids[closestIdx];
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
    case sf::Keyboard::A: crosshairMoveDir.x = clamp(crosshairMoveDir.x + CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::D: crosshairMoveDir.x = clamp(crosshairMoveDir.x - CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::W: crosshairMoveDir.y = clamp(crosshairMoveDir.y + CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::S: crosshairMoveDir.y = clamp(crosshairMoveDir.y - CROSSHAIR_SPEED, -CROSSHAIR_SPEED, CROSSHAIR_SPEED); break;
    case sf::Keyboard::Space:
        if (selected) {
            selected->sprite.setFillColor(ASTEROID_COLOR);
        }
        selected = nullptr;
        break;
    default:
        break;
    }
}

void GameScreen::onMouseMoved(const sf::Event& /*evt*/)
{
    //printf("onMouseMoved %d %d\n", evt.mouseMove.x, evt.mouseMove.y);
    //planet.sprite.setPosition((float)evt.mouseMove.x + viewRect.left,
                              //(float)evt.mouseMove.y + viewRect.top);
}

void GameScreen::updateForces(const std::vector<Asteroid*> allObjects,
                              float dt)
{
    for (Asteroid* a1: allObjects) {
        if (a1->immovable) {
            continue;
        }

        for (Asteroid* a2: allObjects) {
            if (a1 == a2) {
                continue;
            }

            a1->attractTo(a2->getPosition(), a2->mass, dt);
        }

        if (a1 == &planet && !sun.isBlackHole) {
            a1->acceleration = normalized(a1->acceleration) * std::sqrt(length(a1->acceleration));
        }

        if (sun.isBlackHole) {
            a1->velocityLimit = distance(a1->sprite.getPosition(), sun.sprite.getPosition()) / dt;
        }
        //printf("acceleration = %f %f\n", a1->acceleration.x, a1->acceleration.y);
    }
}

void GameScreen::shakeScreen(float factor)
{
    screenShakeFactor = std::max(screenShakeFactor, clamp(factor, 0.0f, 100.0f));
}

void GameScreen::gameOver()
{
    gameOverDelay = GAME_OVER_DELAY;
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

    second->markedForDelete = true;

    if (!sun.isBlackHole) {
        explosions.push_back(Explosion(collisionPos));

        float dist = distance(collisionPos, planet.sprite.getPosition());
        shakeScreen(first->mass / 4.0f / dist);
    }
}

void GameScreen::checkCollisions(const std::vector<Asteroid*> allObjects)
{
    for (size_t i = 0; i < allObjects.size(); ++i) {
        Asteroid* a1 = allObjects[i];
        if (a1->markedForDelete) {
            continue;
        }

        for (size_t j = i + 1; j < allObjects.size(); ++j) {
            Asteroid* a2 = allObjects[j];
            if (a2->markedForDelete) {
                continue;
            }

            const sf::Vector2f& pos1 = a1->sprite.getPosition();
            const sf::Vector2f& pos2 = a2->sprite.getPosition();
            sf::Vector2f delta = pos2 - pos1;

            if (lengthSq(delta) < EPSILON) {
                handleCollision(a1, a2, pos1);
            } else if (distance(pos1, pos2) < a1->radius + a2->radius) {
                sf::Vector2f collisionPos = pos1 + normalized(delta) * a1->radius;
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
        float initialDistance = ASTEROID_INITIAL_DISTANCE;
        sf::Vector2f initialPos =
                sf::Vector2f(std::cos(initialAngle) * initialDistance,
                             std::sin(initialAngle) * initialDistance);

        sf::Vector2f initialVelocity =
                rotate(normalized(-initialPos),
                       rand_float(-ASTEROID_INITIAL_ANGLE_VARIANCE,
                                  ASTEROID_INITIAL_ANGLE_VARIANCE))
                * rand_float(MIN_ASTEROID_INITIAL_VELOCITY,
                             MAX_ASTEROID_INITIAL_VELOCITY);

        float mass = rand_float(MIN_ASTEROID_MASS, MAX_ASTEROID_MASS);

        asteroids.push_back({ mass, initialPos, initialVelocity, {},
                              ASTEROID_COLOR });
    }
}

void GameScreen::removeOutOfBounds()
{
    //printf("viewRect is %f %f - %f %f\n", viewRect.left, viewRect.top, viewRect.width, viewRect.height);

    for (size_t i = 0; i < asteroids.size();) {
        bool markedForDelete = asteroids[i].markedForDelete;
        const sf::Vector2f& pos = asteroids[i].sprite.getPosition();

        if (markedForDelete
                || lengthSq(pos) > ASTEROID_MAX_DISTANCE * ASTEROID_MAX_DISTANCE) {
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

    //printf("planet @ %f, %f\n", planet.sprite.getPosition().x, planet.sprite.getPosition().y);
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
    if (sun.isBlackHole) {
        return;
    }

    sf::Color color = sf::Color::Green;
    if (delta < 0) {
        color = sf::Color::Red;
    }

    points += delta;

    std::stringstream ss;
    ss << delta;
    messages.add(ss.str(), source, color);
}

