# logic_probe

Teaching the fundamentals of electronics requires tools that allow students to
experiment with real circuits and understand their principles of operation. Traditional
conventional tools lack flexibility for teaching purposes and may be too complicated for
a person who is just discovering the properties of electronic circuits. This work addresses
this need by designing a multifunctional logic probe that combines the functions of a
logic analyzer, signal generator, and communication interface tester. Its key advantage is
the possibility to be assembled simply using available microcontrollers, which makes it
suitable for use in teaching.  
The probe exists in two variants: a full featured STM32 based and a limited Raspberry
Pi Pico based. In basic mode, it provides logic level detection, frequency measurement,
pulse generation and voltage measurement. The extended version provides diagnostics of
known serial interfaces (UART, I2C, SPI, Neopixel). Integration with a PC terminal allows
the use of advanced functions, while local mode is used for fast analysis without the need
for a PC.  
The hardware design is optimized to minimize external components with an emphasis
on the use of internal microcontroller peripherals, allowing the device to be assembled on
a non soldering contact array. The thesis includes firmware, documentation and student
tutorials that cover the probe build and examples of its use. The result is an open source
solution that can be further extended and adapted to specific educational needs.
