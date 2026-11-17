#include "popup_messages.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "utils.h"

PopupMessages::PopupMessages():
    textSize(DEFAULT_POPUP_TEXT_SIZE)
{
    if (!font.openFromFile("data/DejaVuSans.ttf")) {
        printf("cannot load data/DejaVuSans.ttf\n");
        abort();
    }
}

void PopupMessages::add(const std::string& msg,
                        const sf::Vector2f& pos,
                        const sf::Color& color,
                        float ttl_s)
{
    msgs.push_back({ sf::Text(font, msg), ttl_s });

    sf::Text& text = msgs.back().text;
    sf::FloatRect boundingRect = text.getLocalBounds();
    sf::Vector2f centerOffset(boundingRect.position.x + boundingRect.size.x / 2,
                              boundingRect.position.y + boundingRect.size.y / 2);

    text.setOrigin(centerOffset);
    text.setPosition(pos);
    text.setFillColor(color);
    text.setCharacterSize(textSize);
}

void PopupMessages::update(float dt)
{
    for (size_t i = 0; i < msgs.size();) {
        Message& msg = msgs[i];

        msg.ttl_s -= dt;
        if (msg.ttl_s <= 0.0f) {
            swapAndPop(msgs, i);
        } else {
            sf::Color color = msg.text.getFillColor();
            color.a = 255.0f * clamp(msg.ttl_s, 0.0f, 1.0f);
            msg.text.setFillColor(color);
            msg.text.move({ 0.0f, -POPUP_ASCEND_SPEED * dt });
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
