//
// Created by saber on 2/13/2026.
//

#ifndef HEW_GAME_H
#define HEW_GAME_H

#define TILE_SIZE 32
#define TARGET_FPS 60
#define FRAME_TIME (1.0f / TARGET_FPS)

struct GameConfig
{
    static int VIEW_WIDTH;
    static int VIEW_HEIGHT;
    static int FONT_WIDTH;
    static int FONT_HEIGHT;
};

#endif //HEW_GAME_H