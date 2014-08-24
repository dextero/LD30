#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include "screen.h"
#include "menu_screen.h"
#include "game_screen.h"
#include "game_over_screen.h"
#include "audio.h"

class Game:
    public std::enable_shared_from_this<Game>
{
public:
    enum State
    {
        Menu,
        Running,
        Over
    };

    std::shared_ptr<sf::RenderWindow> wnd;
    std::shared_ptr<Screen> screen;
    sf::Font font;
    Audio audio;

    Game();

    void setState(State state);
    void run();
};

