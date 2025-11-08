#include "scr_car_charts.h"

/*****************************************************************************/
/* Variable Declaration - Car Charts */
/*****************************************************************************/
static ar_game_score_t car_gamescore_charts;

/*****************************************************************************/
/* View - Car Charts */
/*****************************************************************************/
static void view_scr_car_charts();

view_dynamic_t dyn_view_item_car_charts = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_car_charts
};

view_screen_t scr_car_charts = {
	&dyn_view_item_car_charts,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_car_charts() {
	view_render.clear();
	view_render.fillScreen(WHITE);
	view_render.drawBitmap(35, 5, bitmap_icon_charts, 60, 20, 0);	// biểu tượng giống archery
	view_render.fillRoundRect(1, 28, 126, 12, 5, 0);
	view_render.fillRoundRect(1, 51, 126, 12, 5, 0);

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(16, 30);
	view_render.print(car_gamescore_charts.score_1st);
	view_render.setCursor(86, 30);
	view_render.print(": 1st");

	view_render.setCursor(16, 54);
	view_render.print(car_gamescore_charts.score_3rd);
	view_render.setCursor(86, 54);
	view_render.print(": 3rd");

	view_render.setTextColor(BLACK);
	view_render.setCursor(16, 42);
	view_render.print(car_gamescore_charts.score_2nd);
	view_render.setCursor(86, 42);
	view_render.print(": 2nd");
}

/*****************************************************************************/
/* Handle - Car Charts */
/*****************************************************************************/
void scr_car_charts_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY (Car Charts)\n");
		view_render.initialize();
		view_render_display_on();

		//Đọc điểm Car Game từ EEPROM
		eeprom_read(EEPROM_CAR_SCORE_START_ADDR,
					(uint8_t*)&car_gamescore_charts,
					sizeof(car_gamescore_charts));
    //Kiểm tra lần đầu (EEPROM rỗng -> toàn 0xFFFFFFFF)
    if (car_gamescore_charts.score_1st == 0xFFFFFFFF ||
        car_gamescore_charts.score_2nd == 0xFFFFFFFF ||
        car_gamescore_charts.score_3rd == 0xFFFFFFFF) {

        APP_DBG("[Car Chart] First-time init, clearing EEPROM\n");
        car_gamescore_charts.score_1st = 0;
        car_gamescore_charts.score_2nd = 0;
        car_gamescore_charts.score_3rd = 0;

        eeprom_write(EEPROM_CAR_SCORE_START_ADDR,
                     (uint8_t*)&car_gamescore_charts,
                     sizeof(car_gamescore_charts));
    }
		//Gọi hàm hiển thị chart (bạn thiếu dòng này)
		view_scr_car_charts();
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED (Car Charts)\n");
		// Quay về menu
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
	BUZZER_PlayTones(tones_cc);
	break;

	case AC_DISPLAY_BUTTON_UP_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_LONG_PRESSED (Reset Car Chart)\n");
		// ✅ Reset điểm Car Game
		car_gamescore_charts.score_1st = 0;
		car_gamescore_charts.score_2nd = 0;
		car_gamescore_charts.score_3rd = 0;
		eeprom_write(EEPROM_CAR_SCORE_START_ADDR,
					 (uint8_t*)&car_gamescore_charts,
					 sizeof(car_gamescore_charts));

		//Vẽ lại màn hình sau khi reset
		view_scr_car_charts();
	}
	BUZZER_PlayTones(tones_cc);
	break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED (Car Charts)\n");
		// Quay lại menu
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
	BUZZER_PlayTones(tones_cc);
	break;

	default:
		break;
	}
}
