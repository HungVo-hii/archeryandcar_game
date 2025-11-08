// ========== car_game_types.h ==========
#ifndef __CAR_GAME_TYPES_H__
#define __CAR_GAME_TYPES_H__

#include <stdint.h>

// Tất cả struct ở đây!
typedef struct {
    int16_t x;
    int16_t y;
    uint8_t lane;
    uint8_t lives;
} car_t;

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t lane;
    uint8_t active;
} obstacle_t;

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t active;
    uint8_t frame;
} explosion_t;

typedef enum {
    GAME_STATE_INIT,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
} game_state_t;

#endif