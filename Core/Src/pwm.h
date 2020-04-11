#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "application.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    TIM_HandleTypeDef* timer;
    
    volatile uint16_t target_value;
    volatile uint16_t transition_speed;
    volatile int8_t step;
    volatile uint8_t transition_speed_throttle;
    volatile uint8_t transition_done;
    volatile uint8_t timers_sync_pending;
} pwm_ctx_t;


/* Private define ------------------------------------------------------------*/
#define PWM_MIN_VALUE       (0U)
#define PWM_MAX_VALUE       (100U)

#define PWM_EXTI_STEP       (25)

/* Private function prototypes -----------------------------------------------*/
void pwm_initialize(TIM_HandleTypeDef* timer);
void pwm_start(void);
void pwm_reset_transition_rates(void);
void pwm_transition(void);

uint32_t pwm_get_percentage(void);
void pwm_set_percentage(uint8_t percentage);
static void _pwm_set_percentage(uint8_t percentage);
void pwm_set_min(void);
void pwm_set_max(void);
void pwm_update_target(void);

uint8_t pwm_is_on(void);
uint8_t pwm_is_off(void);

// TESTS
void __test_pwm_loop(void);
void __test_pwm_loop2(void);
void __test_adc_react(void);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_H */
