#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__

#include <stdint.h>
#include "app.h"

/**
  *****************************************************************************
  * EEPROM define address.
  *
  *****************************************************************************
  */
#define EEPROM_START_ADDR           (0X0000)
#define EEPROM_END_ADDR             (0X1000)

// ===============================
// Archery Game (giữ nguyên)
// ===============================
#define EEPROM_SCORE_START_ADDR     (0X0010)
#define EEPROM_SCORE_PLAY_ADDR      (0X00FA)
#define EEPROM_SETTING_START_ADDR   (0X0100)

/******************************************************************************/
/* Archery game */
/******************************************************************************/
typedef struct {
  /* score data */
  uint32_t score_now;
  uint32_t score_1st;
  uint32_t score_2nd;
  uint32_t score_3rd;
} ar_game_score_t;

typedef struct {
  /* setting data */
  bool silent;
  uint8_t num_arrow;
  uint8_t arrow_speed;
  uint8_t meteoroid_speed;
} ar_game_setting_t;

/******************************************************************************/
/* Car game (✅ thêm phần riêng cho Car Game) */
/******************************************************************************/

// ⚙️ Tạo vùng nhớ riêng cho điểm Car Game — nằm sau vùng Archery
#define EEPROM_CAR_SCORE_START_ADDR   (EEPROM_SETTING_START_ADDR + sizeof(ar_game_setting_t))  
#define EEPROM_CAR_SCORE_PLAY_ADDR    (EEPROM_CAR_SCORE_START_ADDR + sizeof(ar_game_score_t))

// Có thể dùng chung cấu trúc điểm
typedef ar_game_score_t car_game_score_t;

#endif //__APP_EEPROM_H__
