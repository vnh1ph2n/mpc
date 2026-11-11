#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

typedef enum {
    MODE_1_NORMAL = 0,
    MODE_2_MODIFY_RED,
    MODE_3_MODIFY_AMBER,
    MODE_4_MODIFY_GREEN
} TrafficMode_t;

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */