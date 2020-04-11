#include "application.h"

/* Private variables ---------------------------------------------------------*/
adc_ctx_t g_adc_ctx = {0};


void adc_initialize(ADC_HandleTypeDef* adc, TIM_HandleTypeDef* timer)
{
    g_adc_ctx.adc = adc;
    g_adc_ctx.timer = timer;
    
    HAL_ADCEx_Calibration_Start(g_adc_ctx.adc);
}

void adc_start()
{
    HAL_ADC_Start_IT(g_adc_ctx.adc); 
    HAL_TIM_OC_Start_IT(g_adc_ctx.timer, TIM_CHANNEL_2);
}

uint16_t adc_get_value()
{
    HAL_ADC_Start_IT(g_adc_ctx.adc);
    volatile uint16_t result = g_adc_ctx.result;
    HAL_ADC_PollForEvent(g_adc_ctx.adc, ADC_FLAG_EOC, 100);
    HAL_ADC_Stop_IT(g_adc_ctx.adc);
    return result;
}
