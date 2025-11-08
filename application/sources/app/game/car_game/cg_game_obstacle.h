#ifndef __CG_GAME_OBSTACLE_H__
#define __CG_GAME_OBSTACLE_H__

#include <stdint.h>
#include "ak.h"

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
#define SIZE_BITMAP_OBSTACLE_X   18    // chiều ngang bitmap vật cản (pixel)
#define SIZE_BITMAP_OBSTACLE_Y   18    // chiều cao bitmap vật cản (pixel)

// -----------------------------------------------------------------------------
// Obstacle structure
// -----------------------------------------------------------------------------
typedef struct {
    int16_t x;         // Vị trí X (tọa độ top-left)
    int16_t y;         // Vị trí Y (tọa độ top-left)
    uint8_t lane;      
    uint8_t active;    
    uint8_t type;
    bool counted;       //  cộng điểm  
} cg_game_obstacle_t;

// -----------------------------------------------------------------------------
// Global obstacle instance (defined in cg_game_obstacle.cpp)
// -----------------------------------------------------------------------------
extern cg_game_obstacle_t obstacle;

// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
void cg_game_obstacle_init(void);
void cg_game_obstacle_setup(void);
void cg_game_obstacle_update(void);
void cg_game_obstacle_draw(void);

#endif  // __CG_GAME_OBSTACLE_H__
