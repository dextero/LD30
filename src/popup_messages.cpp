#include "popup_messages.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "utils.h"

PopupMessages::PopupMessages():
    textSize(DEFAULT_POPUP_TEXT_SIZE)
{
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        abort();
    }
}

void PopupMessages::add(const std::string& msg,
                        const sf::Vector2f& pos,
                        const sf::Color& color,
                        float ttl_s)
{
    msgs.push_back({ sf::Text(msg, font), ttl_s });

    sf::Text& text = msgs.back().text;
    sf::FloatRect boundingRect = text.getLocalBounds();
    sf::Vector2f centerOffset(boundingRect.left + boundingRect.width / 2,
                              boundingRect.top + boundingRect.height / 2);

    text.setOrigin(centerOffset);
    text.setPosition(pos);
    text.setColor(color);
    text.setCharacterSize(textSize);
}

void PopupMessages::update(float dt)
{
    for (size_t i = 0; i < msgs.size();) {
        Message& msg = msgs[i];

        msg.ttl_s -= dt;
        if (msg.ttl_s <= 0.0f) {
            if (i != msgs.size() - 1) {
                msgs[i] = std::move(msgs.back());
            }
            msgs.pop_back();
        } else {
            sf::Color color = msg.text.getColor();
            color.a = 255.0f * clamp(msg.ttl_s, 0.0f, 1.0f);
            msg.text.setColor(color);
            msg.text.move(0.0f, -POPUP_ASCEND_SPEED * dt);
            ++i;
        }
    }
}

void PopupMessages::draw(sf::RenderTarget& rt,
                         sf::RenderStates states) const
{
    for (const Message& msg: msgs) {
        rt.draw(msg.text, states);
    }
}
