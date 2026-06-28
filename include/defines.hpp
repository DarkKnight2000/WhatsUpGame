#pragma once
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;


#define TILE_COLS 16
#define TILE_ROWS 9
#define TILE_SIZE 64.0f
#define ASSETS_PATH "res/platformer/Spritesheets/"
#define B2_SCALE 1.0f
#define PL_SIZE TILE_SIZE
#define UI_ASSET_SCALE 32.0f

enum LEVEL_UP {UP, RIGHT, DOWN, LEFT};

inline LEVEL_UP int_to_up(int res)
{
    res = res % 4;
    if(res == 0) return UP;
    else if(res == 1) return RIGHT;
    else if(res == 2) return DOWN;
    else if(res == 3) return LEFT;
    return UP;
}


// angle from actual UP in degrees
inline int rotated_angle(LEVEL_UP up_dir)
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

enum SceneEnum
{
    HOME, LEVEL_SELECT, LEVEL, CREDITS
};
