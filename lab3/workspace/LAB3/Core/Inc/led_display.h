#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

void display_traffic_light_normal(uint8_t state_road1, uint8_t state_road2);
void blink_all_red_leds(void);
void blink_all_amber_leds(void);
void blink_all_green_leds(void);

void update_7seg_buffer_normal(uint8_t count_road1, uint8_t count_road2);
void update_7seg_buffer_mode(uint8_t mode, uint8_t duration);
void display_7seg_scan(void);

#endif /* INC_LED_DISPLAY_H_ */