#include <memory>
#include <random>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

constexpr float PLANET_MASS = 1000.0f;
constexpr float PLANET_SPEED = 10.0f;
constexpr float MIN_ASTEROID_MASS = 100.0f;
constexpr float MAX_ASTEROID_MASS = 200.0f;
constexpr float MIN_ASTEROID_INITIAL_ACC = 10.0f;
constexpr float MAX_ASTEROID_INITIAL_ACC = 20.0f;
constexpr float MAX_EXPLOSION_START_TIME = 1.0f;
constexpr float MAX_EXPLOSION_TIME = 3.0f;
constexpr float MIN_EXPLOSION_RADIUS = 10.0f;
constexpr float MAX_EXPLOSION_RADIUS = 20.0f;
constexpr float MAX_EXPLOSION_OFFSET = 10.0f;
constexpr size_t EXPLOSION_CIRCLES = 20;

inline float rand_float(float min,
                        float max)
{
    static std::random_device random_device;
    static std::mt19937 random_gen;

    return std::uniform_real_distribution<float>(min, max)(random_gen);
}

constexpr float G = 6.67384;

constexpr inline float lengthSq(const sf::Vector2f& v)
{
    return v.x * v.x + v.y * v.y;
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

struct Planet
{
    sf::Vector2f acceleration;
    float mass;
    float radius;
    sf::CircleShape sprite;
    bool immovable;

    Planet() {}
    Planet(float mass,
           const sf::Vector2f& initialPos,
           const sf::Vector2f& initialAcceleration = { 0.0f, 0.0f },
           const sf::Color& color = sf::Color::Red):
        acceleration(initialAcceleration),
        mass(mass),
        radius(std::sqrt(mass / M_PI)),
        sprite(sf::CircleShape(radius)),
        immovable(false)
    {
        sprite.setPosition(initialPos);
        sprite.setOrigin(radius, radius);
        sprite.setFillColor(color);
    }

    void setMass(float newMass)
    {
        mass = newMass;
        radius = std::sqrt(mass / M_PI);
        sprite.setRadius(radius);
        sprite.setOrigin(radius, radius);
    }
};

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
              size_t numCircles = EXPLOSION_CIRCLES):
        pos(pos)
    {
        //circles.push_back({ 0.0f, 3.0f, 10.0f, { 0.0f, 0.0f }, sf::Color::Yellow });
        circles.reserve(numCircles);
        for (size_t i = 0; i < numCircles; ++i) {
            float tts = rand_float(0.0f, MAX_EXPLOSION_START_TIME);
            float tth = rand_float(tts, MAX_EXPLOSION_TIME);
            float maxRadius = rand_float(MIN_EXPLOSION_RADIUS, MAX_EXPLOSION_RADIUS);
            sf::Vector2f offset(rand_float(-MAX_EXPLOSION_OFFSET, MAX_EXPLOSION_OFFSET),
                                rand_float(-MAX_EXPLOSION_OFFSET, MAX_EXPLOSION_OFFSET));
            sf::Color color(255, (sf::Uint8)rand_float(0.0, 255.0), 0);

            circles.push_back({ tts, tth, maxRadius, offset, color });
        }
    }

    void update(float dt)
    {
        for (Circle& c: circles) {
            c.timeToShow -= dt;
            c.timeToHide -= dt;
        }
    }

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const
    {
        sf::CircleShape shape;
        shape.setPosition(pos);

        for (const Circle& c: circles) {
            if (c.timeToShow > 0.0f || c.timeToHide <= 0.0f) {
                continue;
            }

            float progress = (MAX_EXPLOSION_TIME - c.timeToHide) / MAX_EXPLOSION_TIME;
            float radius = c.maxRadius * progress;
            sf::Uint8 alpha = 255 - (sf::Uint8)(255.0f * progress * progress * progress * progress);

            shape.setRadius(radius);
            shape.setOrigin(c.offset.x + radius, c.offset.y + radius);
            shape.setFillColor(sf::Color(c.color.r, c.color.g, c.color.b, alpha));

            rt.draw(shape, states);
        }
    }
};

class Game
{
public:
    void run()
    {
        sf::VideoMode videoMode(1024, 768);
        wnd.reset(new sf::RenderWindow(videoMode, "LD30"));

        viewRect = sf::FloatRect(-(float)videoMode.width / 2,
                                 -(float)videoMode.height / 2,
                                 videoMode.width, videoMode.height);

        init();

        sf::Clock clock;

        while (wnd->isOpen()) {
            handleInput();
            update(clock.restart().asSeconds());
            draw();
        }
    }

private:
    void init()
    {
        printf("init\n");
        planet = Planet(PLANET_MASS, sf::Vector2f(0.f, 0.f));
        planet.immovable = true;
    }

