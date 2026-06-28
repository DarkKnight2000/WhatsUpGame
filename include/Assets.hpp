#pragma once
#include "defines.hpp"
#include "raylib.h"
#include "Debug.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define TILE_DRAW_Y_OFF 0
#ifdef PLATFORM_DESKTOP
    #undef TILE_DRAW_Y_OFF
    #define TILE_DRAW_Y_OFF 1
#endif

struct Assets
{
    Texture2D tiles, bg, player, ui_thick, ui_thin;
    float anim_time = 0;
    Font font_normal;

    // audio
    Music bg_music;
    Sound btn_click, pl_jump, box_hit, level_win, dead;

    void load()
    {
        tiles = LoadTexture(ASSETS_PATH"spritesheet-tiles-default.png");
        bg = LoadTexture(ASSETS_PATH"spritesheet-backgrounds-default.png");
        player = LoadTexture(ASSETS_PATH"spritesheet-characters-default.png");
        font_normal = LoadFont("res/no-continue/gomarice_no_continue.ttf");
        ui_thick = LoadTexture("res/ui-pack/Tilesheets/Large tiles/Thick outline/tilemap_packed.png");
        ui_thin = LoadTexture("res/ui-pack/Tilesheets/Large tiles/Thin outline/tilemap_packed.png");
        // font_title = LoadFont("./res/achtung-bravo.regular.ttf");

        bg_music = LoadMusicStream("res/audio/slow_song.wav");
        SetMusicVolume(bg_music, 0.5);

        dead = LoadSound("res/platformer/Sounds/sfx_disappear.ogg");
        btn_click = LoadSound("res/platformer/Sounds/sfx_magic.ogg");
        level_win = LoadSound("res/platformer/Sounds/sfx_coin.ogg");
        SetSoundPitch(level_win, 0.5);
        pl_jump = LoadSound("res/platformer/Sounds/sfx_jump.ogg");
        box_hit = LoadSound("res/platformer/Sounds/sfx_bump.ogg");
    }

    void unload()
    {
        UnloadTexture(tiles);
        UnloadTexture(bg);
        UnloadTexture(player);
        UnloadFont(font_normal);
    }

    void update()
    {
        anim_time += GetFrameTime();
        if(anim_time >= 60) anim_time -= 60;
    }

    void draw_bg_empty()
    {
        int num_repeat = GetRenderWidth() / 256 + 1;

        for (int i=0; i<num_repeat; i++)
        {
            // blue sky
            DrawTexturePro(bg,
                {257, 771, 256, 256}, 
                {(float)(i*256), 0, 256, 256}, 
                {0, 0}, 0, WHITE);
            DrawTexturePro(bg,
                {0, 0, 256, 256}, 
                {(float)(i*256), 256, 256, 256}, 
                {0, 0}, 0, WHITE);
            DrawTexturePro(bg,
                {257, 771, 256, 256}, 
                {(float)(i*256), 3*256, 256, 256}, 
                {0, 0}, 0, WHITE);
        }

        num_repeat = GetRenderWidth() / (256*4) + 1;

        for (int i=0; i<num_repeat; i++)
        {
            DrawTexturePro(bg,
                    {257, 257, 256, 256}, 
                    {(float)i*4*256, 256*2, 256, 256}, 
                    {0, 0}, 0, WHITE);
            DrawTexturePro(bg,
                    {514, 257, 256, 256}, 
                    {(float)i*4*256+256, 256*2, 256, 256}, 
                    {0, 0}, 0, WHITE);
            DrawTexturePro(bg,
                    {771, 257, 256, 256}, 
                    {(float)i*4*256+2*256, 256*2, 256, 256}, 
                    {0, 0}, 0, WHITE);
            DrawTexturePro(bg,
                    {0, 514, 256, 256}, 
                    {(float)i*4*256+3*256, 256*2, 256, 256}, 
                    {0, 0}, 0, WHITE);
        }
    }

    void draw_tile(int row, int col, float pos_x, float pos_y, float rotation = 0, float tex_off_x = 0, float tex_off_y = TILE_DRAW_Y_OFF)
    {
        DrawTexturePro(tiles,
            {(float)col*65+tex_off_x, (float)row*65+tex_off_y, TILE_SIZE, TILE_SIZE},
            {pos_x, pos_y, TILE_SIZE, TILE_SIZE},
            {0, 0},
            rotation,
            WHITE);
    }

