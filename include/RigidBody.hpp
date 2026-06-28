#pragma once

#include "defines.hpp"
#include "raylib.h"
#include "raymath.h"

struct RigidBody
{
    Vector2 pos, vel = {0, 0};
    float width = 0, height = 0;

    RigidBody(): width(PL_SIZE), height(PL_SIZE) {}
    RigidBody(float x, float y, float width = PL_SIZE, float height = PL_SIZE):
        pos({x, y}), width(width), height(height) {}

    void update(Vector2 gravity, vector<Rectangle>& walls, Sound hit_sound)
    {
        vel = Vector2Add(vel, 
            Vector2Scale(gravity, GetFrameTime()));
        pos = Vector2Add(pos, 
            Vector2Scale(vel, GetFrameTime()));

        Rectangle body_rect = {pos.x, pos.y, width, height};
        for (auto& rect : walls) {
            Rectangle collide_rect = GetCollisionRec(
                body_rect,
                rect);
            
            if(collide_rect.height > collide_rect.width)
            {
                // Debug::get().draw("collide hei\n");
                if(collide_rect.x > body_rect.x)
                {
                    body_rect.x -= collide_rect.width;
                    if(collide_rect.width && vel.x > 0)
                    {
                        vel.x = 0;
                        // PlaySound(hit_sound);
                    }
                }
                else if(collide_rect.x == body_rect.x)
                {
                    body_rect.x += collide_rect.width;
                    if(collide_rect.width && vel.x < 0)
                    {
                        vel.x = 0;
                        // PlaySound(hit_sound);
                    }
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
                    if(collide_rect.height && vel.y > 0)
                    {
                        vel.y = 0;
                        // PlaySound(hit_sound);
                    }
                }
                else if(collide_rect.y == body_rect.y)
                {
                    body_rect.y += collide_rect.height;
                    if(collide_rect.height && vel.y < 0)
                    {
                        vel.y = 0;
                        // PlaySound(hit_sound);
                    }
                }
            }
        }
        pos.x = body_rect.x;
        pos.y = body_rect.y;
    }
};