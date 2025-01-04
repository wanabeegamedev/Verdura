//
// Created by hous on 12/25/24.
//

#ifndef GAMEUI_H
#define GAMEUI_H



#include <string>
#include <vector>

#include "imgui.h"
#include "../Character/Stats.h"
#include "../Character/WarriorClass.h"

#define UI_ELEMENT_IMG_WIDTH 48.0f;

class GameUI {
public:
    GameUI();

    ImTextureID texture_width ;
    ImVec2 texture_size;
    ImVec2 uv0 ;
    ImVec2 uv1 ;
    ImVec4 bg_col ;
    ImVec4 tint_col;

    unsigned int sword_texture_id{};
    unsigned int snow_texture_id{};
    unsigned int fire_texture_id{};
    unsigned int mana_texture_id{};
    unsigned int health_texture_id{};
    unsigned int loaded_texture_id{};

    float delayWindow{6.0f};
    float delayWindowAccumulator{};

    bool show_welcome = false;
    bool show_help = false;
    bool show_info_log = true;
    bool show_quit = false;
    bool show_about = false;
    bool show_inventory = false;
    bool show_info_once = false;
    bool show_reward_choice = false;
    int stateFlag{};
    int choice{};

    std::string info;
    std::vector<WarriorClass*> warriorClasses{};
    Stats* stats;

    void show_welcome_window() ;
    void show_info_log_window();
    void handleInputs(float);
    void update(float);

    void show_info_once_window();
    void show_stats();
    void toggle_info_once_window(const std::string &_info);


    void show_reward_window();
    void toggle_info_reward_window();
    void load_ui_elements();
};
#endif //GAMEUI_H
