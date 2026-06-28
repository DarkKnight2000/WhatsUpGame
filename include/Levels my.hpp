#pragma once
#include "Assets.hpp"
#include "Debug.hpp"
#include "defines.hpp"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <raymath.h>


#define PL_SIZE TILE_SIZE

enum LEVEL_UP {UP, RIGHT, DOWN, LEFT};

struct RigidBody
{
    Vector2 pos, vel;

    void update(Vector2 gravity, vector<Rectangle>& walls)
    {
        vel = Vector2Add(vel, 
            Vector2Scale(gravity, GetFrameTime()));
        pos = Vector2Add(pos, 
            Vector2Scale(vel, GetFrameTime()));

        Rectangle body_rect = {pos.x, pos.y, PL_SIZE, PL_SIZE};
        for (auto rect : walls) {
            Rectangle collide_rect = GetCollisionRec(
                body_rect,
                rect);
            
            if(collide_rect.height > collide_rect.width)
            {
                // Debug::get().draw("collide hei\n");
                if(collide_rect.x > body_rect.x)
                {
                    body_rect.x -= collide_rect.width;
                    if(collide_rect.width && vel.x > 0) vel.x = 0;
                }
                else if(collide_rect.x == body_rect.x)
                {
                    body_rect.x += collide_rect.width;
                    if(collide_rect.width && vel.x < 0) vel.x = 0;
                }
            }
            else if(collide_rect.width > 0)
            {
                // Debug::get().draw("collide wid\n");
                if(collide_rect.y > body_rect.y)
                {
                    // Debug::get().draw("falling down\n");
                    // Debug::get().draw(TextFormat("col hei %f, pl y %f", collide_rect.height, pl_rect.y));
                    body_rect.y -= collide_rect.height;
                    // Debug::get().draw(TextFormat("pl y %f", pl_rect.y));
                    if(collide_rect.height && vel.y > 0) vel.y = 0;
                }
                else if(collide_rect.y == body_rect.y)
                {
                    body_rect.y += collide_rect.height;
                    if(collide_rect.height && vel.y < 0) vel.y = 0;
                }
            }
        }
        pos.x = body_rect.x;
        pos.y = body_rect.y;
    }
};

struct LevelSimulate
{
    int wall_thick = 1, level_width_tiles = 12+2*wall_thick, level_height_tiles = 7+2*wall_thick;

    // level data
    // Vector2 player_pos = {0, 0}, player_vel = {0, 0};
    RigidBody player;
    LEVEL_UP up_dir = LEVEL_UP::UP;
    bool is_paused = true, is_dead = false;
    vector<Rectangle> walls;
    vector<Vector2> wall_spikes;
    vector<RigidBody> spike_blocks;

    // // level data - box2d
    // vector<b2BodyId> b2_walls;
    // // map<b2BodyId, Vector2> wall_dims;
    // b2BodyId b2_player;

    // once every program start
    void init()
    {
    }


    // everytime new level is loaded
    void load_level()
    {
        walls.push_back({0, 0, level_width_tiles*TILE_SIZE, wall_thick*TILE_SIZE});
        walls.push_back({0, 0, wall_thick*TILE_SIZE, level_height_tiles*TILE_SIZE});
        walls.push_back({(level_width_tiles-wall_thick)*TILE_SIZE, 0, wall_thick*TILE_SIZE, level_height_tiles*TILE_SIZE});
        walls.push_back({0, (level_height_tiles-wall_thick)*TILE_SIZE, level_width_tiles*TILE_SIZE, wall_thick*TILE_SIZE});
    
        player.pos = {2*TILE_SIZE, 2*TILE_SIZE};
        player.vel = {0, 0};

        wall_spikes.push_back({(float)wall_thick, 2});
        // wall_spikes.push_back({6, (float)wall_thick});
        // wall_spikes.push_back({level_width_tiles-wall_thick-0.5f, 4});
        // wall_spikes.push_back({6, level_height_tiles-wall_thick-0.5f});
    }

    // destory when closing game
    void deinit()
    {

    }

    // angle from actual UP in degrees
    int rotated_angle()
    {
        switch (up_dir)
        {
            case UP: return 0;
            case RIGHT: return 90;
            case DOWN: return 180;
            case LEFT: return 270;
            default: return 0;
        }
    }

    // display dir to physics dir and vice versa
    // inverse=True  -> display to physics
    // inverse=False -> physics to display
    Vector2 transform_dir(float dir_x, float dir_y, bool inverse)
    {
        Vector2 inp_dir = {dir_x, dir_y};
        Vector2 rotated = Vector2Rotate(inp_dir,
            rotated_angle() * DEG2RAD * (inverse ? -1 : 1));

        return rotated;
    }

