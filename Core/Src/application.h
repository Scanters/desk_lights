#ifndef __APPLICATION_H
#define __APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "pwm.h"
#include "error.h"
#include "encoder.h"
#include "adc.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TRUE                (1)
#define FALSE               (0)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Private function prototypes -----------------------------------------------*/
bool is_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif /* __APPLICATION_H */