    void handleInput()
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

    void onKeyPressed(const sf::Event& evt)
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

    void onKeyReleased(const sf::Event& evt)
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

    void onMouseMoved(const sf::Event& evt)
    {
        //printf("onMouseMoved %d %d\n", evt.mouseMove.x, evt.mouseMove.y);
        planet.sprite.setPosition((float)evt.mouseMove.x + viewRect.left,
                                  (float)evt.mouseMove.y + viewRect.top);
    }

    void updateForces(const std::vector<Planet*> allObjects,
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
            //printf("acceleration = %f %f\n", p1->acceleration.x, p1->acceleration.y);
        }
    }

    void shakeScreen(float factor)
    {
        screenShakeFactor = clamp(factor, 0.0f, 100.0f);
    }

    void handleCollision(Planet* first,
                         Planet* second,
                         const sf::Vector2f& collisionPos)
    {
        if (first->immovable && second->immovable) {
            printf("uh oh. TODO\n");
            return;
        }

        if (second->immovable) {
            std::swap(first, second);
        }

        if (first->immovable) {
            shakeScreen(std::exp(second->mass / 100.0f));
        } else {
            float newMass = first->mass + second->mass;
            first->acceleration = (first->acceleration * first->mass + second->acceleration * second->mass) / newMass;
            first->setMass(newMass);
        }

        // FIXME: remove `second` in a 'prettier' way
        second->sprite.setPosition(100000.0f, 100000.0f);

        explosions.push_back(Explosion(collisionPos));
    }

    void checkCollisions(const std::vector<Planet*> allObjects)
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

    void spawnAsteroids(float dt)
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
            sf::Vector2f initialAcceleration =
                    normalized(-initialPos)
                    * rand_float(MIN_ASTEROID_INITIAL_ACC, MAX_ASTEROID_INITIAL_ACC);
            float mass = rand_float(MIN_ASTEROID_MASS, MAX_ASTEROID_MASS);

            asteroids.emplace_back(mass, initialPos, initialAcceleration,
                                   sf::Color::Blue);
        }
    }

    void removeOutOfBounds()
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

    void doUpdateStep(float dt)
    {
        std::vector<Planet*> allObjects { &planet };
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

    void updatePlanet()
    {
    }

    void updateShake(float dt)
    {
        static float upTo2PiAccumulator = 0.0f;
        upTo2PiAccumulator = std::fmod(upTo2PiAccumulator + dt, (float)M_PI * 2.0f);
        shakeOffset = {
            std::sin(upTo2PiAccumulator * 40.0f) * screenShakeFactor,
            std::cos(upTo2PiAccumulator * 43.0f) * screenShakeFactor
        };

        screenShakeFactor *= 0.97;
        if (screenShakeFactor < 1.0f) {
            screenShakeFactor = 0.0f;
        }
    }

    void update(float dt)
    {
        static float timeAccumulator = 0.0f;
        static constexpr float UPDATE_STEP_S = 1.0f / 60.0f;

        timeAccumulator += dt;
        while (timeAccumulator > UPDATE_STEP_S) {
            timeAccumulator -= UPDATE_STEP_S;
            doUpdateStep(UPDATE_STEP_S);

            planet.sprite.move(planetMoveDir);
        }

        updateShake(dt);
    }

    void draw()
    {
        wnd->clear(sf::Color::Black);
        wnd->setView(sf::View(moveRect(viewRect, shakeOffset)));

        wnd->draw(planet.sprite, sf::RenderStates::Default);
        for (const Planet& a: asteroids) {
            //printf("asteroid @ %f %f\n", a.sprite.getPosition().x, a.sprite.getPosition().y);
            wnd->draw(a.sprite, sf::RenderStates::Default);
        }

        for (const Explosion& e: explosions) {
            wnd->draw(e);
        }

        wnd->display();
    }

    std::unique_ptr<sf::RenderWindow> wnd;
    sf::FloatRect viewRect;

    float screenShakeFactor;
    sf::Vector2f shakeOffset;

    sf::Vector2f planetMoveDir;
    Planet planet;
    std::vector<Planet> asteroids;
    std::vector<Explosion> explosions;
};

int main(int /*argc*/,
         char** /*argv*/)
{
    Game game;
    game.run();

    return 0;
}

