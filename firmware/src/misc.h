#ifndef _MISC_H
#define _MISC_H

// Macros
#define SETBITS(X, Y)				X |= Y
#define CLEARBITS(X, Y)				X &= ~Y

#define SETBIT(X, Y)				X |= (1 << Y)
#define CLEARBIT(X, Y)				X &= ~(1 << Y)

#define CONSTRAIN_MIN(VALUE, MIN)	(VALUE > MIN)?VALUE:MIN
#define CONSTRAIN_MAX(VALUE, MAX)	(VALUE < MAX)?VALUE:MAX
#define CONSTRAIN(VALUE, MIN, MAX)	CONSTRAIN_MIN(CONSTRAIN_MAX(VALUE, MAX), MIN)

// Function constructs
void delay_ms(unsigned int);

#endif
