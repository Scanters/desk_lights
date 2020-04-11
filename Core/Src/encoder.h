#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "stdbool.h"
#include "itr_callbacks.h"
#include "application.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    TIM_HandleTypeDef* timer;
    volatile uint32_t counter;
    volatile uint8_t direction;
    
    GPIO_TypeDef* push_port;
    uint32_t push_pin;
    bool boost_mode;
} encoder_ctx_t;


/* Private define ------------------------------------------------------------*/
#define ENCODER_DIR_UP          (0)
#define ENCODER_DIR_DOWN        (1)

#define ENCODER_STEP_DIVISOR    (4)
#define ENCODER_HUGE_STEP       (25)
#define ENCODER_BOOST           (25)

/* Private function prototypes -----------------------------------------------*/
void encoder_initialize(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint32_t pin);
void encoder_start(void);

uint32_t encoder_get_counter(void);
void encoder_set_counter(uint32_t counter);

void encoder_update_timer_counter(void);
uint8_t encoder_get_direction(void);
uint8_t encoder_is_countdown(void);
uint8_t encoder_is_countup(void);

void _encoder_do_set_target_value(void);
void handle_encoder_capture(void);

uint8_t encoder_is_pressed(void);

#ifdef __cplusplus
}
#endif

#endif /* __ENCODER_H */
