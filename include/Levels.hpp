#pragma once
#include "Assets.hpp"
#include "Debug.hpp"
#include "RigidBody.hpp"
#include "defines.hpp"
#include "raylib.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <raymath.h>
#include <string>

#define MAX_LEVELS 8

// ASSERT PL_COLLIDE_W less than PL_COLLIDE_H
#define PL_COLLIDE_W PL_SIZE*(9.0f/16)
#define PL_COLLIDE_H PL_SIZE*(12.0f/16)

struct LevelSimulate
{
    int wall_thick = 1, level_width_tiles = 13+2*wall_thick, level_height_tiles = 13+2*wall_thick;
    bool is_paused = true, is_dead = false, is_won = false;

    // level data
    // Vector2 player_pos = {0, 0}, player_vel = {0, 0};
    int level_num = 0;
    LEVEL_UP up_dir = LEVEL_UP::UP;

    RigidBody player = RigidBody(0.0f, 0, PL_COLLIDE_W, PL_COLLIDE_H);
    vector<Rectangle> walls;

    // pos, is_vertical, up_dir
    vector<std::tuple<Vector2, bool, LEVEL_UP>> wall_spikes;

    vector<RigidBody> spike_blocks;

    Vector2 win_door;
    LEVEL_UP win_door_up = UP;

    // cur, start, end, length, velocity
    vector<std::tuple<Vector2, Vector2, Vector2, float, Vector2>> sliding_plats;

    Vector2 torch_light; // needs to point up to open the door
    LEVEL_UP torch_up = LEVEL_UP::UP;
    bool has_torch = false;

    std::string level_hint;

    // once every program start
    void init()
    {
    }

    void load_level_1();
    void load_level_2();
    void load_level_3();
    void load_level_4();
    void load_level_5();
    void load_level_6();
    void load_level_7();
    void load_level_8();

    void clear_level()
    {
        player.vel = {0, 0};
        walls.clear();
        wall_spikes.clear();
        spike_blocks.clear();
        sliding_plats.clear();
        has_torch = false;
        is_paused = true;
        is_dead = false;
        is_won = false;
        up_dir = UP;
        win_door_up = UP;
    }
    void add_walls();

    // everytime new level is loaded
    // assume previous level data is still loaded, unload it first
    void load_test_level();

    void load_level(int passed_level_num)
    {
        level_num = passed_level_num;
        clear_level();
        std::cout << "loading level " << level_num << std::endl;
        if(level_num == 1) load_level_1();
        else if(level_num == 2) load_level_2();
        else if(level_num == 3) load_level_3();
        else if(level_num == 4) load_level_4();
        else if(level_num == 5) load_level_5();
        else if(level_num == 6) load_level_6();
        else if(level_num == 7) load_level_7();
        else if(level_num == 8) load_level_8();
        else load_test_level();
    }

    // destory when closing game
    void deinit()
    {

    }

    // display dir to physics dir and vice versa
    // inverse=True  -> display to physics
    // inverse=False -> physics to display
    Vector2 transform_dir(float dir_x, float dir_y, bool inverse)
    {
        Vector2 inp_dir = {dir_x, dir_y};
        Vector2 rotated = Vector2Rotate(inp_dir,
            rotated_angle(up_dir) * DEG2RAD * (inverse ? -1 : 1));

        return rotated;
    }

    // physics coord to display coord
    Rectangle transform_cord(float x, float y, float wid, float hei, float x_offset, float y_offset)
    {
        float level_width = level_width_tiles * TILE_SIZE;
        float level_height = level_height_tiles * TILE_SIZE;
        float res_x = 0, res_y = 0;
        switch (up_dir)
        {
            case UP:
            {
                res_x = x; res_y = y;
                break;
            }
            case RIGHT:
            {
                res_x = level_height - y - hei; res_y = x;
                break;
            }
            case DOWN:
            {
                res_x = level_width - x - wid; res_y = level_height - y - hei;
                break;
            }
            case LEFT:
            {
                res_x = y; res_y = level_width - x - wid;
                break;
            }
            break;
        }
        float resw = wid, resh = hei;
        if(up_dir == LEFT || up_dir == RIGHT)
        {
            resh = wid; resw = hei;
        }
        return {res_x+x_offset, res_y+y_offset, resw, resh};
    }

