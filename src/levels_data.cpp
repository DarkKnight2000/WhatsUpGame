#include "Levels.hpp"
#include "defines.hpp"

void LevelSimulate::add_walls()
{
    walls.push_back({0, 0, level_width_tiles*TILE_SIZE, wall_thick*TILE_SIZE});
    walls.push_back({0, 0, wall_thick*TILE_SIZE, level_height_tiles*TILE_SIZE});
    walls.push_back({(level_width_tiles-wall_thick)*TILE_SIZE, 0, wall_thick*TILE_SIZE, level_height_tiles*TILE_SIZE});
    walls.push_back({0, (level_height_tiles-wall_thick)*TILE_SIZE, level_width_tiles*TILE_SIZE, wall_thick*TILE_SIZE});
}

void LevelSimulate::load_test_level()
{
    level_hint = "Oops this is a test level. You shouldn't be seeing this.";
    level_width_tiles = 7+2*wall_thick;
    level_height_tiles = 5+2*wall_thick;
    add_walls();
    player.pos = {2*TILE_SIZE, (level_height_tiles-wall_thick-2)*TILE_SIZE};

    wall_spikes.push_back({{(float)wall_thick, wall_thick+1.0f}, true, RIGHT});
    // wall_spikes.push_back({6, (float)wall_thick});
    // wall_spikes.push_back({level_width_tiles-wall_thick-0.5f, 4});
    // wall_spikes.push_back({6, level_height_tiles-wall_thick-0.5f});

    spike_blocks.push_back({3.0f*wall_thick*TILE_SIZE, 3.0f*wall_thick*TILE_SIZE});

    win_door = {level_width_tiles-1.0f*wall_thick-2, level_height_tiles-1.0f*wall_thick-2};

    has_torch = true;
    torch_light = {(level_width_tiles-wall_thick-1.0f)*wall_thick, 2.0f*wall_thick};
    torch_up = LEVEL_UP::UP;

    sliding_plats.push_back({
        {2*TILE_SIZE, 2*TILE_SIZE},
        {2*TILE_SIZE, 2*TILE_SIZE},
        {2*TILE_SIZE, 4*TILE_SIZE},
        TILE_SIZE,
        {0, 0}
    });
    sliding_plats.push_back({
        {3*TILE_SIZE, 5*TILE_SIZE},
        {2*TILE_SIZE, 5*TILE_SIZE},
        {6*TILE_SIZE, 5*TILE_SIZE},
        2*TILE_SIZE,
        {0, 0}
    });
}

// Just demonstrate gravity shift
// Player starts bot-left, door is top-right
void LevelSimulate::load_level_1()
{
    level_hint = "Use WASD to move. Press 'Q' or 'E' to spin the entire level. Press SPACE to pause/unpause time.";
    level_width_tiles = 8 + 2 * wall_thick;
    level_height_tiles = 5 + 2 * wall_thick;
    add_walls();
    player.pos = {(wall_thick + 1)*TILE_SIZE, (level_height_tiles-wall_thick)*TILE_SIZE-PL_SIZE};
    win_door = {(level_width_tiles-wall_thick-1.0f), (float)wall_thick};
    win_door_up = DOWN;
}

// Falling spike blocks everywhere except in one col
void LevelSimulate::load_level_2()
{
    level_hint = "Its all about the momentum.";
    level_width_tiles = 10 + 2 * wall_thick;
    level_height_tiles = 7 + 2 * wall_thick;
    add_walls();
    player.pos = {(wall_thick)*TILE_SIZE, (level_height_tiles-wall_thick)*TILE_SIZE-PL_SIZE};
    win_door = {(float)(level_width_tiles-wall_thick-1), (float)wall_thick};
    win_door_up = DOWN;

    spike_blocks.push_back({
        (wall_thick+1)*TILE_SIZE,
        (float)wall_thick*TILE_SIZE+TILE_SIZE,
        (level_width_tiles-2*wall_thick-2)*TILE_SIZE,
        TILE_SIZE});
    // walls.push_back({(wall_thick + 3.0f)*TILE_SIZE, wall_thick*TILE_SIZE, TILE_SIZE, TILE_SIZE});

    // spike_blocks.push_back({
    //     (wall_thick+4)*TILE_SIZE,
    //     (float)wall_thick*TILE_SIZE+TILE_SIZE,
    //     (2)*TILE_SIZE,
    //     TILE_SIZE});
    // walls.push_back({(wall_thick + 6.0f)*TILE_SIZE, wall_thick*TILE_SIZE, TILE_SIZE, TILE_SIZE});
}

