#include "cg_game_obstacle.h"
#include "app_dbg.h"
#include "ak.h"
#include "task_list.h"
#include "app.h"
#include "screens_bitmap.h"
#include "view_render.h"
#include "car_game_signal.h"
#include "scr_car_game.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// ======================================================================
// ⚙️ Cấu hình màn hình & sprite
// ======================================================================
#define SCREEN_WIDTH                128
#ifndef SIZE_BITMAP_OBSTACLE_X
#define SIZE_BITMAP_OBSTACLE_X      18
#endif
#ifndef SIZE_BITMAP_OBSTACLE_Y
#define SIZE_BITMAP_OBSTACLE_Y      18
#endif
#ifndef CAR_WIDTH
#define CAR_WIDTH                   18
#endif

// ======================================================================
// Lane và tốc độ
// ======================================================================
static const int16_t lane_center_y[3] = { 12, 32, 48 };
#define OBSTACLE_COUNT 3
static cg_game_obstacle_t obstacles[OBSTACLE_COUNT];
static int16_t obstacle_speed = 2;   // tốc độ ban đầu

// ======================================================================
// 🌐 Biến ngoại từ module xe
// ======================================================================
extern int16_t car_x;
extern uint8_t car_lane;
extern uint32_t car_game_score;

// ======================================================================
// 🔧 Hàm tiện ích
// ======================================================================
static inline int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

static inline int16_t lane_top_left_y(uint8_t lane) {
    return (int16_t)(lane_center_y[lane] - (SIZE_BITMAP_OBSTACLE_Y / 2));
}

// ======================================================================
// 🚀 Khởi tạo
// ======================================================================
void cg_game_obstacle_init(void) {
    for (uint8_t i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].active = 0;
        obstacles[i].lane   = i;
        obstacles[i].x      = SCREEN_WIDTH;
        obstacles[i].y      = lane_top_left_y(i);
        obstacles[i].counted = false;
    }
}

// ======================================================================
// 🎯 Setup obstacle: random 1 hoặc 2 vật cản ban đầu
// ======================================================================
void cg_game_obstacle_setup(void) {
    // Reset lại tốc độ mỗi lần bắt đầu game mới
    obstacle_speed = 2;

    // 1 hoặc 2 obstacle ngẫu nhiên mỗi lần spawn ban đầu
    uint8_t obstacle_count_this_round = random_range(1, 2);

    // Reset toàn bộ
    for (uint8_t i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].active = 0;
        obstacles[i].counted = false;
        obstacles[i].type = 0;   // reset type cũ
    }

    // Random lane không trùng nhau
    uint8_t lanes_used[3] = {0, 0, 0};
    for (uint8_t i = 0; i < obstacle_count_this_round; i++) {
        uint8_t lane;
        do {
            lane = random_range(0, 2);
        } while (lanes_used[lane]);  // tránh trùng lane
        lanes_used[lane] = 1;

        // Gán giá trị cho obstacle
        obstacles[lane].active  = 1;
        obstacles[lane].counted = false;
        obstacles[lane].lane    = lane;
        obstacles[lane].x       = SCREEN_WIDTH + random_range(32, 64);
        obstacles[lane].y       = lane_top_left_y(lane);

        // ✅ Random loại obstacle (và ép 1 cái chắc chắn type=1 để test)
        if (i == 0) {
            obstacles[lane].type = 1;  // ép obstacle đầu tiên là loại 2 để bạn thấy rõ hình
        } else {
            obstacles[lane].type = rand() % 2;
        }

        APP_DBG("[SETUP] lane=%d, type=%d\n", lane, obstacles[lane].type);
    }
}