    void draw_level_bg(float x_offset, float y_offset, float width_tiles, float height_tiles)
    {
        // for (float i=0; i<width_tiles; i++) {
        //     for (float j=0; j<height_tiles; j++) {
        //         DrawTexturePro(bg,
        //         {0, 772, 256, 256}, 
        //         {x_offset+i*TILE_SIZE, y_offset+j*TILE_SIZE, TILE_SIZE, TILE_SIZE}, 
        //         {0, 0}, 0, WHITE);
        //     }
        // }
        // DrawRectangleRec(
        //     {x_offset, y_offset, width_tiles*TILE_SIZE, height_tiles*TILE_SIZE},
        //     {239, 207, 140, 255});
        DrawTexturePro(bg,
                {0, 772, 256, 256}, 
                {x_offset, y_offset, width_tiles*TILE_SIZE, height_tiles*TILE_SIZE}, 
                {0, 0}, 0, WHITE);
    }

    void draw_wall(Rectangle& wall_rect)
    {
        for (float i = wall_rect.x ; i < wall_rect.x + wall_rect.width; i += TILE_SIZE)
        {
            // std::cout << "i " << i << ", " << wall_rect.x + wall_rect.width << std::endl;
            for (float j = wall_rect.y ; j < wall_rect.y + wall_rect.height; j += TILE_SIZE)
            {
                // std::cout << "j " << j << ", " << wall_rect.y + wall_rect.height << std::endl;
                draw_tile(1, 9, i, j);
            }
        }
        // std::cout << "-----\n";
    }

    void draw_spike_block(Rectangle& spike_rect)
    {
        for (float i = spike_rect.x ; i < spike_rect.x + spike_rect.width; i += TILE_SIZE)
        {
            // std::cout << "i " << i << ", " << wall_rect.x + wall_rect.width << std::endl;
            for (float j = spike_rect.y ; j < spike_rect.y + spike_rect.height; j += TILE_SIZE)
            {
                // std::cout << "j " << j << ", " << wall_rect.y + wall_rect.height << std::endl;
                draw_tile(0, 11, i, j);
            }
        }
        // std::cout << "-----\n";
    }

    void draw_door(Rectangle& door_rect, LEVEL_UP door_up, bool is_open)
    {
        float top_x = door_rect.x, top_y = door_rect.y;
        float bot_x = door_rect.x, bot_y = door_rect.y + TILE_SIZE;
        float rot = 0;
        if(door_up == RIGHT)
        {
            rot = 90;
            top_x += 2*TILE_SIZE-2;
            bot_x += TILE_SIZE-1;
            bot_y -= TILE_SIZE;
        }
        else if(door_up == DOWN)
        {
            rot = 180;
            top_x += TILE_SIZE-1;
            top_y += 2*TILE_SIZE-2;
            bot_x += TILE_SIZE-1;
        }
        else if(door_up == LEFT)
        {
            rot = 270;
            top_y += TILE_SIZE;
            bot_x += TILE_SIZE-1;
        }


        if(!is_open)
        {
            draw_tile(2, 6, top_x, top_y, rot);
            draw_tile(2, 5, bot_x, bot_y, rot);
        }
        else
        {
            draw_tile(2, 8, top_x, top_y, rot);
            draw_tile(2, 7, bot_x, bot_y, rot);
        }
    }

    void draw_player(float pos_x, float pos_y, float vel_x, float vel_y)
    {
        // idle 0, walk a 1, walk b 2, jump 3, hit 4
        int anim = 0;
        if(abs(vel_y) > 0.0f) anim = 3;
        else if (abs(vel_x) >= 0.1f)
        {
            if(int(anim_time*3) % 2 == 0) anim = 1;
            else anim = 2; 
        };
        float ax = 0, ay = 258;
        if(anim == 1) {ax = 258;}
        else if(anim == 2) {ax = 387;}
        else if(anim == 3) {ax = 129;}
        DrawTexturePro(player,
            {ax+128*(2.8f/16), ay+128*(3.4f/16), 128*(10.4f/16) * (vel_x < -0.1f ? -1 : 1), 128*(12.2f/16)},
            {pos_x, pos_y, PL_SIZE*(10.4f/16), PL_SIZE*(12.2f/16)},
            {0, 0},
            0,
            WHITE);
        // DrawRectangleLinesEx({pos_x, pos_y, PL_SIZE*(9.0f/16), PL_SIZE*(12.0f/16)}, 1, BLACK);
        // Debug::get().draw(
        //     TextFormat(
        //         "source rect %f, %f, %f, %f", ax+(128*3.5f/16), ay+128*(4.0f/16), 128*(9.0f/16), 128*(12.0f/16)));
        // Debug::get().draw(
        //     TextFormat(
        //         "dest rect %f, %f, %f, %f", pos_x, pos_y, PL_SIZE*(9.0f/16), PL_SIZE*(12.0f/16)));
    }