    // independent of angle
    void level_simulate_phy()
    {
        // set gravity based on up direction
        Vector2 gravity = Vector2Scale(transform_dir(0, 1, true), 500);

        if(!is_paused && !is_dead)
        {
            player.update(gravity, walls);
        }

        // Player rect
        Rectangle pl_rect = {player.pos.x, player.pos.y, PL_SIZE, PL_SIZE};

        // wall spike collisions
        for(auto spike_pos: wall_spikes)
        {
            bool is_vertical = false;
            if(spike_pos.x == wall_thick || spike_pos.x == level_width_tiles-wall_thick-0.5f)
            {
                is_vertical = true;
            }
            float wid = is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            float hei = !is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            Rectangle rec = {
                spike_pos.x*TILE_SIZE,
                spike_pos.y*TILE_SIZE,
                wid, hei};
            Rectangle col_rect = GetCollisionRec(pl_rect, rec);
            float mincol = std::min(col_rect.width, col_rect.height);
            if(mincol >= TILE_SIZE*0.1)
            {
                is_dead = true;
            }
        }

        // spike block collisions
        for (auto& block : spike_blocks)
        {
            if(!is_paused && !is_dead)
            {
                block.update(gravity, walls);
            }
            Rectangle block_rect = {block.pos.x, block.pos.y, PL_SIZE, PL_SIZE};
            Rectangle colrect = GetCollisionRec(block_rect, pl_rect);
            if(std::min(colrect.width, colrect.height) >= 0.1*TILE_SIZE)
            {
                is_dead = true;
            }
        }


        // level collisions done inside update


        // player controls
        Vector2 pl_display_vel = transform_dir(player.vel.x, player.vel.y, false);
        Vector2 pl_display_grav = transform_dir(gravity.x, gravity.y, false);
        Debug::get().draw(TextFormat("pl dis vel %f, %f", pl_display_vel.x, pl_display_vel.y));
        Debug::get().draw(TextFormat("pl dis grav %f, %f", pl_display_grav.x, pl_display_grav.y));
        if(pl_display_vel.y == 0 && IsKeyPressed(KEY_UP))
        {
            player.vel = Vector2Add(player.vel,
                Vector2Scale(transform_dir(0, -1, true), 200));
            auto rr = Vector2Scale(transform_dir(0, -1, true), 200);
            std::cout << "user up " << rr.x << ", " << rr.y << std::endl;
        }
        if(IsKeyPressed(KEY_LEFT) && pl_display_vel.x >= 0)
        {
            pl_display_vel.x -= 200;
        }
        else if(IsKeyReleased(KEY_LEFT))
        {
            pl_display_vel.x += 200;
        }
        if(IsKeyPressed(KEY_RIGHT) && pl_display_vel.x <= 0)
        {
            pl_display_vel.x += 200;
        }
        else if(IsKeyReleased(KEY_RIGHT))
        {
            pl_display_vel.x -= 200;
        }
        player.vel = transform_dir(pl_display_vel.x, pl_display_vel.y, true );
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

    void draw(Assets& assets)
    {
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
        Debug::get().draw(TextFormat("UP dir %d", up_dir));
        Debug::get().draw(TextFormat("Paused %d", is_paused));
        Debug::get().draw(TextFormat("IsDead %d", is_dead));

        level_simulate_phy();

        int draw_width = level_width_tiles, draw_height = level_height_tiles;
        if(up_dir == LEFT || up_dir == RIGHT)
        {
            draw_height = level_width_tiles; draw_width = level_height_tiles;
        }
        float x_offset = (GetRenderWidth()  - draw_width  * TILE_SIZE) / 2.0f;
        float y_offset = (GetRenderHeight() - draw_height * TILE_SIZE) / 2.0f;

        for (auto rect : walls)
        {
            auto xy = transform_cord(rect.x, rect.y, rect.width, rect.height, x_offset, y_offset);
            DrawRectangleLinesEx(xy, 2, BLACK);
        }

        for(auto spice_pos: wall_spikes)
        {
            bool on_wall = false, is_vertical = false;
            if(spice_pos.x == wall_thick || spice_pos.x == level_width_tiles-wall_thick-0.5f)
            {
                on_wall = true;
                is_vertical = true;
            }
            // if(spice_pos.y == wall_thick || spice_pos.y == level_height_tiles-wall_thick-0.5f)
            //     on_wall = true;
            // remove wall if on_wall

            // if(up_dir == LEFT || up_dir == RIGHT)
            // {
            //     is_vertical = !is_vertical;
            // }
            float wid = is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            float hei = !is_vertical ? TILE_SIZE*0.5f : TILE_SIZE;
            assets.draw_wall_spikes(
                transform_cord(
                    spice_pos.x*TILE_SIZE,
                    spice_pos.y*TILE_SIZE,
                    wid, hei, x_offset, y_offset));
        }

        // Debug::get().draw(TextFormat(
        //     "Top wall %f, %f, %f, %f",
        //     walls[0].x, walls[0].y, walls[0].width, walls[0].height
        // ));
        // auto trect = transform_cord(walls[0].x, walls[0].y, walls[0].width, walls[0].height, 0, 0);
        // Debug::get().draw(TextFormat(
        //     "Top wall2 %f, %f, %f, %f",
        //     trect.x, trect.y, trect.width, trect.height
        // ));
        Debug::get().draw(TextFormat(
            "pl vel %f, %f",
            player.vel.x, player.vel.y
        ));

        Rectangle pl_rect = transform_cord(player.pos.x, player.pos.y, PL_SIZE, PL_SIZE, x_offset, y_offset);
        Debug::get().draw(TextFormat(
            "Pl Rect %f, %f, %f, %f",
            pl_rect.x, pl_rect.y, pl_rect.width, pl_rect.height
        ));
        DrawRectangleLinesEx(pl_rect, 5, DARKGREEN);
    }
};