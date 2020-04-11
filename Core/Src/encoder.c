#include "application.h"

/* Private variables ---------------------------------------------------------*/
encoder_ctx_t g_encoder_ctx = {0};

extern pwm_ctx_t g_pwm_ctx;
extern error_ctx_t g_error_ctx;


void encoder_initialize(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint32_t pin)
{
    g_encoder_ctx.timer = timer;
    g_encoder_ctx.counter = 0;
    g_encoder_ctx.direction = ENCODER_DIR_UP;
    
    g_encoder_ctx.push_port = port;
    g_encoder_ctx.push_pin = pin;
}

void encoder_start()
{
    HAL_TIM_Encoder_Start_IT(g_encoder_ctx.timer, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

uint32_t encoder_get_counter()
{
    return __HAL_TIM_GET_COUNTER(g_encoder_ctx.timer) / ENCODER_STEP_DIVISOR;
}

uint8_t encoder_get_direction()
{
    return (__HAL_TIM_IS_TIM_COUNTING_DOWN(g_encoder_ctx.timer)) ? ENCODER_DIR_DOWN : ENCODER_DIR_UP;
}

uint8_t encoder_is_countdown()
{
    return encoder_get_direction() == ENCODER_DIR_DOWN;
}

uint8_t encoder_is_countup()
{
    return encoder_get_direction() == ENCODER_DIR_UP;
}

void encoder_set_counter(uint32_t counter)
{
    if (is_interrupt())
    {
        return;
    }
    
    if (counter > PWM_MAX_VALUE)
    {
        error_set(ERROR_BAD_STATE);
        return;
    }
    g_pwm_ctx.timers_sync_pending = TRUE;
    __HAL_TIM_SET_COUNTER(g_encoder_ctx.timer, counter * ENCODER_STEP_DIVISOR);
    g_pwm_ctx.timers_sync_pending = FALSE;
}

void encoder_update_timer_counter()
{
    volatile uint32_t current_encoder_counter = encoder_get_counter();
    if (current_encoder_counter != g_pwm_ctx.target_value)
    {
        encoder_set_counter(g_pwm_ctx.target_value);
    }
}

void handle_encoder_capture()
{
    volatile int8_t step = (encoder_is_countdown()) ? -1 : 1;
    
    if (encoder_is_pressed())
    {
        step *= ENCODER_BOOST;
    }
    g_pwm_ctx.step = step;
    
    _encoder_do_set_target_value();
}

void _encoder_do_set_target_value()
{
    volatile uint16_t target_value = PWM_MIN_VALUE;

    if (0 == g_pwm_ctx.step) { return; }
    
    if (g_pwm_ctx.step > PWM_MAX_VALUE && ((int16_t)g_pwm_ctx.target_value + g_pwm_ctx.step) > PWM_MAX_VALUE)
    {
        target_value = PWM_MIN_VALUE;
    }
    else
    {
        target_value = MIN(g_pwm_ctx.target_value + g_pwm_ctx.step, PWM_MAX_VALUE);
    }

    g_pwm_ctx.target_value = target_value;
    g_pwm_ctx.step = 0;
}

uint8_t encoder_is_pressed()
{
    return HAL_GPIO_ReadPin(ENCODER_BTN_GPIO_Port, ENCODER_BTN_Pin) ? FALSE : TRUE;
}
