#ifndef __CG_GAME_CAR_H__
#define __CG_GAME_CAR_H__

#include <stdint.h>
#include "ak.h"

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
#define SIZE_BITMAP_CAR_X   24   // chiều ngang bitmap xe (pixel)
#define SIZE_BITMAP_CAR_Y   16   // chiều cao bitmap xe (pixel)

// -----------------------------------------------------------------------------
// Car structure
// -----------------------------------------------------------------------------
typedef struct {
    int16_t x;         // Vị trí X (cố định)
    int16_t y;         // Vị trí Y (thay đổi theo làn đường)
    uint8_t visible;   // 0 = ẩn, 1 = hiện (hoặc dùng để collision)
} car_t;

// -----------------------------------------------------------------------------
// Global car instance (defined in cg_game_car.cpp)
// -----------------------------------------------------------------------------
extern car_t car;

// -----------------------------------------------------------------------------
// Task handler
// -----------------------------------------------------------------------------
void cg_game_car_handle(ak_msg_t* msg);

#endif  // __CG_GAME_CAR_H__