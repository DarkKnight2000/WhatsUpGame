#pragma once

#include "Assets.hpp"
#include "defines.hpp"
#include "raylib.h"
#include <iostream>

struct MenuScreen
{

    void draw(Assets& assets, SceneEnum& scene_id, bool& is_bg_playing)
    {
        float button_w = 7*TILE_SIZE;
        float button_x = (GetRenderWidth() - button_w) / 2;
        float button_h = 2*TILE_SIZE;
        float button_y = 500;

        assets.draw_title(200);

        bool is_focused = false;
        Rectangle button_rect = {button_x, button_y, button_w, button_h};
        if(assets.draw_ui_button(button_rect, "START", 44))
        {
            scene_id = SceneEnum::LEVEL_SELECT;
        }

        button_y += 3*TILE_SIZE;
        is_focused = false;
        button_rect = {button_x, button_y, button_w, button_h};
        if(assets.draw_ui_button(button_rect, "CREDITS", 44))
        {
            scene_id = SceneEnum::CREDITS;
        }

        assets.manage_bg_music(is_bg_playing);

    }
};


struct LevelSelectScreen
{
    void draw(Assets& assets, SceneEnum& scene_id, int& level_id, bool& is_bg_playing)
    {
        float level_space = 1*TILE_SIZE;
        float level_w = 5*TILE_SIZE;
        float row1_w = 4*level_w + 3*level_space;
        float row1_x = (GetRenderWidth() - row1_w) / 2;
        float row_y = 300;

        for (int i = 1; i <= 4; i++)
        {
            if (assets.draw_ui_button({row1_x, row_y, 5 * TILE_SIZE, 2 * TILE_SIZE},
                                        TextFormat("Level %d", i), 32))
            {
                scene_id = SceneEnum::LEVEL;
                level_id = i;
                std::cout << "clicked level button " << i << std::endl;
            }
            row1_x += level_space + level_w;
        }

        row1_w = 4*level_w + 3*level_space;
        row1_x = (GetRenderWidth() - row1_w) / 2;
        for (int i = 5; i <= 8; i++)
        {
            if (assets.draw_ui_button({row1_x, row_y + 5*TILE_SIZE, 5 * TILE_SIZE, 2 * TILE_SIZE},
                                        TextFormat("Level %d", i), 32))
            {
                scene_id = SceneEnum::LEVEL;
                level_id = i;
                std::cout << "clicked level button " << i << std::endl;
            }
            row1_x += level_space + level_w;
        }

        // back button
        assets.manage_back_button(scene_id, SceneEnum::HOME);

        assets.manage_bg_music(is_bg_playing);
    }
};


struct FinishScreen
{
    void draw(Assets& assets, SceneEnum& scene_id, bool& is_bg_playing)
    {
        float button_w = 7*TILE_SIZE;
        float button_x = (GetRenderWidth() - button_w) / 2;
        float button_h = 2*TILE_SIZE;
        float button_y = 500;

        assets.draw_text_center("Thank you for playing!!", 320, true, 120);
        assets.draw_text_center("This game was made as a part of 'The Very Serious Juniper Dev Game Jam'.", 540, false, 30);
        assets.draw_text_center("Thank you Juniper Dev for organizing the jam, it was a lot of fun!", 600, false, 30);
        assets.draw_text_center(
            "The art assets and SFX are taken from Kenny Platformer pack - (https://kenney.nl/assets/new-platformer-pack)",
            700, false, 22);
        assets.draw_text_center(
            "Some UI elements were taken from Kenny UI pack - Pixel Adventure (https://kenney.nl/assets/ui-pack-pixel-adventure)",
            750, false, 22);
        assets.draw_text_center(
            "The font was taken from 'No Continue Regular' (https://www.1001fonts.com/no-continue-font.html)",
            800, false, 22);
        assets.draw_text_center(
            "The song was made by me using 'Bosca Ceoil' (https://yurisizov.itch.io/boscaceoil-blue)",
            850, false, 22);
        

        // back button
        assets.manage_back_button(scene_id, SceneEnum::HOME);

        assets.manage_bg_music(is_bg_playing);
    }
};