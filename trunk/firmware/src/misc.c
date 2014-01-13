#include <util/delay.h>
#include "misc.h"

// Delay function
void delay_ms(unsigned int ms)
{
        while(ms){
                _delay_ms(0.96);
                ms--;
        }
}