    // independent of angle
    void level_simulate_phy(Assets& assets)
    {
        // set gravity based on up direction
        Vector2 gravity = Vector2Scale(transform_dir(0, 1, true), 500);

        // player controls
        Vector2 pl_display_vel = transform_dir(player.vel.x, player.vel.y, false);
        Vector2 pl_display_grav = transform_dir(gravity.x, gravity.y, false);
        // Debug::get().draw(TextFormat("pl dis vel %f, %f", pl_display_vel.x, pl_display_vel.y));
        // Debug::get().draw(TextFormat("pl dis grav %f, %f", pl_display_grav.x, pl_display_grav.y));
        if(std::abs(pl_display_vel.y) <= 0.0001f && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
        {
            pl_display_vel = Vector2Add(pl_display_vel,
                Vector2Scale({0, -1}, 200));
                PlaySound(assets.pl_jump);
        }

        float prev_x = pl_display_vel.x;
        // pl_display_vel.x = 0;
        bool left_down = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
        bool right_down = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        if(left_down && !right_down)
        {
            pl_display_vel.x -= 400*GetFrameTime();
            pl_display_vel.x = std::max(-200.0f, pl_display_vel.x);
        }
        else if(!left_down && right_down)
        {
            pl_display_vel.x += 400*GetFrameTime();
            pl_display_vel.x = std::min(200.0f, pl_display_vel.x);
        }
        else
        {
            // decelerate
            if(pl_display_vel.x) pl_display_vel.x = prev_x - std::min(std::abs(prev_x), 800*GetFrameTime())*(prev_x/abs(prev_x));
        }
        player.vel = transform_dir(pl_display_vel.x, pl_display_vel.y, true );

        // move sliding plats
        for (auto& slide_plat : sliding_plats) {
            // cur pos, start pos, end pos
            auto& cur_pos = std::get<0>(slide_plat);
            auto start_pos = std::get<1>(slide_plat);
            auto end_pos = std::get<2>(slide_plat);
            auto plt_len = std::get<3>(slide_plat);
            auto& plt_vel = std::get<4>(slide_plat);
            if(start_pos.y == end_pos.y)
            {
                // only move x
                plt_vel.x += gravity.x * GetFrameTime();
                cur_pos.x += plt_vel.x * GetFrameTime();
                cur_pos.x = Clamp(cur_pos.x, start_pos.x, end_pos.x - plt_len);
                if(cur_pos.x == start_pos.x && plt_vel.x < 0) plt_vel.x = 0;
                if(cur_pos.x == end_pos.x - plt_len && plt_vel.x > 0) plt_vel.x = 0;
            }
            else if(start_pos.x == end_pos.x)
            {
                // only move y
                plt_vel.y += gravity.y * GetFrameTime();
                cur_pos.y += plt_vel.y * GetFrameTime();
                cur_pos.y = Clamp(cur_pos.y, start_pos.y, end_pos.y - plt_len);
                if(cur_pos.y == start_pos.y && plt_vel.y < 0) plt_vel.y = 0;
                if(cur_pos.y == end_pos.y - plt_len && plt_vel.y > 0) plt_vel.y = 0;
            }
        }

        vector<Rectangle> dummy_walls = walls;
        for (auto& slide_plat : sliding_plats)
        {
            auto cur_pos = std::get<0>(slide_plat);
            auto start_pos = std::get<1>(slide_plat);
            auto end_pos = std::get<2>(slide_plat);
            auto plt_len = std::get<3>(slide_plat);
            bool is_vertical = (start_pos.x == end_pos.x);
            
            dummy_walls.push_back({cur_pos.x, cur_pos.y, 
                    is_vertical ? TILE_SIZE: plt_len,
                    is_vertical ? plt_len: TILE_SIZE});
        }

        // update pos and vel
        // player rotates based on up_dir
        if(up_dir == LEFT || up_dir == RIGHT)
        {
            player.width = PL_COLLIDE_H;
            player.height = PL_COLLIDE_W;
        }
        else
        {
            player.width = PL_COLLIDE_W;
            player.height = PL_COLLIDE_H;
        }
        player.update(gravity, dummy_walls, assets.box_hit);

        // Player rect
        Rectangle pl_rect = {player.pos.x, player.pos.y, player.width, player.height};
        Rectangle pl_rect_dis = transform_cord(pl_rect.x, pl_rect.y, pl_rect.width, pl_rect.height, 100, 100);
        // DrawRectangleLinesEx(pl_rect_dis, 3, RED);

        // wall spike collisions
        for(auto spike_pos: wall_spikes)
        {
            bool is_vertical = std::get<1>(spike_pos);
            float wid = is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            float hei = !is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            Rectangle rec = {
                std::get<0>(spike_pos).x*TILE_SIZE,
                std::get<0>(spike_pos).y*TILE_SIZE,
                wid, hei};
            Rectangle col_rect = GetCollisionRec(pl_rect, rec);
            float mincol = std::min(col_rect.width, col_rect.height);
            if(mincol >= 0.1*PL_COLLIDE_W)
            {
                is_dead = true;
                PlaySound(assets.dead);
            }
        }

        // spike block collisions
        for (auto& block : spike_blocks)
        {
            block.update(gravity, dummy_walls, assets.box_hit);
            Rectangle block_rect = {block.pos.x, block.pos.y, block.width, block.height};
            Rectangle colrect = GetCollisionRec(block_rect, pl_rect);
            if(std::min(colrect.width, colrect.height) >= 0.1*PL_COLLIDE_W)
            {
                is_dead = true;
                PlaySound(assets.dead);
            }
        }
        // if(spike_blocks.size())
        // {
        //     std::cout << "spike " << spike_blocks[0].pos.x << ", " << spike_blocks[0].pos.y << std::endl;
        //     std::cout << "spike " << spike_blocks[0].vel.x << ", " << spike_blocks[0].vel.y << std::endl;
        // }

        // win door collision
        Rectangle win_door_phy = {win_door.x*TILE_SIZE, win_door.y*TILE_SIZE, TILE_SIZE, 2*TILE_SIZE};
        // check if torch-light exists, it is UP
        if(!has_torch || (torch_up + up_dir)%4 == 0)
        {
            // std::cout << "torch ok\n";
            Rectangle colrect = GetCollisionRec(win_door_phy, pl_rect);
            if(std::min(colrect.width, colrect.height) >= 0.8*PL_COLLIDE_W)
            {
                // std::cout << "win\n";
                is_won = true;
                PlaySound(assets.level_win);
            }
        }


        // level collisions done inside update
        // for (auto rect : walls)
        // {
        //     auto xy = rect;
        //     // std::cout << "walls " << xy.x << ", " << xy.y << std::endl;
        // }
    }

    void draw(Assets& assets, SceneEnum& scene_id, bool& is_bg_playing)
    {
        if(level_num > MAX_LEVELS) scene_id = CREDITS;

        // rotate level
        if(IsKeyPressed(KEY_SPACE))
        {
            is_paused = !is_paused;
        }
        if(is_paused)
        {
            int res = up_dir;
            if(IsKeyPressed(KEY_Q))
            {
                res = up_dir - 1;
                if(res < 0) res += 4;
            }
            if(IsKeyPressed(KEY_E))
            {
                res = up_dir + 1;
                if(res >= 4) res -= 4;
            }
            if(res == 0) up_dir = UP;
            else if(res == 1) up_dir = RIGHT;
            else if(res == 2) up_dir = DOWN;
            else if(res == 3) up_dir = LEFT;
            else
            {
                std::cout << "Invalid Up direction\n"; up_dir = UP;
            }
        }
        // Debug::get().draw(TextFormat("UP dir %d", up_dir));
        // Debug::get().draw(TextFormat("Paused %d", is_paused));
        // Debug::get().draw(TextFormat("IsDead %d", is_dead));
        // Debug::get().draw(TextFormat("IsWon  %d", is_won));

        if(!is_paused && !is_dead && !is_won)
        {
            level_simulate_phy(assets);
            // update animations
            assets.update();
        }

        int draw_width = level_width_tiles, draw_height = level_height_tiles;
        if(up_dir == LEFT || up_dir == RIGHT)
        {
            draw_height = level_width_tiles; draw_width = level_height_tiles;
        }
        float x_offset = (GetRenderWidth()  - draw_width  * TILE_SIZE) / 2.0f;
        float y_offset = (GetRenderHeight() - draw_height * TILE_SIZE) / 2.0f;

        assets.draw_level_bg(x_offset, y_offset, draw_width, draw_height);
        
        // draw level name and level hint
        float left_txt_yoff = assets.draw_text(TextFormat("Level %d", level_num), 100, 256, 5*TILE_SIZE, 42);
        left_txt_yoff += assets.draw_text(level_hint,
            100, 400 + left_txt_yoff, TILE_SIZE*5, 24);

        for (auto rect : walls)
        {
            auto xy = transform_cord(rect.x, rect.y, rect.width, rect.height, x_offset, y_offset);
            // DrawRectangleLinesEx(xy, 2, BLACK);
            // // Debug::get().draw(TextFormat("wall %f, %f, %f, %f", xy.x, xy.y, xy.width, xy.height));
            assets.draw_wall(xy);
        }

        for (auto& slide_plat : sliding_plats)
        {
            auto cur_pos = std::get<0>(slide_plat);
            auto start_pos = std::get<1>(slide_plat);
            auto end_pos = std::get<2>(slide_plat);
            auto plt_len = std::get<3>(slide_plat);
            bool is_vertical = (start_pos.x == end_pos.x);
            
            // DrawRectangleLinesEx(
            //     transform_cord(start_pos.x, start_pos.y, 
            //         is_vertical ? TILE_SIZE*0.5f: end_pos.x - start_pos.x,
            //         is_vertical ? end_pos.y-start_pos.y: TILE_SIZE*0.5f, x_offset, y_offset), 5, BLUE);
            float beh_x = is_vertical ? start_pos.x + TILE_SIZE*0.45f : start_pos.x;
            float beh_y = !is_vertical ? start_pos.y + TILE_SIZE*0.45f : start_pos.y;
            float beh_w = is_vertical ?  TILE_SIZE*0.1f : end_pos.x-start_pos.x;
            float beh_h = !is_vertical ?  TILE_SIZE*0.1f : end_pos.y-start_pos.y;
            DrawRectangleRec(
                transform_cord(beh_x, beh_y, beh_w, beh_h, x_offset, y_offset),
                DARKGRAY);
            auto slide_rect = transform_cord(cur_pos.x, cur_pos.y, 
                    is_vertical ? TILE_SIZE: plt_len,
                    is_vertical ? plt_len: TILE_SIZE, x_offset, y_offset);
            assets.draw_slide_plat(slide_rect,
                (up_dir == LEFT || up_dir == RIGHT) ? !is_vertical : is_vertical);
        }

        for(auto spike_pos: wall_spikes)
        {
            bool on_wall = false, is_vertical = std::get<1>(spike_pos);
            float wid = is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            float hei = !is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            assets.draw_wall_spikes(
                transform_cord(
                    std::get<0>(spike_pos).x*TILE_SIZE,
                    std::get<0>(spike_pos).y*TILE_SIZE,
                    wid, hei, x_offset, y_offset),
                    std::get<1>(spike_pos),
                    int_to_up(up_dir + std::get<2>(spike_pos)));
        }

        for (auto& block : spike_blocks) {
            Rectangle rect = transform_cord(block.pos.x, block.pos.y, block.width, block.height, x_offset, y_offset);
            // DrawRectangleLinesEx(rect, 5, RED);
            assets.draw_spike_block(rect);
        }
        
        Rectangle win_door_display = transform_cord(
            win_door.x*TILE_SIZE,
            win_door.y*TILE_SIZE,
            (win_door_up == UP || win_door_up == DOWN) ? TILE_SIZE : 2*TILE_SIZE,
            (win_door_up == UP || win_door_up == DOWN) ? 2*TILE_SIZE : TILE_SIZE,
            x_offset, y_offset);
        // DrawRectangleLinesEx(win_door_display, 5, BLUE);
        assets.draw_door(win_door_display,
            int_to_up(win_door_up + up_dir),
            (!has_torch || (torch_up + up_dir) % 4 == 0));

        if(has_torch)
        {
            Rectangle torch_display = transform_cord(
                torch_light.x*TILE_SIZE,
                torch_light.y*TILE_SIZE,
                (torch_up-UP)%2 ? TILE_SIZE : 0.5f*TILE_SIZE,
                (torch_up-UP)%2 ? 0.5f*TILE_SIZE : TILE_SIZE,
                x_offset, y_offset);
            // Debug::get().draw(TextFormat("torch dir %d", (torch_up + up_dir)%4));
            assets.draw_torch(torch_display, int_to_up(torch_up+up_dir));
        }



        // Debug::get().draw(TextFormat(
        //     "pl vel %f, %f",
        //     player.vel.x, player.vel.y
        // ));
        // Vector2 display_vel = transform_dir(player.vel.x, player.vel.y, false);
        // Debug::get().draw(TextFormat(
        //     "pl dis vel %f, %f",
        //     display_vel.x, display_vel.y
        // ));

        Rectangle pl_rect = transform_cord(player.pos.x, player.pos.y, player.width, player.height, x_offset, y_offset);
        auto pl_vel = transform_dir(player.vel.x, player.vel.y, false);
        /* Debug::get().draw(TextFormat(
            "Pl Rect %f, %f, %f, %f",
            pl_rect.x, pl_rect.y, pl_rect.width, pl_rect.height
        )); */
        // DrawRectangleLinesEx({pl_rect.x, pl_rect.y, player.width, player.height}, 2, DARKGREEN);
        assets.draw_player(pl_rect.x, pl_rect.y, pl_vel.x, pl_vel.y);

        // win condition
        if(is_won)
        {
            if(level_num < MAX_LEVELS)
            {
                bool res = assets.draw_ui_button({100, 550 + left_txt_yoff, 5*TILE_SIZE, 2*TILE_SIZE}, "Next Level", 24);
                if(res || IsKeyPressed(KEY_N))
                {
                    load_level(level_num+1);
                }
            }
            else {
                bool res = assets.draw_ui_button({100, 550 + left_txt_yoff, 5*TILE_SIZE, 2*TILE_SIZE}, "Credits", 24);
                if(res || IsKeyPressed(KEY_N))
                {
                    scene_id = CREDITS;
                }
            }
        }

        // pause status
        if(is_paused)
        {
            assets.draw_text("Paused", 102, 752, 5*TILE_SIZE, 36);
            assets.draw_text("Paused", 100, 750, 5*TILE_SIZE, 36, WHITE);
        }

        // after death
        if(is_dead)
        {
            bool res = assets.draw_ui_button({100, 550 + left_txt_yoff, 5*TILE_SIZE, 2*TILE_SIZE}, "Try again", 24);
            if(res || IsKeyPressed(KEY_N))
            {
                load_level(level_num);
            }
        }


        // back button
        assets.manage_back_button(scene_id, SceneEnum::LEVEL_SELECT);

        // pause music button
        assets.manage_bg_music(is_bg_playing);
    }
};