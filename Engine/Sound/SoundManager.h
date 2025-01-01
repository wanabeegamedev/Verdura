//
// Created by hous on 12/25/24.
//

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <unordered_map>
#include <string>
#include <iostream>

#include <SDL2/SDL_mixer.h>

class SoundManager {
public:
    SoundManager() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
        }
    }

    ~SoundManager() {
        for (auto& [name, chunk] : sounds) {
            Mix_FreeChunk(chunk);
        }
        Mix_CloseAudio();
    }

    void loadSound(const std::string& name, const std::string& path) {
        if (sounds.find(name) != sounds.end()) {
            std::cerr << "Sound \"" << name << "\" already loaded.\n";
            return;
        }

        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
            return;
        }

        sounds[name] = chunk;
    }
    void playSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it == sounds.end()) {
            std::cerr << "Sound \"" << name << "\" not found.\n";
            return;
        }

        Mix_PlayChannel(-1, it->second, 0); // Play once on the first free channel
    }
    void stopAll() {
        Mix_HaltChannel(-1);
    }
    //La seconde Music jouera sans arret
    void playSequential(const std::string& firstSound,
                        const std::string& secondSound)
    {
        auto it1 = sounds.find(firstSound);
        auto it2 = sounds.find(secondSound);

        if (it1 == sounds.end() || it2 == sounds.end()) {
            std::cerr << "One or both sounds not found.\n";
            return;
        }
        static Mix_Chunk* nextChunk = nullptr;
        nextChunk = it2->second;
        Mix_ChannelFinished([](int channel) {
            if (nextChunk) {
                Mix_PlayChannel(-1, nextChunk, -1);
                nextChunk = nullptr;
            }
        });
        Mix_PlayChannel(-1, it1->second, 0);
    }
private:
    std::unordered_map<std::string, Mix_Chunk*> sounds; // Store loaded chunks
};


#endif // SOUNDMANAGER_H
