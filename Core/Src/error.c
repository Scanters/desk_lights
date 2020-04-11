#include "error.h"
#include "math.h"

/* Private variables ---------------------------------------------------------*/
error_ctx_t g_error_ctx = {0};

void error_initialize(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint32_t pin)
{
    g_error_ctx.error_timer = timer;
    g_error_ctx.error_gpio_port = port;
    g_error_ctx.error_gpio_pin = pin;
}

void error_start()
{
    // Start_error timer
    HAL_TIM_OC_Start_IT(g_error_ctx.error_timer, TIM_CHANNEL_1);
}

void error_set(uint16_t error)
{
    // The error is already set, no need to re-raise
    if (error == g_error_ctx.error_freq) { return; }    
    
    g_error_ctx.error_freq = error;
    __HAL_TIM_SET_AUTORELOAD(g_error_ctx.error_timer, error);
}

void error_clear()
{
    error_set(ERROR_OK);
    // Mind the inversed error LED circuitry
    HAL_GPIO_WritePin(g_error_ctx.error_gpio_port, g_error_ctx.error_gpio_pin, GPIO_PIN_SET);
}

bool error_is_set()
{
    return ERROR_OK == g_error_ctx.error_freq;
}