// Falling spike blocks everywhere except in one col
void LevelSimulate::load_level_3()
{
    level_hint = "The torch should point UP to unlock the door.";
    level_width_tiles = 10 + 2 * wall_thick;
    level_height_tiles = 7 + 2 * wall_thick;
    add_walls();
    player.pos = {(wall_thick)*TILE_SIZE, (level_height_tiles-wall_thick)*TILE_SIZE-PL_SIZE};
    win_door = {(float)(level_width_tiles-wall_thick-1), (float)wall_thick};
    win_door_up = DOWN;

    spike_blocks.push_back({
        (wall_thick+1)*TILE_SIZE,
        (float)wall_thick*TILE_SIZE+TILE_SIZE,
        (level_width_tiles-2*wall_thick-2)*TILE_SIZE,
        TILE_SIZE});
    
    has_torch = true;
    torch_light = {1.0f*wall_thick, 1.5f*wall_thick};
    torch_up = LEFT;
}

// Falling spike blocks everywhere except in one col
void LevelSimulate::load_level_4()
{
    level_hint = "Easy? Try doing that without landing anywhere.";
    level_width_tiles  = 13 + 2 * wall_thick;
    level_height_tiles = 13 + 2 * wall_thick;
    add_walls();
    player.pos = {(wall_thick+1)*TILE_SIZE, (level_height_tiles-wall_thick)*TILE_SIZE-PL_SIZE};
    win_door = {(float)(level_width_tiles-wall_thick-1), (float)wall_thick};
    win_door_up = DOWN;
    
    for (int i=wall_thick; i<level_height_tiles-wall_thick; i++)
    {
        wall_spikes.push_back({{(float)wall_thick, (float)i}, true, RIGHT});
    }

    walls.push_back({wall_thick*3*TILE_SIZE+PL_SIZE, wall_thick*2*TILE_SIZE+PL_SIZE, wall_thick*TILE_SIZE, (level_height_tiles-3*wall_thick)*TILE_SIZE-PL_SIZE});
    for (int i=wall_thick*2*TILE_SIZE+PL_SIZE; i<(level_height_tiles-wall_thick)*TILE_SIZE; i+=TILE_SIZE)
    {
        wall_spikes.push_back({{(wall_thick*2.5f*TILE_SIZE+PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, LEFT});
        wall_spikes.push_back({{(wall_thick*4.0f*TILE_SIZE+PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, RIGHT});
    }

    walls.push_back({wall_thick*5*TILE_SIZE+3*PL_SIZE, (float)wall_thick*TILE_SIZE, wall_thick*TILE_SIZE, (level_height_tiles-3*wall_thick)*TILE_SIZE-PL_SIZE});
    for (int i=wall_thick*TILE_SIZE; i<(level_height_tiles-3*wall_thick)*TILE_SIZE; i+=TILE_SIZE)
    {
        wall_spikes.push_back({{(wall_thick*4.5f*TILE_SIZE+3*PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, LEFT});
        wall_spikes.push_back({{(wall_thick*6.0f*TILE_SIZE+3*PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, RIGHT});
    }

    walls.push_back({wall_thick*7*TILE_SIZE+5*PL_SIZE, wall_thick*2*TILE_SIZE+PL_SIZE, 2*wall_thick*TILE_SIZE, (level_height_tiles-3*wall_thick)*TILE_SIZE-PL_SIZE});
    for (int i=wall_thick*2*TILE_SIZE+PL_SIZE; i<(level_height_tiles-wall_thick)*TILE_SIZE; i+=TILE_SIZE)
    {
        wall_spikes.push_back({{(wall_thick*6.5f*TILE_SIZE+5*PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, LEFT});
    }
}


// Falling spike blocks everywhere except in one col
void LevelSimulate::load_level_5()
{
    level_hint = "*Sigh* Yet another platforming level. But this one is a little more serious.";
    level_width_tiles  = 13 + 2 * wall_thick;
    level_height_tiles = 13 + 2 * wall_thick;
    add_walls();
    player.pos = {(wall_thick+1)*TILE_SIZE, (level_height_tiles-wall_thick)*TILE_SIZE-PL_SIZE};
    win_door = {(float)(level_width_tiles-wall_thick-1), (float)wall_thick};
    win_door_up = DOWN;
    
    for (int i=wall_thick; i<level_height_tiles-wall_thick; i++)
    {
        wall_spikes.push_back({{(float)wall_thick, (float)i}, true, RIGHT});
    }

    walls.push_back({wall_thick*3*TILE_SIZE+PL_SIZE, wall_thick*2*TILE_SIZE+2*PL_SIZE, wall_thick*TILE_SIZE, (level_height_tiles-3*wall_thick)*TILE_SIZE-2*PL_SIZE});
    wall_spikes.push_back({{(wall_thick+1.0f), (float)wall_thick}, false, DOWN});
    for (int i=wall_thick*2*TILE_SIZE+2*PL_SIZE; i<(level_height_tiles-wall_thick)*TILE_SIZE; i+=TILE_SIZE)
    {
        wall_spikes.push_back({{(wall_thick*2.5f*TILE_SIZE+PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, LEFT});
        wall_spikes.push_back({{(wall_thick*4.0f*TILE_SIZE+PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, RIGHT});
    }

    walls.push_back({wall_thick*5*TILE_SIZE+3*PL_SIZE, (float)wall_thick*TILE_SIZE, wall_thick*TILE_SIZE, (level_height_tiles-3*wall_thick)*TILE_SIZE-2*PL_SIZE});
    wall_spikes.push_back({{(wall_thick+2.0f)+3*(PL_SIZE/TILE_SIZE), (float)wall_thick}, false, DOWN});
    wall_spikes.push_back({{(wall_thick+2.0f)+3*(PL_SIZE/TILE_SIZE), level_height_tiles-wall_thick-0.5f}, false, UP});
    for (int i=wall_thick*TILE_SIZE; i<(level_height_tiles-4*wall_thick)*TILE_SIZE; i+=TILE_SIZE)
    {
        wall_spikes.push_back({{(wall_thick*4.5f*TILE_SIZE+3*PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, LEFT});
        wall_spikes.push_back({{(wall_thick*6.0f*TILE_SIZE+3*PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, RIGHT});
    }

    walls.push_back({wall_thick*7*TILE_SIZE+5*PL_SIZE, wall_thick*2*TILE_SIZE+2*PL_SIZE, 2*wall_thick*TILE_SIZE, (level_height_tiles-3*wall_thick)*TILE_SIZE-2*PL_SIZE});
    wall_spikes.push_back({{(wall_thick+4.0f)+5*(PL_SIZE/TILE_SIZE), level_height_tiles-wall_thick-0.5f}, false, UP});
    wall_spikes.push_back({{(wall_thick+4.0f)+5*(PL_SIZE/TILE_SIZE), (float)wall_thick}, false, DOWN});
    for (int i=wall_thick*2*TILE_SIZE+2*PL_SIZE; i<(level_height_tiles-wall_thick)*TILE_SIZE; i+=TILE_SIZE)
    {
        wall_spikes.push_back({{(wall_thick*6.5f*TILE_SIZE+5*PL_SIZE)/TILE_SIZE, i/TILE_SIZE}, true, LEFT});
    }
}


void LevelSimulate::load_level_6()
{
    level_hint = "Those gray blocks can be handy.";
    level_width_tiles = 8+2*wall_thick;
    level_height_tiles = 7+2*wall_thick;
    add_walls();
    player.pos = {2*TILE_SIZE, (level_height_tiles-wall_thick-2)*TILE_SIZE};

    wall_spikes.push_back({{(float)wall_thick, wall_thick+1.0f}, true, RIGHT});
    // wall_spikes.push_back({6, (float)wall_thick});
    // wall_spikes.push_back({level_width_tiles-wall_thick-0.5f, 4});
    // wall_spikes.push_back({6, level_height_tiles-wall_thick-0.5f});

    spike_blocks.push_back({4.0f*wall_thick*TILE_SIZE, 3.0f*wall_thick*TILE_SIZE});

    win_door = {level_width_tiles-1.0f*wall_thick-2, level_height_tiles-1.0f*wall_thick-2};

    has_torch = true;
    torch_light = {(level_width_tiles-wall_thick-1.0f)*wall_thick, 2.0f*wall_thick};
    torch_up = LEVEL_UP::UP;

    sliding_plats.push_back({
        {2*TILE_SIZE, 2*TILE_SIZE},
        {2*TILE_SIZE, 2*TILE_SIZE},
        {2*TILE_SIZE, 4*TILE_SIZE},
        TILE_SIZE,
        {0, 0}
    });
    sliding_plats.push_back({
        {3*TILE_SIZE, 7*TILE_SIZE},
        {2*TILE_SIZE, 7*TILE_SIZE},
        {6*TILE_SIZE, 7*TILE_SIZE},
        2*TILE_SIZE,
        {0, 0}
    });
}


void LevelSimulate::load_level_7()
{
    level_hint = "That's a lot of spikes.";
    level_width_tiles = 12+2*wall_thick;
    level_height_tiles = 8+2*wall_thick;
    add_walls();
    player.pos = {2*TILE_SIZE, (level_height_tiles-wall_thick-2)*TILE_SIZE};

    for (float i=wall_thick; i < level_height_tiles-wall_thick; i++) {
        wall_spikes.push_back({{(float)wall_thick, i}, true, RIGHT});
    }

    for (float i=wall_thick+1; i < level_width_tiles-wall_thick; i++) {
        wall_spikes.push_back({{i, (float)wall_thick}, false, DOWN});
    }

    for (float i=wall_thick+4; i < wall_thick+6; i++) {
        wall_spikes.push_back({{wall_thick+3.5f, i}, true, LEFT});
    }
    walls.push_back({(wall_thick+4.0f)*TILE_SIZE, (4+wall_thick)*TILE_SIZE, TILE_SIZE, 2*TILE_SIZE});
    for (float i=wall_thick+4; i < wall_thick+6; i++) {
        wall_spikes.push_back({{i, level_height_tiles-wall_thick-0.5f}, false, UP});
    }

    for (float i=wall_thick+4; i < level_height_tiles-1; i++) {
        wall_spikes.push_back({{wall_thick+6.5f, i}, true, LEFT});
    }
    walls.push_back({(wall_thick+7.0f)*TILE_SIZE, (2+wall_thick)*TILE_SIZE, TILE_SIZE, (level_height_tiles-4)*TILE_SIZE});

    win_door = {level_width_tiles-1.0f*wall_thick-2, level_height_tiles-1.0f*wall_thick-2};

    // has_torch = true;
    // torch_light = {(level_width_tiles-wall_thick-1.0f)*wall_thick, 2.0f*wall_thick};
    // torch_up = LEVEL_UP::UP;

    sliding_plats.push_back({
        {2*TILE_SIZE, 2*TILE_SIZE},
        {2*TILE_SIZE, 2*TILE_SIZE},
        {(level_width_tiles-2)*TILE_SIZE, 2*TILE_SIZE},
        3*TILE_SIZE,
        {0, 0}
    });
    // sliding_plats.push_back({
    //     {3*TILE_SIZE, 5*TILE_SIZE},
    //     {2*TILE_SIZE, 5*TILE_SIZE},
    //     {6*TILE_SIZE, 5*TILE_SIZE},
    //     2*TILE_SIZE,
    //     {0, 0}
    // });
}

void LevelSimulate::load_level_8()
{
    level_hint = "The last level. Time to get serious!";
    level_width_tiles = 13+2*wall_thick;
    level_height_tiles = 10+2*wall_thick;
    add_walls();
    player.pos = {2*TILE_SIZE, level_height_tiles/2.0f*TILE_SIZE};

    for (float i=wall_thick; i < level_width_tiles-wall_thick; i++) {
        wall_spikes.push_back({{i, (float)wall_thick}, false, DOWN});
        wall_spikes.push_back({{i, level_height_tiles-wall_thick-0.5f}, false, UP});
    }

    // middle platforms
    walls.push_back({(wall_thick+2)*TILE_SIZE, (wall_thick+2)*TILE_SIZE, 2*TILE_SIZE, TILE_SIZE});
    walls.push_back({(wall_thick+5)*TILE_SIZE, (wall_thick+2)*TILE_SIZE, (level_width_tiles-2*wall_thick-7)*TILE_SIZE, TILE_SIZE});
    
    walls.push_back({(wall_thick+2)*TILE_SIZE, (level_height_tiles-wall_thick-3)*TILE_SIZE, 6*TILE_SIZE, TILE_SIZE});
    walls.push_back({(wall_thick+9)*TILE_SIZE, (level_height_tiles-wall_thick-3)*TILE_SIZE, (level_width_tiles-2*wall_thick-11)*TILE_SIZE, TILE_SIZE});
    
    // spikes on them
    for (float i=wall_thick+2; i < level_width_tiles-3; i++) {
        if(i != wall_thick+4) {
            wall_spikes.push_back({{i, (float)wall_thick+3}, false, DOWN});
        }
        if(i != wall_thick+8) {
            wall_spikes.push_back({{i, (level_height_tiles-wall_thick-3.5f)}, false, UP});
        }
        if(i != wall_thick+4 && i != wall_thick+8)
        {
            wall_spikes.push_back({{i, (level_height_tiles-wall_thick-2.0f)}, false, DOWN});
            wall_spikes.push_back({{i, wall_thick+1.5f}, false, UP});
        }
    }

    // behind player
    wall_spikes.push_back({{(float)wall_thick, level_height_tiles/2.0f}, true, RIGHT});
    wall_spikes.push_back({{(float)wall_thick, level_height_tiles/2.0f+1}, true, RIGHT});
    wall_spikes.push_back({{(float)wall_thick, level_height_tiles/2.0f-1}, true, RIGHT});
    wall_spikes.push_back({{(float)wall_thick, level_height_tiles/2.0f+2}, true, RIGHT});
    wall_spikes.push_back({{(float)wall_thick, level_height_tiles/2.0f-2}, true, RIGHT});

    win_door = {level_width_tiles-1.0f*wall_thick-2, level_height_tiles/2.0f - 1};
    win_door_up = LEFT;

    has_torch = true;
    torch_light = {(level_width_tiles-wall_thick-1.0f)*wall_thick, 2.0f*wall_thick};
    torch_up = LEVEL_UP::LEFT;

    sliding_plats.push_back({
        {(wall_thick+4)*TILE_SIZE, wall_thick*TILE_SIZE},
        {(wall_thick+4)*TILE_SIZE, wall_thick*TILE_SIZE},
        {(wall_thick+4)*TILE_SIZE, (wall_thick+6.5f)*TILE_SIZE},
        3*TILE_SIZE,
        {0, 0}
    });
    sliding_plats.push_back({
        {(wall_thick+8)*TILE_SIZE, (wall_thick+3.5f)*TILE_SIZE},
        {(wall_thick+8)*TILE_SIZE, (wall_thick+3.5f)*TILE_SIZE},
        {(wall_thick+8)*TILE_SIZE, (wall_thick+10.0f)*TILE_SIZE},
        3*TILE_SIZE,
        {0, 0}
    });
}

