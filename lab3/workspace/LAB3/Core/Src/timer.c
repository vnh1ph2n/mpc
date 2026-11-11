#include "timer.h"

static uint16_t timer1_counter = 0;
static uint16_t timer2_counter = 0;

volatile uint8_t timer1_flag = 0;
volatile uint8_t timer2_flag = 0;

void set_timer1(uint16_t duration_ms) {
    timer1_counter = duration_ms / TIMER_TICK_MS;
    timer1_flag = 0;
}

void set_timer2(uint16_t duration_ms) {
    timer2_counter = duration_ms / TIMER_TICK_MS;
    timer2_flag = 0;
}

/* Goi moi TIMER_TICK_MS tu ngat Timer 2 */
void timer_run(void) {
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter == 0) {
            timer1_flag = 1;
        }
    }

    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter == 0) {
            timer2_flag = 1;
        }
    }
}