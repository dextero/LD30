#include "game_screen.h"

#include "utils.h"
#include "game.h"

GameScreen::GameScreen(Game& game):
    Screen(game)
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
    }

    updateShake(dt);
}

void GameScreen::draw() const
{
    wnd->clear(sf::Color(0, 0, 50));
    wnd->setView(sf::View(moveRect(viewRect, shakeOffset)));

    wnd->draw(planet);
    wnd->draw(sun);
    for (const Planet& a: asteroids) {
        //printf("asteroid @ %f %f\n", a.sprite.getPosition().x, a.sprite.getPosition().y);
        wnd->draw(a);
    }

    for (const Explosion& e: explosions) {
        wnd->draw(e);
    }

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

void GameScreen::updateForces(const std::vector<Planet*> allObjects,
                              float dt)
{
    for (Planet* p1: allObjects) {
        if (p1->immovable) {
            continue;
        }

        sf::Vector2f totalForce(0.0f, 0.0f);

        for (Planet* p2: allObjects) {
            if (p1 == p2) {
                continue;
            }

            sf::Vector2f delta = p2->sprite.getPosition() - p1->sprite.getPosition();
            sf::Vector2f dir = normalized(delta);
            totalForce += dir * G * (p1->mass * p2->mass) / lengthSq(delta);
        }

        p1->acceleration += totalForce * dt;
        if (p1 == &planet && !sun.isBlackHole) {
            p1->acceleration = normalized(p1->acceleration) * std::sqrt(length(p1->acceleration));
        }

        if (sun.isBlackHole) {
            accelerationLimit = distance(p1->sprite.getPosition(), sun.sprite.getPosition()) / UPDATE_STEP_S;
        }
        p1->acceleration = normalized(p1->acceleration) * clamp(length(p1->acceleration), 0.0f, accelerationLimit);
        //printf("acceleration = %f %f\n", p1->acceleration.x, p1->acceleration.y);
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

void GameScreen::handleCollision(Planet* first,
                                 Planet* second,
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
    } else if (first == &sun) {
        sun.setMass(sun.mass + second->mass);
    } else {
        float newMass = first->mass + second->mass;
        first->acceleration = (first->acceleration * first->mass + second->acceleration * second->mass) / newMass;
        first->setMass(newMass);
    }

    // FIXME: remove `second` in a 'prettier' way
    second->sprite.setPosition(100000.0f, 100000.0f);

    if (!sun.isBlackHole) {
        explosions.push_back(Explosion(collisionPos));
        shakeScreen(first->mass / 4.0f / distance(collisionPos, planet.sprite.getPosition()));
    }
}

void GameScreen::checkCollisions(const std::vector<Planet*> allObjects)
{
    for (size_t i = 0; i < allObjects.size(); ++i) {
        for (size_t j = i + 1; j < allObjects.size(); ++j) {
            Planet* p1 = allObjects[i];
            Planet* p2 = allObjects[j];

            const sf::Vector2f& pos1 = p1->sprite.getPosition();
            const sf::Vector2f& pos2 = p2->sprite.getPosition();

            if (distance(pos1, pos2) < p1->radius + p2->radius) {
                sf::Vector2f collisionPos = pos1 + normalized(pos2 - pos1) * p1->radius;
                handleCollision(p1, p2, collisionPos);
            }
        }
    }
}

void GameScreen::spawnAsteroids(float dt)
{
    static float timeAccumulator = 0.0f;
    static constexpr float ASTEROID_SPAWN_DELAY_S = 1.0f;

    timeAccumulator += dt;

    while (timeAccumulator > ASTEROID_SPAWN_DELAY_S) {
        timeAccumulator -= ASTEROID_SPAWN_DELAY_S;

        float initialAngle = rand_float(0, 2 * M_PI);
        float initialDistance = 300.0f;
        sf::Vector2f initialPos =
                sf::Vector2f(std::cos(initialAngle) * initialDistance,
                             std::sin(initialAngle) * initialDistance);
        sf::Vector2f initialAcceleration = rand_vector()
                * rand_float(MIN_ASTEROID_INITIAL_ACC, MAX_ASTEROID_INITIAL_ACC);
        float mass = rand_float(MIN_ASTEROID_MASS, MAX_ASTEROID_MASS);

        asteroids.emplace_back(mass, initialPos, initialAcceleration,
                               sf::Color::Blue);
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
    std::vector<Planet*> allObjects { &sun, &planet };
    for (Planet& a: asteroids) {
        allObjects.push_back(&a);
    }

    checkCollisions(allObjects);
    updateForces(allObjects, dt);

    for (Planet* p: allObjects) {
        p->sprite.move(p->acceleration * dt);
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

