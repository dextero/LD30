#pragma once

#include <vector>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

class Audio
{
public:
    enum Type
    {
        Explosion,
        Powerup,
        Attach
    };

    Audio();

    void addSound(Type type);
    void setMusic(const std::string& music);

private:
    std::vector<sf::SoundBuffer> soundBuffers;
    std::vector<sf::Sound> sounds;
    sf::Music music;
};
