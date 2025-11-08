#ifndef __SCR_CAR_CHARTS_H__
#define __SCR_CAR_CHARTS_H__

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

#include "buzzer.h"

#include "eeprom.h"
#include "app_eeprom.h"

#include "screens.h"
#include "screens_bitmap.h"

// ============================================================
// Cấu trúc và khai báo cho Car Game Charts
// ============================================================
extern view_dynamic_t dyn_view_item_car_charts;
extern view_screen_t scr_car_charts;
extern void scr_car_charts_handle(ak_msg_t* msg);

#endif //__SCR_CAR_CHARTS_H__
