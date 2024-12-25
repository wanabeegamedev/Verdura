//
// Created by hous on 12/25/24.
//

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
//#define MINIAUDIO_IMPLEMENTATION
//#include "miniaudio.h"
/*#include <unordered_map>
#include <iostream>
#include <string>

struct SoundData {
    ma_decoder decoder{};
    ma_device device{};
    bool isPlaying = false;
};

class SoundManager {
public:
    std::unordered_map<std::string, SoundData> sounds;

    SoundManager() = default;
    ~SoundManager() {
        // Clean up all loaded sounds
        for (auto& [name, sound] : sounds) {
            ma_device_uninit(&sound.device);
            ma_decoder_uninit(&sound.decoder);
        }
    }

    void loadSound(const std::string& name, const std::string& path) {
        if (sounds.find(name) != sounds.end()) {
            std::cerr << "Sound \"" << name << "\" already loaded.\n";
            return;
        }

        SoundData sound;
        if (ma_decoder_init_file(path.c_str(), nullptr, &sound.decoder) != MA_SUCCESS) {
            std::cerr << "Failed to load sound: " << path << "\n";
            return;
        }

        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format   = sound.decoder.outputFormat;
        config.playback.channels = sound.decoder.outputChannels;
        config.sampleRate        = sound.decoder.outputSampleRate;
        config.dataCallback      = [](ma_device* device, void* output, const void* input, ma_uint32 frameCount) {
            ma_decoder* decoder = (ma_decoder*)device->pUserData;
            ma_decoder_read_pcm_frames(decoder, output, frameCount, nullptr);
            (void)input; // Unused
        };
        config.pUserData = &sound.decoder;

        if (ma_device_init(nullptr, &config, &sound.device) != MA_SUCCESS) {
            std::cerr << "Failed to initialize playback device for sound: " << path << "\n";
            ma_decoder_uninit(&sound.decoder);
            return;
        }

        sounds[name] = sound;
    }

    void playSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it == sounds.end()) {
            std::cerr << "Sound \"" << name << "\" not found.\n";
            return;
        }

        auto& sound = it->second;
        if (ma_device_start(&sound.device) != MA_SUCCESS) {
            std::cerr << "Failed to start playback for sound \"" << name << "\".\n";
        } else {
            sound.isPlaying = true;
        }
    }

    void stopSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it == sounds.end()) {
            std::cerr << "Sound \"" << name << "\" not found.\n";
            return;
        }

        auto& sound = it->second;
        ma_device_stop(&sound.device);
        sound.isPlaying = false;
    }
};*/
#include <unordered_map>
#include <string>
#include <iostream>

#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <iostream>

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
        Mix_HaltChannel(-1); // Stop all channels
    }

private:
    std::unordered_map<std::string, Mix_Chunk*> sounds; // Store loaded chunks
};


#endif // SOUNDMANAGER_H
