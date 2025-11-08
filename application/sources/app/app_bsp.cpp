#include "button.h"

#include "sys_dbg.h"
#include "app.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "app_if.h"
#include "task_list.h"

#include "scr_archery_game.h"
#include "scr_car_game.h"  // ✅ Car Game

/* =========================================================
 *  Khai báo các nút vật lý
 * =======================================================*/
button_t btn_mode;
button_t btn_up;
button_t btn_down;

/* =========================================================
 *  NÚT MODE (phải)
 * =======================================================*/
void btn_mode_callback(void* b) {
    button_t* me_b = (button_t*)b;
    switch (me_b->state) {
    case BUTTON_SW_STATE_PRESSED: {
        APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_PRESSED\n");
    } break;

    case BUTTON_SW_STATE_LONG_PRESSED: {
        APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_MODE_LONG_PRESSED);
    } break;

    case BUTTON_SW_STATE_RELEASED: {
        APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_RELEASED\n");

        // Archery game: MODE = bắn cung
        if (ar_game_state != GAME_OFF) {
            task_post_pure_msg(AR_GAME_ARROW_ID, AR_GAME_ARROW_SHOOT);
            APP_DBG("[btn_mode_callback] Archery SHOOT\n");
        }
        // Car game
        else if (car_game_state != GAME_OFF) {
            if (car_game_state == GAME_PLAY) {
                APP_DBG("[btn_mode_callback] MODE ignored in Car Game PLAY\n");
            }
            else if (car_game_state == GAME_OVER) {
                // ✅ Trong Game Over → gửi tín hiệu về display
                task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_MODE_RELEASED);
                APP_DBG("[btn_mode_callback] MODE in Car Game OVER -> to display\n");
            }
        }
        // Menu / Idle
        else {
            task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_MODE_RELEASED);
        }
    } break;

    default:
        break;
    }
}

/* =========================================================
 *  NÚT UP (giữa)
 * =======================================================*/
void btn_up_callback(void* b) {
    button_t* me_b = (button_t*)b;
    switch (me_b->state) {
    case BUTTON_SW_STATE_PRESSED: {
        APP_DBG("[btn_up_callback] BUTTON_SW_STATE_PRESSED\n");
    } break;

    case BUTTON_SW_STATE_LONG_PRESSED: {
        APP_DBG("[btn_up_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_UP_LONG_PRESSED);
    } break;

    case BUTTON_SW_STATE_RELEASED: {
        APP_DBG("[btn_up_callback] BUTTON_SW_STATE_RELEASED\n");

        // Archery game: UP = di chuyển cung lên
        if (ar_game_state != GAME_OFF) {
            task_post_pure_msg(AR_GAME_ARCHERY_ID, AR_GAME_ARCHERY_UP);
            APP_DBG("[btn_up_callback] Archery UP\n");
        }
        // Car game
        else if (car_game_state != GAME_OFF) {
            if (car_game_state == GAME_PLAY) {
                // ✅ Khi đang chơi → điều khiển xe lên
                task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_MOVE_LEFT);
                APP_DBG("[btn_up_callback] Car move UP\n");
            }
            else if (car_game_state == GAME_OVER) {
                // ✅ Khi Game Over → gửi tín hiệu về display
                task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_UP_RELEASED);
                APP_DBG("[btn_up_callback] UP in Car Game OVER -> to display\n");
            }
        }
        // Menu / Idle
        else {
            task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_UP_RELEASED);
        }
    } break;

    default:
        break;
    }
}

/* =========================================================
 *  NÚT DOWN (trái)
 * =======================================================*/
void btn_down_callback(void* b) {
    button_t* me_b = (button_t*)b;
    switch (me_b->state) {
    case BUTTON_SW_STATE_PRESSED: {
        APP_DBG("[btn_down_callback] BUTTON_SW_STATE_PRESSED\n");
    } break;

    case BUTTON_SW_STATE_LONG_PRESSED: {
        APP_DBG("[btn_down_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED);
    } break;

    case BUTTON_SW_STATE_RELEASED: {
        APP_DBG("[btn_down_callback] BUTTON_SW_STATE_RELEASED\n");

        // Archery game: DOWN = di chuyển cung xuống
        if (ar_game_state != GAME_OFF) {
            task_post_pure_msg(AR_GAME_ARCHERY_ID, AR_GAME_ARCHERY_DOWN);
            APP_DBG("[btn_down_callback] Archery DOWN\n");
        }
        // Car game
        else if (car_game_state != GAME_OFF) {
            if (car_game_state == GAME_PLAY) {
                // ✅ Khi đang chơi → xe đi xuống
                task_post_pure_msg(CAR_GAME_CAR_ID, CAR_GAME_CAR_MOVE_RIGHT);
                APP_DBG("[btn_down_callback] Car move DOWN\n");
            }
            else if (car_game_state == GAME_OVER) {
                // ✅ Khi Game Over → gửi tín hiệu về display
                task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_DOWN_RELEASED);
                APP_DBG("[btn_down_callback] DOWN in Car Game OVER -> to display\n");
            }
        }
        // Menu / Idle
        else {
            task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTTON_DOWN_RELEASED);
        }
    } break;

    default:
        break;
    }
}
