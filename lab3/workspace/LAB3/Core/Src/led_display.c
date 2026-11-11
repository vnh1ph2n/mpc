#include "led_display.h"

/* LED giao thong noi active-low (anode VDDA, cathode vao chan MCU) */
#define LED_ON   GPIO_PIN_RESET
#define LED_OFF  GPIO_PIN_SET

#define ALL_TRAFFIC_PINS (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
                          GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13)

/* 7 doan anode chung: 0 = sang */
static const uint8_t LED7_HEX[10] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
};

static uint8_t led7_buffer[4] = {0, 0, 0, 0};
static uint8_t led7_index = 0;

static uint8_t clamp99(uint8_t v) {
    return (v > 99) ? 99 : v;
}

void update_7seg_buffer_normal(uint8_t count_road1, uint8_t count_road2) {
    count_road1 = clamp99(count_road1);
    count_road2 = clamp99(count_road2);
    led7_buffer[0] = count_road1 / 10;
    led7_buffer[1] = count_road1 % 10;
    led7_buffer[2] = count_road2 / 10;
    led7_buffer[3] = count_road2 % 10;
}

void update_7seg_buffer_mode(uint8_t mode, uint8_t duration) {
    duration = clamp99(duration);
    led7_buffer[0] = duration / 10;
    led7_buffer[1] = duration % 10;
    led7_buffer[2] = 0;
    led7_buffer[3] = mode % 10;
}

/* Goi moi 10ms tu ngat Timer 2: quet 1 trong 4 LED 7 doan */
void display_7seg_scan(void) {
    /* Tat ca 4 EN (PNP: HIGH = tat) de tranh ghosting */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);

    uint8_t value = LED7_HEX[led7_buffer[led7_index]];
    for (uint8_t i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, (uint16_t)(1U << i),
                          ((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    /* Bat LED hien tai (LOW = bat) */
    HAL_GPIO_WritePin(GPIOA, (uint16_t)(GPIO_PIN_4 << led7_index), GPIO_PIN_RESET);

    led7_index = (led7_index + 1) % 4;
}

/* Nhap nhay mot cap den (huong 1 + huong 2) cung pha, tat cac den con lai */
static void blink_pair(uint16_t pins) {
    static uint8_t on = 0;
    HAL_GPIO_WritePin(GPIOA, ALL_TRAFFIC_PINS & ~pins, LED_OFF);
    on = !on;
    HAL_GPIO_WritePin(GPIOA, pins, on ? LED_ON : LED_OFF);
}

void blink_all_red_leds(void)   { blink_pair(GPIO_PIN_8  | GPIO_PIN_11); }
void blink_all_amber_leds(void) { blink_pair(GPIO_PIN_9  | GPIO_PIN_12); }
void blink_all_green_leds(void) { blink_pair(GPIO_PIN_10 | GPIO_PIN_13); }

/* state: 0 = do, 1 = vang, 2 = xanh */
void display_traffic_light_normal(uint8_t state_road1, uint8_t state_road2) {
    HAL_GPIO_WritePin(GPIOA, ALL_TRAFFIC_PINS, LED_OFF);

    if (state_road1 <= 2) HAL_GPIO_WritePin(GPIOA, (uint16_t)(GPIO_PIN_8  << state_road1), LED_ON);
    if (state_road2 <= 2) HAL_GPIO_WritePin(GPIOA, (uint16_t)(GPIO_PIN_11 << state_road2), LED_ON);
}