#ifndef __SCR_CAR_GAME_H__
#define __SCR_CAR_GAME_H__

/**
 * @file scr_car_game.h
 * @brief Màn hình Car Game (event-driven, tương tự Archery Game)
 */

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "cg_game_car.h"       
#include "cg_game_obstacle.h"  
#include "car_game_signal.h"  

/* -------------------------------------------------------------------------- */
/* Định nghĩa trạng thái game                         */
/* -------------------------------------------------------------------------- */
#ifndef GAME_OFF
#define GAME_OFF   (0)
#endif

#ifndef GAME_PLAY
#define GAME_PLAY  (1)
#endif

#ifndef GAME_OVER
#define GAME_OVER  (2)
#endif

/* -------------------------------------------------------------------------- */
/* Biến trạng thái toàn cục                                                  */
/* -------------------------------------------------------------------------- */
extern uint8_t  car_game_state;
extern uint32_t car_game_score;

/* -------------------------------------------------------------------------- */
/* Khai báo view/screen                                                      */
/* -------------------------------------------------------------------------- */
extern view_dynamic_t dyn_view_item_car_game;
extern view_screen_t  scr_car_game;

/* -------------------------------------------------------------------------- */
/* Hàm xử lý tín hiệu cho screen                                             */
/* -------------------------------------------------------------------------- */
void scr_car_game_handle(ak_msg_t* msg);

#endif /* __SCR_CAR_GAME_H__ */
