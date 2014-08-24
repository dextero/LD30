#pragma once

#include <string>

constexpr float EPSILON = 0.001f;
constexpr float G = 6.67384e-2;

constexpr unsigned WINDOW_WIDTH = 1024;
constexpr unsigned WINDOW_HEGIHT = 768;
const std::string WINDOW_TITLE = "LD30";

constexpr float UPDATE_STEP_S = 1.0f / 60.0f;
constexpr float GAME_OVER_DELAY = 3.0f;

constexpr float PLANET_MASS = 1000.0f;
constexpr float PLANET_SPEED = 10.0f;

constexpr float ASTEROID_SPAWN_DELAY_S = 0.1f;
constexpr float ASTEROID_INITIAL_DISTANCE = 700.0f;
constexpr float ASTEROID_INITIAL_ANGLE_VARIANCE = 0.5f;
constexpr float ASTEROID_MAX_DISTANCE = ASTEROID_INITIAL_DISTANCE + 500.0f;
constexpr float MIN_ASTEROID_MASS = 10.0f;
constexpr float MAX_ASTEROID_MASS = 50.0f;
constexpr float MIN_ASTEROID_INITIAL_VELOCITY = 50.0f;
constexpr float MAX_ASTEROID_INITIAL_VELOCITY = 100.0f;

constexpr float MAX_EXPLOSION_START_TIME = 1.0f;
constexpr float MAX_EXPLOSION_TIME = 3.0f;
constexpr float MIN_EXPLOSION_RADIUS = 10.0f;
constexpr float MAX_EXPLOSION_RADIUS = 20.0f;
constexpr float MAX_EXPLOSION_OFFSET = 10.0f;
constexpr size_t EXPLOSION_CIRCLES = 20;

constexpr float SUN_INITIAL_MASS = 1500.0f;
constexpr float SUN_VAPORIZE_SPEED = 50.0f;
constexpr float SUN_RED_GIANT_THRESHOLD = 100.0f;
constexpr float SUN_BLACK_HOLE_THRESHOLD = 5000.0f;
constexpr float BLACK_HOLE_RADIUS = 100.0f;
constexpr float RED_GIANT_EXPAND_SPEED = 100.0f;

