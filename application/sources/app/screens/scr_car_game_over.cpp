#include "scr_car_game_over.h"
#include "scr_menu_game.h"
#include "scr_car_game.h"
#include "scr_car_charts.h"   
#include "app_eeprom.h"       

extern uint32_t car_game_score;
static ar_game_score_t car_gamescore;

// ===============================
// View hiển thị Game Over
// ===============================
static void view_scr_car_game_over();

view_dynamic_t dyn_view_item_car_game_over = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_car_game_over
};

view_screen_t scr_car_game_over = {
    &dyn_view_item_car_game_over,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

void view_scr_car_game_over() {
    view_render.fillScreen(WHITE);
    view_render.setTextSize(2);
    view_render.setTextColor(BLACK);
    view_render.setCursor(11, 10);
    view_render.print("GAME OVER");

    view_render.setTextSize(1);
    view_render.setCursor(16, 35);
    view_render.print("Your score:");
    view_render.setCursor(81, 35);
    view_render.print(car_game_score);

    view_render.drawBitmap(10,  48, icon_restart, 15, 15, 0);
    view_render.drawBitmap(55,  50, icon_charts,  17, 15, 0);
    view_render.drawBitmap(100, 48, icon_go_home, 16, 16, 0);
}

// ===============================
// Xếp hạng điểm cao
// ===============================
static void car_rank_ranking() {
    if (car_game_score > car_gamescore.score_1st) {
        car_gamescore.score_3rd = car_gamescore.score_2nd;
        car_gamescore.score_2nd = car_gamescore.score_1st;
        car_gamescore.score_1st = car_game_score;
    }
    else if (car_game_score > car_gamescore.score_2nd) {
        car_gamescore.score_3rd = car_gamescore.score_2nd;
        car_gamescore.score_2nd = car_game_score;
    }
    else if (car_game_score > car_gamescore.score_3rd) {
        car_gamescore.score_3rd = car_game_score;
    }
}

// ===============================
// Handle – Car Game Over
// ===============================
void scr_car_game_over_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY:
        view_render.initialize();
        view_render_display_on();

        // đọc vùng EEPROM của Car Game
        eeprom_read(EEPROM_CAR_SCORE_START_ADDR,
                    (uint8_t*)&car_gamescore,
                    sizeof(car_gamescore));

        car_rank_ranking();

        // ghi lại dữ liệu sau khi xếp hạng
        eeprom_write(EEPROM_CAR_SCORE_START_ADDR,
                     (uint8_t*)&car_gamescore,
                     sizeof(car_gamescore));

        view_scr_car_game_over();
        break;

    // MODE → Menu
    case AC_DISPLAY_BUTTON_MODE_RELEASED:
        SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
        BUZZER_PlayTones(tones_cc);
        break;

    // UP → Mở Car Chart
    case AC_DISPLAY_BUTTON_UP_RELEASED:
        SCREEN_TRAN(scr_car_charts_handle, &scr_car_charts);   // ✅ FIX 3: mở chart Car
        BUZZER_PlayTones(tones_cc);
        break;

    // DOWN → Restart Car Game
    case AC_DISPLAY_BUTTON_DOWN_RELEASED:
        SCREEN_TRAN(scr_car_game_handle, &scr_car_game);
        BUZZER_PlayTones(tones_cc);
        break;

    default:
        break;
    }
}