    void draw_wall_spikes(Rectangle box, bool is_vertical, LEVEL_UP block_up)
    {
        float top_x = box.x, top_y = box.y;
        float rot = 0;
        if(block_up == RIGHT)
        {
            rot = 90;
            top_x += TILE_SIZE-1;
        }
        else if(block_up == DOWN)
        {
            rot = 180;
            top_x += TILE_SIZE-1;
            top_y += TILE_SIZE-1;
        }
        else if(block_up == LEFT)
        {
            rot = 270;
            top_y += TILE_SIZE;
            top_x -= TILE_SIZE*0.5f;
        }
        else if(block_up == UP)
        {
            top_y -= TILE_SIZE*0.5f;
        }

        // DrawRectangleLinesEx((box), 5, RED);
        draw_tile(7, 0, top_x, top_y, rot);
    }

    void draw_torch(Rectangle torch_rect, LEVEL_UP torch_up)
    {
        float top_x = torch_rect.x, top_y = torch_rect.y;
        float rot = 0, tex_off_y = 0;
        int torch_tile = 0;
        if(torch_up == RIGHT)
        {
            rot = 90;
            top_x += TILE_SIZE-1;
            top_y -= 15.2 * TILE_SIZE / 64;
        }
        else if(torch_up == DOWN)
        {
            rot = 180;
            top_x += TILE_SIZE - 1 - 15.2 * TILE_SIZE / 64;
            top_y += TILE_SIZE;
            tex_off_y = -1;
        }
        else if(torch_up == LEFT)
        {
            rot = 270;
            top_y += TILE_SIZE - 16 * TILE_SIZE / 64;
        }
        else if(torch_up == UP)
        {
            top_x -= 16 * TILE_SIZE / 64;
            torch_tile = 1 + (int(anim_time*2) % 2);
        }

        // DrawRectangleLinesEx(torch_rect, 5, torch_up ? YELLOW : ORANGE);
        draw_tile(17, torch_tile, top_x, top_y, rot, 0, tex_off_y);
    }

    void draw_slide_plat(Rectangle& wall_rect, bool is_vertical)
    {
        if(!is_vertical)
        {
            for (float i = wall_rect.x ; i < wall_rect.x + wall_rect.width; i += TILE_SIZE)
            {
                draw_tile(15, 8, i, wall_rect.y);
            }
        }
        else
        {
            for (float i = wall_rect.y ; i < wall_rect.y + wall_rect.height; i += TILE_SIZE)
            {
                draw_tile(15, 8, wall_rect.x, i);
            }
        }
        // std::cout << "-----\n";
    }

    size_t split(const std::string &text, std::vector<std::string> &strs, char ch)
    {
        size_t pos = text.find( ch );
        size_t initialPos = 0;
        strs.clear();

        // Decompose statement
        while( pos != std::string::npos ) {
            strs.push_back( text.substr( initialPos, pos - initialPos ) );
            initialPos = pos + 1;

            pos = text.find( ch, initialPos );
        }

        // Add the last one
        strs.push_back( text.substr( initialPos, std::min( pos, text.size() ) - initialPos + 1 ) );

        return strs.size();
    }

    float draw_text(const std::string& text, float x, float y, float max_width, float text_size, Color color = BLACK)
    {
        // DrawRectangleLines(x, y, max_width, text_size*3, DARKGRAY);
        vector<string> parts;
        split(text, parts, ' ');
        // for (auto x : parts) {
        //     std::cout << "'" <<  x << "'" << std::endl;
        // }

        float wx = x, wy = y;
        for (auto& txtpart : parts)
        {
            float partlen = MeasureTextEx(font_normal, txtpart.c_str(), text_size, 1).x;
            if(wx + partlen > x + max_width)
            {
                wx = x;
                wy += text_size + 5;
            }
            DrawTextEx(font_normal, txtpart.c_str(), {wx, wy}, text_size, 1, color);
            wx += partlen + text_size;
        }

        return wy - y;
    }

