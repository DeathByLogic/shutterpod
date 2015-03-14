#Hantronix HDM16216L-5 16x2 LCD Display

## Description ##
The Hantronix LCD display is a 16x2 display with a white LED back light. Other than power back light and contrast the module uses three pins as control signals and eight pins for data (though it is possible to use only four).

## Contrast Adjustment ##
The contrast for the Hantronix display is controlled by adjusting the current flow from the V<sub>L</sub> pin to ground. This can be done using a resistor to tie the pin to ground and limit the current flowing, or in our case using a transistor to adjust the flow of current.

Through experimentation I have found that the display does not start to become visible until there is around 325 uA flowing from the V<sub>L</sub> pin. At full contrast (V<sub>L</sub> shorted to ground) there was around 450 uA.

## Contrast Adjustment Circuit Analysis ##

All the calculations done in this section were based on a transistor with a DC gain (B) of 160 and can easily be recalculated for transistors with different gains.

[R9](https://code.google.com/p/shutterpod/source/detail?r=9) biases the transistor for around 325 uA of current to flow from collector to emitter without any input from the micro-controller. This forces the display contrast to stay at a point where it is always visible, ever so slightly. The output of the micro-controller is a PWM signal which passes through a low pass filter ([R11](https://code.google.com/p/shutterpod/source/detail?r=11) & C9) to smooth out the PWM signal to a DC voltage ranging from 0 to 5V.

![http://shutterpod.googlecode.com/svn/wiki/Contrast_Control_Circuit.png](http://shutterpod.googlecode.com/svn/wiki/Contrast_Control_Circuit.png)