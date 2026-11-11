#include "input_reading.h"

#define BUTTON_IS_PRESSED   GPIO_PIN_RESET
#define BUTTON_IS_RELEASED  GPIO_PIN_SET

/* Ket qua cuoi sau khi chong rung. volatile vi ISR ghi, main doc */
static volatile GPIO_PinState buttonBuffer[NO_OF_BUTTONS] =
    {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] =
    {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] =
    {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

/* Trang thai da loc o lan quet truoc, dung de bat canh xuong */
static GPIO_PinState prevState[NO_OF_BUTTONS] =
    {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

/* Co su kien "vua nhan", ISR set, main xoa */
static volatile uint8_t pressedEvent[NO_OF_BUTTONS] = {0, 0, 0};

static const uint16_t BUTTON_PINS[NO_OF_BUTTONS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2};

/* Goi moi 10ms tu ngat Timer 2 */
void button_reading(void) {
    for (uint8_t i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, BUTTON_PINS[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }

        /* Canh xuong: tha -> nhan */
        if (buttonBuffer[i] == BUTTON_IS_PRESSED && prevState[i] == BUTTON_IS_RELEASED) {
            pressedEvent[i] = 1;
        }
        prevState[i] = buttonBuffer[i];
    }
}

unsigned char is_button_pressed(uint8_t index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_once(uint8_t index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (pressedEvent[index]) {
        pressedEvent[index] = 0;
        return 1;
    }
    return 0;
}