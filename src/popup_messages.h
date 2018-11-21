#pragma once

#include <vector>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>

class PopupMessages:
    public sf::Drawable
{
public:
    static constexpr float DEFAULT_POPUP_TTL_S = 3.0f;
    static constexpr float POPUP_ASCEND_SPEED = 15.0f;
    static constexpr unsigned DEFAULT_POPUP_TEXT_SIZE = 20;

    struct Message
    {
        sf::Text text;
        float ttl_s;
    };

    PopupMessages();
    virtual ~PopupMessages() {}
    
    void add(const std::string& msg,
             const sf::Vector2f& pos,
             const sf::Color& color,
             float ttl_s = DEFAULT_POPUP_TTL_S);

    void update(float dt);
    void clear() { msgs.clear(); }

protected:
    virtual void draw(sf::RenderTarget& rt,
                      sf::RenderStates states) const;

private:
    std::vector<Message> msgs;
    sf::Font font;
    unsigned textSize;
};

