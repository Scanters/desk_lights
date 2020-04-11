#ifndef __ITR_CALLBACKS_H
#define __ITR_CALLBACKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "application.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

void _handle_sensor_itr(uint16_t pin);
void _handle_encoder_push_itr(uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif /* __ITR_CALLBACKS_H */
