#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"

#define NO_OF_BUTTONS       3
#define BUTTON_MODE_INDEX   0
#define BUTTON_INC_INDEX    1
#define BUTTON_SET_INDEX    2

void button_reading(void);

/* Tra ve 1 trong luc nut dang duoc giu (muc) */
unsigned char is_button_pressed(uint8_t index);

/* Tra ve 1 DUNG MOT LAN cho moi lan nhan (canh xuong), roi tu xoa co */
unsigned char is_button_pressed_once(uint8_t index);

#endif /* INC_INPUT_READING_H_ */