// ======================================================================
// Update obstacle (endless mode + tính điểm + tăng tốc)
// ======================================================================
void cg_game_obstacle_update(void) {
    // Đếm số obstacle đang còn hoạt động
    uint8_t active_count = 0;

    // Cập nhật vị trí và kiểm tra va chạm 
    for (uint8_t i = 0; i < OBSTACLE_COUNT; i++) {
        cg_game_obstacle_t* ob = &obstacles[i];
        if (!ob->active) continue;

        active_count++;

        // Tăng tốc dựa theo điểm số
        if (car_game_score > 20)  obstacle_speed = 3;
        if (car_game_score > 50)  obstacle_speed = 4;
        if (car_game_score > 100) obstacle_speed = 5;
        if (car_game_score > 200) obstacle_speed = 6;

        // Di chuyển obstacle
        ob->x -= obstacle_speed;
    // =======================================================
    //  Nếu obstacle là loại lửa → tự nhảy giữa các lane
    // =======================================================
    if (ob->type == 1) {
        static uint16_t frame_counter[OBSTACLE_COUNT] = {0};
        static int16_t target_y[OBSTACLE_COUNT] = {0};
        frame_counter[i]++;
    
        // Nếu chưa có target, gán theo lane hiện tại
        if (target_y[i] == 0) {
            target_y[i] = ob->y;
        }
    
        // Mỗi 80 frame (~1.3s ở 60FPS) chọn lane mới
        if (frame_counter[i] > 80) {
            frame_counter[i] = 0;
    
            int8_t new_lane;
            do {
                new_lane = random_range(0, 2);
            } while (new_lane == ob->lane);  // tránh trùng lane
    
            ob->lane = new_lane;
            target_y[i] = lane_top_left_y(new_lane);  // đặt vị trí đích
        }
    
        // --------------------------------------------------
        // 🎞 Animation di chuyển mượt giữa lane cũ và mới
        // --------------------------------------------------
        int16_t dy = target_y[i] - ob->y;
    
        if (abs(dy) > 1) {
            ob->y += (dy > 0) ? 1 : -1;   // ✅ di chuyển 1px mỗi frame
        } else {
            ob->y = target_y[i];          // đến nơi → dừng lại
        }
    }
 // --------------------------------------------------
// Tránh 2 obstacle chồng lên nhau  :)) 
// --------------------------------------------------
for (uint8_t j = 0; j < OBSTACLE_COUNT; j++) {
    if (j == i) continue;
    if (!obstacles[j].active) continue;

    int dx = abs(ob->x - obstacles[j].x);
    int dy2 = abs(ob->y - obstacles[j].y);

    // Nếu 2 obstacle gần nhau cả theo X và Y → né sang bên
    if (dx < 20 && dy2 < 6) {
        if (ob->x < obstacles[j].x) {
            ob->x -= 1;  // dịch sang trái 2 pixel
        } else {
            ob->x += 1;  // dịch sang phải 2 pixel
        }
    }
}

        // Nếu obstacle vừa vượt qua xe mà chưa tính điểm
        if (!ob->counted && (ob->x + SIZE_BITMAP_OBSTACLE_X) < car_x) {
            car_game_score += 2;
            ob->counted = true;
            APP_DBG("[SCORE] +2 (total: %d)\n", car_game_score);
        }

        //  Nếu obstacle ra khỏi màn hình -> đánh dấu hết hiệu lực
        if ((ob->x + SIZE_BITMAP_OBSTACLE_X) < 0) {
            ob->active = 0;
            ob->counted = false;
        }
// =====================================================
// KIỂM TRA VA CHẠM 2D + EDGE COLLISION GIỮA CÁC LANE
// =====================================================
// Kiểm tra va chạm (chuẩn 2D – tránh chết sớm và phát hiện giao giữa lane)
{
    const int16_t ob_left   = ob->x;
    const int16_t ob_right  = ob->x + SIZE_BITMAP_OBSTACLE_X;
    const int16_t car_left  = car_x;
    const int16_t car_right = car_x + CAR_WIDTH;

    const int16_t margin_x = 8;   // ✅ giảm hitbox ngang để không chết sớm
    bool overlap_x = ((ob_left + margin_x) <= (car_right - margin_x)) &&
                     ((ob_right - margin_x) >= (car_left + margin_x));

    // --- Kiểm tra trục Y (cho phép chạm nhẹ khi đang chuyển lane) ---
    int16_t car_top    = car.y;
    int16_t car_bottom = car.y + SIZE_BITMAP_CAR_Y;
    int16_t ob_top     = ob->y;
    int16_t ob_bottom  = ob->y + SIZE_BITMAP_OBSTACLE_Y;

    const int16_t margin_y = 5;   // ✅ vùng đệm dọc
    bool overlap_y = !(car_bottom < (ob_top + margin_y) ||
                       car_top > (ob_bottom - margin_y));

    // ✅ Nếu cả 2 trục đều overlap → thật sự va chạm
    if (overlap_x && overlap_y) {
        APP_DBG("[Obstacle] Collision (2D overlap)!\n");
        task_post_pure_msg(AC_TASK_DISPLAY_ID, CAR_GAME_CAR_COLLISION_SIG);
        return;
    }
    static uint8_t prev_lane = 1;
if (car_lane != prev_lane) {
    if ((ob->lane == car_lane) || (ob->lane == prev_lane)) {
        if (overlap_x) {
            // Kiểm tra nếu xe đang ở gần mép của obstacle
            if ((car.y + SIZE_BITMAP_CAR_Y + 2) > ob->y && 
                car.y < (ob->y + SIZE_BITMAP_OBSTACLE_Y + 2)) {
                APP_DBG("[Collision] Lane-switch overlap!\n");
                task_post_pure_msg(AC_TASK_DISPLAY_ID, CAR_GAME_CAR_COLLISION_SIG);
                prev_lane = car_lane;
                return;
            }
        }
    }
}
prev_lane = car_lane;
}


    }

    // --- 2. Nếu KHÔNG còn obstacle nào active => spawn 1–2 cái cùng lúc ---
    if (active_count == 0) {
        uint8_t count_new = random_range(1, 2);
        uint8_t lanes_used[3] = {0, 0, 0};

        for (uint8_t j = 0; j < count_new; j++) {
            uint8_t lane;
            do {
                lane = random_range(0, 2);
            } while (lanes_used[lane]); // tránh trùng lane
            lanes_used[lane] = 1;

            obstacles[lane].active  = 1;
            obstacles[lane].counted = false;
            obstacles[lane].lane    = lane;
            obstacles[lane].x       = SCREEN_WIDTH + random_range(0, 30); // vị trí bắt đầu gần nhau
            obstacles[lane].y       = lane_top_left_y(lane);
        }
    }
}
// ======================================================================
// 🖼️ Vẽ obstacle
// ======================================================================
void cg_game_obstacle_draw(void) {
    for (uint8_t i = 0; i < OBSTACLE_COUNT; i++) {
        if (!obstacles[i].active) continue;

        const unsigned char* sprite = bitmap_obstacle;
        if (obstacles[i].type == 1) {
        sprite = bitmap_obstacle2;  // ✅ obstacle nhảy lane
}

        view_render.drawBitmap(
        obstacles[i].x,
        obstacles[i].y,
        sprite,
        SIZE_BITMAP_OBSTACLE_X,
        SIZE_BITMAP_OBSTACLE_Y,
        WHITE
        );
    }
}
