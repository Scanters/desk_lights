#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "application.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    ADC_HandleTypeDef* adc;
    TIM_HandleTypeDef* timer;
    volatile uint32_t result;
} adc_ctx_t;

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void adc_initialize(ADC_HandleTypeDef* adc, TIM_HandleTypeDef* timer);
void adc_start(void);
uint16_t adc_get_value(void);


#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
