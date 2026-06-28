#pragma once
#include "Assets.hpp"
#include "Levels.hpp"
#include "Scenes.hpp"
#include "defines.hpp"
#include "raylib.h"

struct SceneManager
{
    SceneEnum scene_id = SceneEnum::HOME;
    int level_id = -1;
    Assets assets;
    LevelSimulate level_sim;

    MenuScreen menu;
    LevelSelectScreen level_select_screen;
    FinishScreen finish_screen;

    bool is_bg_playing = true;

    void load()
    {
        assets.load();
        PlayMusicStream(assets.bg_music);

        level_sim.init();
    }

    void unload()
    {
        level_sim.deinit();
        assets.unload();
    }

    void draw()
    {
        assets.draw_bg_empty();
        UpdateMusicStream(assets.bg_music);
        bool last_bg_setting = is_bg_playing;
        
        switch (scene_id)
        {
            case HOME:
            {
                menu.draw(assets, scene_id, is_bg_playing);
                break;
            }
            case LEVEL_SELECT:
            {
                level_id = -1;
                level_select_screen.draw(assets, scene_id, level_id, is_bg_playing);
                if(-1 != level_id)
                {
                    // load level
                    level_sim.load_level(level_id);
                    scene_id = SceneEnum::LEVEL;
                }
                break;
            }
            case LEVEL:
            {
                level_sim.draw(assets, scene_id, is_bg_playing);
                break;
            }
            case CREDITS:
            {
                finish_screen.draw(assets, scene_id, is_bg_playing);
                break;
            }
        }

        if(last_bg_setting != is_bg_playing)
        {
            if(is_bg_playing) ResumeMusicStream(assets.bg_music);
            else PauseMusicStream(assets.bg_music);
        }
    }
};