#ifndef __CAR_GAME_SIGNAL_H__
#define __CAR_GAME_SIGNAL_H__

// =============================================================================
// GAME CONTROL SIGNALS
// =============================================================================
#define CAR_GAME_START_SIG            (0x50)
#define CAR_GAME_UPDATE_SIG           (0x51)
#define CAR_GAME_PAUSE_SIG            (0x52)
#define CAR_GAME_GAME_OVER_SIG        (0x53)
#define CAR_GAME_EXIT_SIG             (0x54)

// =============================================================================
// TIMER SIGNALS
// =============================================================================
#define CAR_GAME_TIME_TICK            (0x55)
#define CAR_GAME_TIME_TICK_INTERVAL   (16)      // 60fps = 16ms
#define CAR_GAME_EXIT_GAME            (0x56)
#define CAR_GAME_TIME_EXIT_INTERVAL   (2000)    // 2 seconds
#define CAR_GAME_RESET                (0x57)

// =============================================================================
// CAR SIGNALS
// =============================================================================
#define CAR_GAME_CAR_SETUP            (0x60)
#define CAR_GAME_CAR_RESET            (0x61)
#define CAR_GAME_CAR_UPDATE           (0x62)
#define CAR_GAME_CAR_MOVE_LEFT        (0x63)
#define CAR_GAME_CAR_MOVE_RIGHT       (0x64)
#define CAR_GAME_CAR_COLLISION_SIG    (0x65)    // gửi khi xe va chạm

// =============================================================================
// OBSTACLE SIGNALS
// =============================================================================
#define CAR_GAME_OBSTACLE_SETUP_SIG   (0x70)
#define CAR_GAME_OBSTACLE_UPDATE_SIG  (0x71)
#define CAR_GAME_OBSTACLE_DRAW_SIG    (0x72)

// =============================================================================
// PLAYER INPUT (tùy chọn)
 // =============================================================================
#define CAR_GAME_MOVE_LEFT_SIG        (0x80)
#define CAR_GAME_MOVE_RIGHT_SIG       (0x81)

// =============================================================================
// GAME EVENTS
// =============================================================================
#define CAR_GAME_COLLISION_SIG        (0x90)
#define CAR_GAME_SPAWN_OBSTACLE_SIG   (0x91)
#define CAR_GAME_SCORE_UPDATE_SIG     (0x92)

#endif // __CAR_GAME_SIGNAL_H__
