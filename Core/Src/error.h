#ifndef __ERROR_H
#define __ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


#include "application.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    volatile uint16_t error_freq;
    TIM_HandleTypeDef* error_timer;
    
    GPIO_TypeDef* error_gpio_port;
    uint16_t error_gpio_pin;
} error_ctx_t;


/* Private define ------------------------------------------------------------*/
#define ERROR_OK            (0)
#define ERROR_CONST         (1)
#define ERROR_CRITICAL      (199)
#define ERROR_BAD_STATE     (499)
#define ERROR_CORRECTABLE   (999)


/* Private function prototypes -----------------------------------------------*/
void error_initialize(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint32_t pin);
void error_start(void);

void error_set(uint16_t error);
void error_clear(void);
bool error_is_set(void);


#ifdef __cplusplus
}
#endif

#endif /* __ERROR_H */