    void nine_piece_button(Rectangle button_rect, bool is_focused)
    {
        Texture2D ui_tex;
        ui_tex = !is_focused ? ui_thin : ui_thick;
        float tex_x = !is_focused ? 0 : 160;
        float tex_y = !is_focused ? 0 : 32;

        // corners
        DrawTexturePro(
            ui_tex, {tex_x + 0, tex_y + 0, 32 / 3.0f, 32 / 3.0f},
            {button_rect.x, button_rect.y, UI_ASSET_SCALE, UI_ASSET_SCALE},
            {0, 0}, 0, WHITE);
        DrawTexturePro(ui_tex, {tex_x + 0, tex_y + 64 / 3.0f, 32 / 3.0f, 32 / 3.0f},
                       {button_rect.x,
                        button_rect.y + button_rect.height - UI_ASSET_SCALE,
                        UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);
        DrawTexturePro(ui_tex, {tex_x + 64 / 3.0f, tex_y + 0, 32 / 3.0f, 32 / 3.0f},
                       {button_rect.x + button_rect.width - UI_ASSET_SCALE,
                        button_rect.y, UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);
        DrawTexturePro(ui_tex, {tex_x + 64 / 3.0f, tex_y + 64 / 3.0f, 32 / 3.0f, 32 / 3.0f},
                       {button_rect.x + button_rect.width - UI_ASSET_SCALE,
                        button_rect.y + button_rect.height - UI_ASSET_SCALE,
                        UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);

        // body
        DrawTexturePro(ui_tex, {32 / 3.0f, 32 / 3.0f, 32 / 3.0f, 32 / 3.0f},
                       {button_rect.x + UI_ASSET_SCALE,
                        button_rect.y + UI_ASSET_SCALE,
                        button_rect.width-2*UI_ASSET_SCALE, button_rect.height-2*UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);

        for (float i = button_rect.x + UI_ASSET_SCALE; i < button_rect.x + button_rect.width-UI_ASSET_SCALE; i += UI_ASSET_SCALE)
        {
            DrawTexturePro(ui_tex, {tex_x + 32 / 3.0f, tex_y + 0, 32 / 3.0f, 32 / 3.0f},
                       {i,
                        button_rect.y, UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);
            DrawTexturePro(ui_tex, {tex_x + 32 / 3.0f, tex_y + 64 / 3.0f, 32 / 3.0f, 32 / 3.0f},
                       {i,
                        button_rect.y+button_rect.height-UI_ASSET_SCALE, UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);
        }

        for (float i = button_rect.y + UI_ASSET_SCALE; i < button_rect.y + button_rect.height-UI_ASSET_SCALE; i += UI_ASSET_SCALE)
        {
            DrawTexturePro(ui_tex, {tex_x + 0, tex_y + 32 / 3.0f, 32 / 3.0f, 32 / 3.0f},
                       {button_rect.x,
                        i, UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);
            DrawTexturePro(ui_tex, {tex_x + 64 / 3.0f, tex_y + 32 / 3.0f, 32 / 3.0f, 32 / 3.0f},
                       {button_rect.x+button_rect.width-UI_ASSET_SCALE,
                        i, UI_ASSET_SCALE, UI_ASSET_SCALE},
                       {0, 0}, 0, WHITE);
        }
    }

    void draw_title(float y)
    {
        string title = "What's Up?";
        int font_size = 120;
        auto textlen = MeasureTextEx(font_normal, title.c_str(), font_size, 15);
        float x = (GetRenderWidth() - textlen.x) / 2;

        DrawTextEx(font_normal, title.c_str(), {x+4, y+4}, font_size, 15, BLACK);
        DrawTextEx(font_normal, title.c_str(), {x, y}, font_size, 15, RED);
    }

    bool draw_ui_button(Rectangle button_rect, string button_text, float font_size)
    {
        bool is_focused = false;
        if(CheckCollisionPointRec(GetMousePosition(), button_rect))
        {
            is_focused = true;
        }
        nine_piece_button(button_rect, is_focused);
        auto text_len = MeasureTextEx(font_normal, button_text.c_str(), font_size, 4);
        float textx = button_rect.x + (button_rect.width - text_len.x) / 2.0f;
        float texty = button_rect.y + (button_rect.height - text_len.y) / 2.0f;
        DrawTextEx(font_normal, button_text.c_str(), {textx, texty}, font_size, 4, BLACK);

        bool res = is_focused && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if(res) PlaySound(btn_click);
        return res;
    }

    void manage_bg_music(bool& is_bg_playing)
    {
        // pause music button
        if(draw_ui_button(
            {GetRenderWidth()-50.0f-TILE_SIZE*4, 50, TILE_SIZE*4, TILE_SIZE},
            is_bg_playing ? "Turn off music" : "Turn on music", 20))
        {
            is_bg_playing = !is_bg_playing;
        }
    }

    void manage_back_button(SceneEnum& scene_id, SceneEnum move_to_scene)
    {
        if(draw_ui_button({50, 50, 4*TILE_SIZE, TILE_SIZE}, "Go Back", 20))
        {
            scene_id = move_to_scene;
        }
    }

    void draw_text_center(string text, float y, bool is_title, int font_size)
    {
        int spacing = is_title ? 15 : 5;
        auto textlen = MeasureTextEx(font_normal, text.c_str(), font_size, spacing);
        float x = (GetRenderWidth() - textlen.x) / 2;

        if(is_title)
            DrawTextEx(font_normal, text.c_str(), {x+4, y+4}, font_size, spacing, BLACK);
        DrawTextEx(font_normal, text.c_str(), {x, y}, font_size, spacing, is_title ? RED : BLACK);
    }
};