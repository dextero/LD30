#include "audio.h"

#include <cassert>

Audio::Audio():
    sounds(8)
{
    const std::string soundPaths[] {
        "data/explosion.wav",
        "data/powerup.wav",
        "data/attach.wav"
    };

    for (const std::string& path: soundPaths) {
        soundBuffers.push_back({});
        if (!soundBuffers.back().loadFromFile(path)) {
            abort();
        }
    }
}

void Audio::addSound(Type type)
{
    float earliestSecondsLeft = 10000.0f;
    sf::Sound* earliestSound = nullptr;

    for (sf::Sound& snd: sounds) {
        if (!snd.getBuffer()) {
            snd = sf::Sound(soundBuffers[(size_t)type]);
            snd.play();
            return;
        }

        float secondsLeft = snd.getBuffer()->getDuration().asSeconds() - snd.getPlayingOffset().asSeconds();
        if (secondsLeft < earliestSecondsLeft) {
            earliestSecondsLeft = secondsLeft;
            earliestSound = &snd;
        }
    }

    assert(earliestSound);
    *earliestSound = sf::Sound(soundBuffers[(size_t)type]);
    earliestSound->play();
}

void Audio::setMusic(const std::string& path)
{
    if (!music.openFromFile(path)) {
        abort();
    }

    music.setLoop(true);
    music.play();
}
