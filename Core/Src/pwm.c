#include "application.h"

/* Private variables ---------------------------------------------------------*/
pwm_ctx_t g_pwm_ctx = {0};

extern error_ctx_t      g_error_ctx;
extern encoder_ctx_t    g_encoder_ctx;
extern adc_ctx_t        g_adc_ctx;


void pwm_initialize(TIM_HandleTypeDef* timer)
{
    g_pwm_ctx.timer = timer;
    g_pwm_ctx.transition_speed = 1;
    g_pwm_ctx.transition_speed_throttle = 1;
}

void pwm_start()
{
    error_clear();
    HAL_TIM_PWM_Start_IT(g_pwm_ctx.timer, TIM_CHANNEL_1);
    pwm_set_min();
}

void pwm_reset_transition_rates()
{
    g_pwm_ctx.transition_speed = 1;
    g_pwm_ctx.transition_speed_throttle = 1;
    
    g_pwm_ctx.transition_done = FALSE;
    g_pwm_ctx.target_value = PWM_MIN_VALUE;
}

void pwm_transition()
{
    uint16_t current_value = __HAL_TIM_GET_COMPARE(g_pwm_ctx.timer, TIM_CHANNEL_1);
    g_pwm_ctx.transition_done = FALSE;
    
    if (TRUE == g_pwm_ctx.transition_done) { return; }
    
    if (g_pwm_ctx.target_value == current_value ||
        g_pwm_ctx.target_value > PWM_MAX_VALUE)
    {
        g_pwm_ctx.transition_done = TRUE;
        return;
    }
    
    volatile int16_t step = (g_pwm_ctx.target_value < current_value) ? -1 : 1;
    
    do
    {
        current_value = __HAL_TIM_GET_COMPARE(g_pwm_ctx.timer, TIM_CHANNEL_1);
        volatile uint16_t new_value = current_value + step;
        if (new_value > PWM_MAX_VALUE)
        {
            break;
        }
        pwm_set_percentage(new_value);
        HAL_Delay(g_pwm_ctx.transition_speed * g_pwm_ctx.transition_speed_throttle);
        
    } while (current_value != g_pwm_ctx.target_value);
}

uint32_t pwm_get_percentage()
{
    return __HAL_TIM_GET_COMPARE(g_pwm_ctx.timer, TIM_CHANNEL_1);
}

void pwm_set_percentage(uint8_t percentage)
{
    if (__HAL_TIM_GET_COMPARE(g_pwm_ctx.timer, TIM_CHANNEL_1) == percentage)
    {
        // Already set, no errors to re-reaise
        return;
    }

    if (PWM_MAX_VALUE < percentage)
    {
        error_set(ERROR_BAD_STATE);
        _pwm_set_percentage(PWM_MAX_VALUE);
        return;
    }

    error_clear();
    _pwm_set_percentage(percentage);
}

static void _pwm_set_percentage(uint8_t percentage)
{
    if (PWM_MAX_VALUE < percentage)
    {
        error_set(ERROR_CRITICAL);
        return;
    }

    // It is now implied that the value is verified and validated
    __HAL_TIM_SET_COMPARE(g_pwm_ctx.timer, TIM_CHANNEL_1, percentage);
}

void pwm_set_min()
{
    pwm_set_percentage(PWM_MIN_VALUE);
}

void pwm_set_max()
{
    pwm_set_percentage(PWM_MAX_VALUE);
}

void pwm_update_target()
{
    pwm_set_percentage(g_pwm_ctx.target_value);
    //encoder_update_timer_counter();
}

uint8_t pwm_is_on()
{
    return (__HAL_TIM_GET_COMPARE(g_pwm_ctx.timer, TIM_CHANNEL_1) > PWM_MIN_VALUE) ? TRUE : FALSE;
}

uint8_t pwm_is_off()
{
    return (pwm_is_on()) ? FALSE : TRUE;
}

// TESTS

void __test_pwm_loop()
{
    volatile uint32_t counter = g_encoder_ctx.counter;
    
    error_set(ERROR_BAD_STATE);
    HAL_Delay(1000);
    
    error_set(ERROR_CRITICAL);
    HAL_Delay(1000);

    for (uint8_t x = 0; x <= PWM_MAX_VALUE; x+= 20)
    {
        pwm_set_percentage(x);
        HAL_Delay(500);
    }
}

void __test_pwm_loop2()
{
    g_pwm_ctx.target_value = PWM_MAX_VALUE;
    g_pwm_ctx.transition_speed_throttle = 10;
    pwm_transition();
    
    pwm_reset_transition_rates();
    pwm_transition();
}

void __test_adc_react()
{
    g_pwm_ctx.transition_speed_throttle = 5;

    if (g_adc_ctx.result < 0xFC0)
    {
        g_pwm_ctx.target_value = PWM_MAX_VALUE;
        pwm_transition();
        return;
    }
    else
    {
        g_pwm_ctx.target_value = PWM_MIN_VALUE;
        pwm_transition();
        return;
    }
}
