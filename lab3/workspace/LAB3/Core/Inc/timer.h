#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_TICK_MS   10

void set_timer1(uint16_t duration_ms);
void set_timer2(uint16_t duration_ms);
void timer_run(void);

/* ISR ghi, main doc nen phai volatile */
extern volatile uint8_t timer1_flag;
extern volatile uint8_t timer2_flag;

#endif /* INC_TIMER_H_ */