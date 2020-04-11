#include "application.h"

/* Private variables ---------------------------------------------------------*/

bool is_interrupt()
{
    return FALSE != (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
}
