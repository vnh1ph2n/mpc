#include "input_processing.h"
#include "input_reading.h"
#include "timer.h"
#include "led_display.h"

#define MAX_DURATION 99

static TrafficMode_t current_mode = MODE_1_NORMAL;

/* Thoi gian da duoc SET (giay) */
static uint8_t red_duration   = 5;
static uint8_t amber_duration = 2;
static uint8_t green_duration = 3;

/* Gia tri dang chinh trong mode 2/3/4 */
static uint8_t temp_duration = 1;

/* Dem nguoc va trang thai den: 0 = do, 1 = vang, 2 = xanh */
static uint8_t count_road1 = 5;
static uint8_t count_road2 = 3;
static uint8_t state_road1 = 0;
static uint8_t state_road2 = 2;

/* Giu rang buoc red = amber + green de 2 huong khong bi lech pha.
   Quy uoc: neu red > amber thi green = red - amber, nguoc lai red = amber + green. */
static void normalize_durations(void) {
    if (red_duration > amber_duration) {
        green_duration = red_duration - amber_duration;
    } else {
        red_duration = amber_duration + green_duration;
    }
    if (red_duration > MAX_DURATION) {
        red_duration = MAX_DURATION;
        if (amber_duration >= MAX_DURATION) amber_duration = MAX_DURATION - 1;
        green_duration = red_duration - amber_duration;
    }
    if (green_duration < 1) {
        green_duration = 1;
        amber_duration = red_duration - 1;
    }
}

static void advance_road(uint8_t *state, uint8_t *count) {
    if (*state == 0)      { *state = 2; *count = green_duration; }
    else if (*state == 2) { *state = 1; *count = amber_duration; }
    else                  { *state = 0; *count = red_duration;   }
}

static void increase_temp(void) {
    temp_duration++;
    if (temp_duration > MAX_DURATION) temp_duration = 1;
}

void fsm_for_input_processing(void) {
    switch (current_mode) {

        case MODE_1_NORMAL:
            if (timer2_flag == 1) {
                set_timer2(1000);
                count_road1--;
                count_road2--;

                if (count_road1 == 0) advance_road(&state_road1, &count_road1);
                if (count_road2 == 0) advance_road(&state_road2, &count_road2);
            }

            display_traffic_light_normal(state_road1, state_road2);
            update_7seg_buffer_normal(count_road1, count_road2);

            /* Bo cac su kien Inc/Set bam luc dang o mode 1 de khong bi "an" sang mode 2 */
            is_button_pressed_once(BUTTON_INC_INDEX);
            is_button_pressed_once(BUTTON_SET_INDEX);

            if (is_button_pressed_once(BUTTON_MODE_INDEX)) {
                current_mode = MODE_2_MODIFY_RED;
                temp_duration = red_duration;
                set_timer1(250);
            }
            break;

        case MODE_2_MODIFY_RED:
            if (timer1_flag == 1) {
                set_timer1(250);
                blink_all_red_leds();
            }

            if (is_button_pressed_once(BUTTON_MODE_INDEX)) {
                current_mode = MODE_3_MODIFY_AMBER;
                temp_duration = amber_duration;
                set_timer1(250);
            } else if (is_button_pressed_once(BUTTON_INC_INDEX)) {
                increase_temp();
            } else if (is_button_pressed_once(BUTTON_SET_INDEX)) {
                red_duration = temp_duration;
            }

            update_7seg_buffer_mode(2, temp_duration);
            break;

        case MODE_3_MODIFY_AMBER:
            if (timer1_flag == 1) {
                set_timer1(250);
                blink_all_amber_leds();
            }

            if (is_button_pressed_once(BUTTON_MODE_INDEX)) {
                current_mode = MODE_4_MODIFY_GREEN;
                temp_duration = green_duration;
                set_timer1(250);
            } else if (is_button_pressed_once(BUTTON_INC_INDEX)) {
                increase_temp();
            } else if (is_button_pressed_once(BUTTON_SET_INDEX)) {
                amber_duration = temp_duration;
            }

            update_7seg_buffer_mode(3, temp_duration);
            break;

        case MODE_4_MODIFY_GREEN:
            if (timer1_flag == 1) {
                set_timer1(250);
                blink_all_green_leds();
            }

            if (is_button_pressed_once(BUTTON_MODE_INDEX)) {
                /* Ve mode 1: chuan hoa thoi gian roi khoi dong lai chu ky */
                normalize_durations();
                current_mode = MODE_1_NORMAL;
                state_road1 = 0;
                count_road1 = red_duration;
                state_road2 = 2;
                count_road2 = green_duration;
                set_timer2(1000);
            } else if (is_button_pressed_once(BUTTON_INC_INDEX)) {
                increase_temp();
            } else if (is_button_pressed_once(BUTTON_SET_INDEX)) {
                green_duration = temp_duration;
            }

            update_7seg_buffer_mode(4, temp_duration);
            break;

        default:
            current_mode = MODE_1_NORMAL;
            break;
    }
}