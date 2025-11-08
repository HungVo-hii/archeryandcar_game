#warning "USING FINAL CAR OBJECT CODE (FIXED LANE ALIGNMENT)"

#include "cg_game_car.h"
#include "car_game_signal.h"
#include "task_list.h"
#include "app_dbg.h"
#include "timer.h"
#include "ak.h"

#ifndef WHITE
#define WHITE 1
#endif

// -----------------------------------------------------------------------------
// Cấu hình bitmap (kích thước xe hiển thị)
// -----------------------------------------------------------------------------
#define SIZE_BITMAP_CAR_X   24
#define SIZE_BITMAP_CAR_Y   16

// -----------------------------------------------------------------------------
// Biến toàn cục xe - đối tượng duy nhất trong game
// -----------------------------------------------------------------------------
car_t car = {
    .x = 30,
    .y = 0,
    .visible = WHITE,
};

// ✅ Thêm 2 biến global cho obstacle
int16_t car_x = 30;     // toạ độ X của xe (cố định)
uint8_t car_lane = 1;   // lane hiện tại (0=top,1=mid,2=bot)

// -----------------------------------------------------------------------------
// Cấu hình làn đường và cooldown chuyển làn
// -----------------------------------------------------------------------------

static const int16_t lane_center_y[3] = { 12, 32, 48 };
static int16_t lane_y[3];
static const uint8_t lane_count = 3;

static uint8_t current_lane = 1;                   // Xe bắt đầu ở làn giữa
static uint8_t lane_cooldown = 0;                  // Thời gian chờ đổi làn
static const uint8_t LANE_COOLDOWN_FRAMES = 6;     // 6 frame ~100ms @60fps

// -----------------------------------------------------------------------------
// Hàm nội bộ
// -----------------------------------------------------------------------------

// Đặt xe vào làn tương ứng (vẽ từ top-left nên trừ nửa chiều cao ảnh)
static void car_set_lane(uint8_t lane) {
    if (lane >= lane_count) return;
    current_lane = lane;
    car.y = lane_y[lane];
}

// Reset xe về trạng thái ban đầu
static void car_reset() {
    car.x = 10;
    car.visible = WHITE;
    car_set_lane(1);
    car_lane = current_lane;    // ✅ đồng bộ luôn lane khi reset
    lane_cooldown = 0;
}

// -----------------------------------------------------------------------------
// Hàm xử lý tín hiệu 
// -----------------------------------------------------------------------------
void cg_game_car_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    // Khởi tạo xe khi bắt đầu game
    case CAR_GAME_CAR_SETUP: {
        APP_DBG_SIG("CAR_GAME_CAR_SETUP\n");
        // Tính lại toạ độ lane hiển thị chuẩn theo bitmap
        for (uint8_t i = 0; i < lane_count; i++) {
            lane_y[i] = lane_center_y[i] - SIZE_BITMAP_CAR_Y / 2;
        }
        car_reset();
        car_lane = current_lane;    // ✅ đồng bộ lane ban đầu cho obstacle
    } break;

    // Cập nhật theo frame – giảm cooldown sau mỗi lần update
    case CAR_GAME_CAR_UPDATE: {
        if (lane_cooldown > 0) {
            lane_cooldown--;
        }
          // ======================================================
    // 🟢 Animation chuyển lane – di chuyển mượt từng frame
    // ======================================================
    static int16_t target_y = 0;
    static uint8_t moving = 0;

    if (!moving) {
        
        target_y = lane_y[current_lane];
        moving = 1;
    }

    // Di chuyển dần tới vị trí đích theo từng frame
    if (car.y < target_y) {
        car.y += 2;  // tốc độ xuống
        if (car.y > target_y) car.y = target_y;
    } else if (car.y > target_y) {
        car.y -= 2;  // tốc độ lên
        if (car.y < target_y) car.y = target_y;
    }

    // Dừng lại khi tới nơi
    if (car.y == target_y) {
        moving = 0;
    }
    } break;

    //  Nút UP :)) → chuyển lên làn trên
    case CAR_GAME_CAR_MOVE_LEFT: {
        if (lane_cooldown > 0) break;  

        if (current_lane > 0) {
            current_lane--;
            car_lane = current_lane;   // cập nhật lane thật sự cho obstacle
            lane_cooldown = LANE_COOLDOWN_FRAMES;
            APP_DBG_SIG("CAR MOVE UP -> lane %d\n", current_lane);
        }
    } break;

    // Nút DOWN → chuyển xuống làn dưới
    case CAR_GAME_CAR_MOVE_RIGHT: {
        if (lane_cooldown > 0) break;  

        if (current_lane < lane_count - 1) {
            current_lane++;
            car_lane = current_lane;   
            lane_cooldown = LANE_COOLDOWN_FRAMES;
            APP_DBG_SIG("CAR MOVE DOWN -> lane %d\n", current_lane);
        }
    } break;

    // Reset xe 
    case CAR_GAME_CAR_RESET: {
        APP_DBG_SIG("CAR_GAME_CAR_RESET\n");
        car_reset();
    } break;

    default:
        break;
    }
}
