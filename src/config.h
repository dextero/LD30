#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>

constexpr float EPSILON = 0.001f;
constexpr float G = 6.67384e0;

constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEGIHT = 900;
const std::string WINDOW_TITLE = "LD30";

constexpr float UPDATE_STEP_S = 1.0f / 60.0f;
constexpr float GAME_OVER_DELAY = 3.0f;

constexpr float CROSSHAIR_ANGULAR_SPEED = 2.0f * M_PI;
constexpr float CROSSHAIR_DISTANCE = 100.0f;
constexpr float MAX_SELECT_DISTANCE = 50.0f;
constexpr float VELOCITY_LINE_LENGTH = 100.0f;

constexpr float PLANET_MASS = 1000.0f;
constexpr float PLANET_SPEED = 200.0f;
constexpr float ATTRACT_MASS = PLANET_MASS * 10000.0f;

constexpr float POWERUP_RADIUS = 10.0f;
constexpr float POWERUP_SPAWN_DELAY_S = 0.1f;
constexpr float POWERUP_SPEED = 300.0f;

constexpr float ASTEROID_SPAWN_DELAY_S = 0.5f;
constexpr float ASTEROID_INITIAL_DISTANCE = 700.0f;
constexpr float ASTEROID_INITIAL_ANGLE_VARIANCE = 1.0f;
constexpr float ASTEROID_MAX_DISTANCE = ASTEROID_INITIAL_DISTANCE + 500.0f;
constexpr float MIN_ASTEROID_MASS = 20.0f;
constexpr float MAX_ASTEROID_MASS = 70.0f;
constexpr float MIN_ASTEROID_INITIAL_VELOCITY = 50.0f;
constexpr float MAX_ASTEROID_INITIAL_VELOCITY = 100.0f;

const sf::Color ASTEROID_COLOR = sf::Color(150.0f, 150.0f, 150.0f);
const sf::Color SELECTED_ASTEROID_COLOR = sf::Color(100, 200, 100);

constexpr float MAX_EXPLOSION_START_TIME = 1.0f;
constexpr float MAX_EXPLOSION_TIME = 3.0f;
constexpr float MIN_EXPLOSION_RADIUS = 10.0f;
constexpr float MAX_EXPLOSION_RADIUS = 20.0f;
constexpr float MAX_EXPLOSION_OFFSET = 10.0f;
constexpr size_t EXPLOSION_CIRCLES = 20;

constexpr float SUN_INITIAL_MASS = 2500.0f;
constexpr float SUN_VAPORIZE_SPEED = 50.0f;
constexpr float SUN_RED_GIANT_THRESHOLD = 100.0f;
constexpr float BLACK_HOLE_MASS = 1.0e+8f;
constexpr float BLACK_HOLE_RADIUS = 100.0f;
constexpr float RED_GIANT_EXPAND_SPEED = 100.0f;

