#pragma once
#include <raylib.h>
#include "defines.hpp"


class DebugNode
{
    public:
    union
    {
        float* fl;
        Vector2* v2;
        Vector3* v3;
        Rectangle* rec;
        int* in;
    };
    int active;

    DebugNode(float* f)
    {
        fl = f;
        active = 0;
    }
    DebugNode(Vector2* f)
    {
        v2 = f;
        active = 1;
    }
    DebugNode(Vector3* f)
    {
        v3 = f;
        active = 2;
    }
    DebugNode(Rectangle* f)
    {
        rec = f;
        active = 3;
    }
    DebugNode(int* f)
    {
        in = f;
        active = 4;
    }

    void draw(float x, float y, int fontSize, Color color)
    {
        switch (active)
        {
        case 0:
            {DrawText(TextFormat("%f", *fl), x, y, fontSize, color);
            break;}
        case 1:
            {DrawText(TextFormat("%f, %f", v2->x, v2->y), x, y, fontSize, color);
            break;}
        case 2:
            {DrawText(
                TextFormat("%f, %f, %f", v3->x, v3->y, v3->z),
                x, y, fontSize, color);
            break;}
        case 3:
            {DrawText(
                TextFormat("%f, %f, %f, %f", rec->x, rec->y, rec->width, rec->height),
                x, y, fontSize, color);
            break;}
        case 4:
            {DrawText(TextFormat("%d", *in), x, y, fontSize, color);
            break;}
        default:
            break;
        }
    }
    
};

class Debug
{
private:
    int fontSize;
    static Debug debug;
    vector<DebugNode> nodes;
    float x, y;
    Debug(int _fontSize = 16, float xx = 30, float yy = 30)
     : fontSize(_fontSize), x(xx), y(yy) {}

public:
    void addNode(int* f)
    {
        nodes.push_back(DebugNode(f));
    }
    void addNode(float* f)
    {
        nodes.push_back(DebugNode(f));
    }
    void addNode(Vector2* f)
    {
        nodes.push_back(DebugNode(f));
    }
    void addNode(Vector3* f)
    {
        nodes.push_back(DebugNode(f));
    }
    void addNode(Rectangle* f)
    {
        nodes.push_back(DebugNode(f));
    }

    void draw()
    {
        reset(50);
        for (auto &&i : nodes)
        {
            i.draw(x, y, fontSize, WHITE);
            y += fontSize * 1.5;
        }
        
    }

    void draw(const char *str)
    {
        // DebugNode(&fl).draw(x, y, fontSize, DARKGRAY);
        DrawText(str, x, y, fontSize, BLACK);
        y += fontSize * 1.5;
    }
    void draw(float fl, string tag = "")
    {
        if(!tag.empty()) DrawText(TextFormat("%s: %f", tag.c_str(), fl), x, y, fontSize, WHITE);
        else DebugNode(&fl).draw(x, y, fontSize, BLACK);
        y += fontSize * 1.5;
    }
    void set_x(float xx)
    {
        x = xx;
    }

    void reset(float yy)
    {
        y = yy;
    }

    static Debug& get()
    {
        return debug;
    }
};
