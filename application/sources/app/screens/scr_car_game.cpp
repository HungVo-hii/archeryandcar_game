#include "scr_car_game.h"
#include "screens.h"   // để SCREEN_TRAN(...)


/* =========================================================
 *  Biến trạng thái Car Game
 * =======================================================*/
uint8_t  car_game_state = GAME_OFF;
uint32_t car_game_score = 0;

/* Định nghĩa kích thước xe khic dùng bitmap */
#ifndef SIZE_BITMAP_CAR_X
#define SIZE_BITMAP_CAR_X  24
#endif
#ifndef SIZE_BITMAP_CAR_Y
#define SIZE_BITMAP_CAR_Y  16
#endif

/* Toạ độ 3 làn để vẽ gợi ý (phối hợp với cg_game_car.cpp: {12,30,48}) */
#define LANE_Y_TOP   (12)
#define LANE_Y_MID   (30)
#define LANE_Y_BOT   (48)
///////

/* =========================================================
 *  View helpers
 * =======================================================*/
static inline void car_game_frame_display() {
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

    // viền khung và thanh HUD phía dưới
    view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
    view_render.drawLine(0, LCD_HEIGHT-10, LCD_WIDTH, LCD_HEIGHT-10, WHITE);

    // score
    view_render.setCursor(4, LCD_HEIGHT-8);
    view_render.print("Score:");
    view_render.print(car_game_score);
}

static inline void car_game_lanes_display() {
    // vẽ dấu chấm gợi ý lane (đường ngang đứt đoạn)
    for (uint8_t x = 6; x < LCD_WIDTH-6; x += 8) {
        view_render.drawPixel(x,     LANE_Y_TOP + (SIZE_BITMAP_CAR_Y/2), WHITE);
        view_render.drawPixel(x+3,   LANE_Y_TOP + (SIZE_BITMAP_CAR_Y/2), WHITE);

        view_render.drawPixel(x,     LANE_Y_MID + (SIZE_BITMAP_CAR_Y/2), WHITE);
        view_render.drawPixel(x+3,   LANE_Y_MID + (SIZE_BITMAP_CAR_Y/2), WHITE);

        view_render.drawPixel(x,     LANE_Y_BOT + (SIZE_BITMAP_CAR_Y/2), WHITE);
        view_render.drawPixel(x+3,   LANE_Y_BOT + (SIZE_BITMAP_CAR_Y/2), WHITE);
    }
}

static inline void car_game_car_display() {
    if (car.visible == WHITE) {
        view_render.drawBitmap(car.x, car.y, bitmap_car, SIZE_BITMAP_CAR_X, SIZE_BITMAP_CAR_Y, WHITE);
    }
}

/* =========================================================
 *  View screen (dynamic)
 * =======================================================*/
static void view_scr_car_game();

view_dynamic_t dyn_view_item_car_game = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_car_game
};

view_screen_t scr_car_game = {
    &dyn_view_item_car_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_car_game() {
    if (car_game_state == GAME_PLAY) {
        car_game_frame_display();
        car_game_lanes_display();
        car_game_car_display();

        // ===== OBSTACLE DRAW =====
        cg_game_obstacle_draw();   // ✅ thêm: vẽ obstacle
    }
    else if (car_game_state == GAME_OVER) {
        view_render.clear();
        view_render.setTextSize(2);
        view_render.setTextColor(WHITE);
        view_render.setCursor(16, 22);
        view_render.print("GAME OVER");
        view_render.setTextSize(1);
        view_render.setCursor(28, 44);
        view_render.print("Score: ");
        view_render.print(car_game_score);
    }
}

/* =========================================================
 *  Screen helpers
 * =======================================================*/
static inline void car_game_time_tick_start() {
    timer_set(AC_TASK_DISPLAY_ID, CAR_GAME_TIME_TICK,
              CAR_GAME_TIME_TICK_INTERVAL, TIMER_PERIODIC);
}

static inline void car_game_time_tick_stop() {
#if defined(timer_remove_attr)
    timer_remove_attr(AC_TASK_DISPLAY_ID, CAR_GAME_TIME_TICK);
#else
    timer_remove_attr(AC_TASK_DISPLAY_ID, CAR_GAME_TIME_TICK);
#endif
}

/* =========================================================
 *  Handle - Car game screen
 * =======================================================*/
void scr_car_game_handle(ak_msg_t* msg) {

    switch (msg->sig) {
    /* ===== Khi vào màn hình ===== */
    case SCREEN_ENTRY: {
        APP_DBG_SIG("SCREEN_ENTRY (Car Game)\n");
        view_render.initialize();
        view_render_display_on();
    
        // setup car object
        task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_SETUP);
    
        // start frame tick
        car_game_time_tick_start();
    
        // reset score/state
        car_game_score = 0;
        car_game_state = GAME_PLAY;
    
        //  setup obstacle SAU KHI car đã setup & game đã sẵn sàng
        task_post_pure_msg(CAR_GAME_SCREEN_ID, CAR_GAME_OBSTACLE_SETUP_SIG);
    } break;

    /* ===== Tick mỗi frame ===== */
    case CAR_GAME_TIME_TICK: {
        // update car object (cooldown/anim)
        task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_UPDATE);

        // update obstacle
        task_post_pure_msg(CAR_GAME_SCREEN_ID, CAR_GAME_OBSTACLE_UPDATE_SIG);

      
    } break;

    /* ===== OBSTACLE SIGNALS ===== */
    case CAR_GAME_OBSTACLE_SETUP_SIG:
        cg_game_obstacle_setup();
        break;

    case CAR_GAME_OBSTACLE_UPDATE_SIG:
        cg_game_obstacle_update();
        break;

    case CAR_GAME_OBSTACLE_DRAW_SIG:
        cg_game_obstacle_draw();
        break;

    case CAR_GAME_CAR_COLLISION_SIG: {
        APP_DBG_SIG("[Car Game] Collision detected -> Game Over\n");
        car_game_time_tick_stop();
        car_game_state = GAME_OVER;
        SCREEN_TRAN(scr_car_game_over_handle, &scr_car_game_over);
    } break;

    /* ===== Nút điều khiển ===== */
    case AC_DISPLAY_BUTTON_UP_RELEASED: {
        if (car_game_state == GAME_PLAY) {
            task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_MOVE_LEFT);
            APP_DBG_SIG("UP -> car move UP\n");
        }
    } break;

    case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
        if (car_game_state == GAME_PLAY) {
            task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_MOVE_RIGHT);
            APP_DBG_SIG("DOWN -> car move DOWN\n");
        }
    } break;

    case AC_DISPLAY_BUTTON_MODE_RELEASED: {
        APP_DBG_SIG("MODE -> ignored (no action)\n");
    } break;

    /* ===== Reset/Exit flow ===== */
    case CAR_GAME_RESET: {
        APP_DBG_SIG("CAR_GAME_RESET\n");
        task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_RESET);
        car_game_time_tick_stop();

        timer_set(AC_TASK_DISPLAY_ID, CAR_GAME_EXIT_GAME,
                  CAR_GAME_TIME_EXIT_INTERVAL, TIMER_ONE_SHOT);

        car_game_state = GAME_OVER;
    } break;

    case CAR_GAME_EXIT_GAME: {
        APP_DBG_SIG("CAR_GAME_EXIT_GAME\n");
        car_game_state = GAME_OFF;
        SCREEN_TRAN(scr_car_game_over_handle, &scr_car_game_over);
    } break;

    default:
        break;
    }
}
