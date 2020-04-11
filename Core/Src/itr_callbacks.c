#include "itr_callbacks.h"
#include "error.h"
#include "encoder.h"

extern pwm_ctx_t        g_pwm_ctx;
extern error_ctx_t      g_error_ctx;
extern encoder_ctx_t    g_encoder_ctx;
extern adc_ctx_t        g_adc_ctx;


// CALLBACKS
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    // Trigger the code part for ERROR_TIMER
    if (htim->Instance == g_error_ctx.error_timer->Instance)
    {
        if (ERROR_OK == g_error_ctx.error_freq) { return; };
        
        HAL_GPIO_TogglePin(g_error_ctx.error_gpio_port, g_error_ctx.error_gpio_pin);
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) 
{
    if(htim->Instance == g_encoder_ctx.timer->Instance)
    {
        handle_encoder_capture();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case (ENCODER_BTN_Pin):
            _handle_encoder_push_itr(GPIO_Pin);
            //_handle_sensor_itr(GPIO_Pin);
            break;
        case (SENSOR_PIN_Pin):
            _handle_sensor_itr(GPIO_Pin);
            break;
        default:
            break;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    volatile uint16_t result = HAL_ADC_GetValue(g_adc_ctx.adc);
    g_adc_ctx.result = result;
}    

void _handle_sensor_itr(uint16_t pin)
{
    if (HAL_GPIO_ReadPin(SENSOR_PIN_GPIO_Port, SENSOR_PIN_Pin))
    {
        volatile uint32_t target_value = g_pwm_ctx.target_value + PWM_EXTI_STEP;
        
        if (g_pwm_ctx.target_value == PWM_MAX_VALUE)
        {
            // Handle overflow, reset to 0?
            // target_value = PWM_MIN_VALUE;
        }
        else if (target_value > PWM_MAX_VALUE)
        {
            target_value = PWM_MAX_VALUE;
        }
        else
        {
        }

        g_pwm_ctx.target_value = target_value;
        encoder_set_counter(g_pwm_ctx.target_value);
    }
}

void _handle_encoder_push_itr(uint16_t pin)
{
    g_encoder_ctx.boost_mode = encoder_is_pressed();
}
