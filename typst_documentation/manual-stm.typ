#import "./template/template/template.typ": *
#import "@preview/fletcher:0.5.4" as fletcher: diagram, node, edge
#set math.equation(numbering: "(1)")
// add pwsh syntax highlighting and make it the default lang for blocks

#show: template.with(
  meta: (
    title: "Manual for STM32 diagnostic logic probe", author: (
      name: "Milan Jiříček", email: "jiricmi1@fel.cvut.cz", url: "https://github.com/jiricmi/logic_probe",
    ), bachelor: true, diff_usage: true, supervisor: "doc. Ing. Jan Fischer, CSc.", faculty: "Faculty of Electrical Engineering", department: "Deparment of measurement", study-programme: "Open Informatics",
  ), print: false, lang: "en", study-spec: [IOT],
  abstract-en: [
     ],
  abstract-cz: [
      ],
  acknowledgement: [
  ],
)

#outline(depth: 3)

#set par(first-line-indent: 0em, spacing: 2.5em, justify: true)

= Build
== STM32G030 SOP8
=== Components
- *1x* STM32G030 SOP8 package
- *1x* LDO HT7533
- *1x* Green LED
- *2x* Capacitor $47$ $mu$F
- *1x* Capacitor $100$ nF
- *1x* WS2812D RGB Neopixel LED
- *1x* Button
- *2x* Resistor $480$ $Omega$
- *1x* Breadboard

=== Assembly
Follow the circuit assembly instructions. Visually inspect all connections before applying power. Incorrect procedure risks damaging the MCU. 
#figure(
    caption:text(size: 15pt)[1. Connect STM32G30 to the breadboard],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_1.jpg")
)
#figure(
    caption:text(size: 15pt)[2. Connect LDO to breadboard with 3V3 pin closer to MCU],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_2.png", width: 75%)
)
#figure(
    caption:text(size: 15pt)[3. Connect LDO Ground to upper side of the breadboard (blue line).\ Connect 3V3 LDO output to down side of the breadboard. Connect LDO 5V input to red line pinout (yellow wire). Connect LDO ground to blue stripe (grey wire).\ Connect $47$ $mu$F capacitor between ground and LDO 5V input],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_3.png", width: 70%)
)
#figure(
    caption:text(size: 15pt)[4. Connect $480$ $Omega$ between LDO 3V3 output (green line) and purple row],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_4.png", width: 65%)
)
#figure(
    caption:text(size: 15pt)[5. Connect LED between purple line and ground\ Connect Pin 2 with LDO 3V3 Output (green lines)\ Connect Pin 3 to ground (blue lines) \ Connect Capacitor $47$ $mu$F between pin 2 and ground],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_5.png", width: 70%)
)
#figure(
    caption:text(size: 15pt)[6. Connect pin 5 to purple line and pin 6 to aqua line (UART USB Converter)],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_6.png", width: 75%)
)
#figure(
    caption:text(size: 15pt)[7. Add button and Neopixel RGB LED \ Pin 2 of LED is longest one \ Connect RGB pin 3 to ground \ Connect Button to ground],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_7.png", width: 75%)
)
#figure(
    caption:text(size: 15pt)[8. Connect pin 7 MCU to button. \ Optionally add wire from pin 1 and 4 to the side of the breadboard for better channel connection during measuring],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_8.jpg", width: 60%)
)
#figure(
    caption:text(size: 15pt)[9. Connect RGB LED pin 2 to 3V3 (green) \ Connect Capacitor $100$ nF between ground and RGB LED pin 2\ Connect RGB LED pin 4 to MCU pin 8 (purple)],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_9.png", width: 65%)
)
#v(10pt)
#figure(
    caption:text(size: 15pt)[10. Connect UART USB Converter according to Pinout],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop8_10.png", width: 65%)
)
#v(10pt)

#v(10pt)
#figure(
        placement:none,
        caption:[Diagram SOP8],
        image("pic/sop8_hw.png")
    )
#v(10pt)

== STM32G030 TSSOP20
=== Components
- *1x* STM32G030 TSSOP20 package
- *1x* LDO HT7533
- *1x* Green LED
- *2x* Capacitor $47$ $mu$F
- *1x* Capacitor $100$ nF
- *1x* WS2812D RGB Neopixel LED
- *1x* Button
- *2x* Resistor $480$ $Omega$
- *1x* Breadboard

=== Assembly
Follow the circuit assembly instructions. Visually inspect all connections before applying power. Incorrect procedure risks damaging the MCU. 
#figure(
    caption:text(size: 15pt)[1. Connect STM32G30 to the breadboard],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_1.jpg") 
)
#figure(
    caption:text(size: 12pt)[2. Connect LDO to breadboard with 3V3 pin closer to MCU \  Connect LDO Ground pin to upper side of the breadboard (blue line).\ Connect 3V3 LDO output to down side of the breadboard. \  Connect LDO 5V input to red line pinout (white wire). \  Connect LDO ground to blue stripe (green wire).\ Connect $47$ $mu$F capacitor between ground and LDO 5V input],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_2.png", width: 75%) 
)
#figure(
    caption:text(size: 15pt)[3. Connect $480$ $Omega$ between LDO 3V3 output and purple line \ Connect LED between purple line and ground],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_3.png", width: 70%)
)
#figure(
    caption:text(size: 15pt)[4. Connect Pin 4 with LDO 3V3 Output (green lines)\ Connect Pin 5 to ground (blue lines) \ Between Pin 4 and ground add $47$ $mu$F capacitor],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_4.png", width: 70%)
)
#figure(
    caption:text(size: 15pt)[5. Connect pin 16 to purple line and pin 17 to aqua line (UART USB Converter)\ Add button and Neopixel RGB LED \ Pin 2 of LED is longest one \ Connect RGB pin 3 to ground \ Connect Button to ground],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_5.png", width: 75%)
)
#figure(
    caption:text(size: 15pt)[6. Connect RGB LED pin 2 to 3V3 (green) \ Connect Capacitor $100$ nF between ground and RGB LED pin 2\ Connect RGB LED pin 4 to MCU pin 20 (purple) \ Connect button to MCU pin 18],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_6.png", width: 75%)
)
#figure(
    caption:text(size: 15pt)[7. Connect MCU pin 11 to 3V3.],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_7.png", width: 75%)
)
#figure(
    caption:text(size: 15pt)[8. Connect UART USB Converter according to Pinout],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/b_sop20_8.png", width: 65%)
)
#figure(
        placement:none,
        caption:[Diagram TSSOP20],
        image("pic/tssop20_hw.png", width: 65%)
    )



= Firmware setup
==== Before start
1. *Download and Install STM32CubeProgrammer*
    - *Note:* version 2.3.0 ideal version for setup
2. *Bootloader Mode*
    - *Power Supply*: Ensure your STM32 microcontroller or development board is not powered before making any connections for bootloader mode.
    - Connect $2.2$ k$Omega$ pull-up resistor between *BOOT0* and *3V3*
        - _TSSOP20_: BOOT0 is PIN 19
        - _SOP8_: BOOT0 is PIN 8
    - Ensure that PIN 4 is not connected to something (due to NRST)
3. *Connect device via UART USB Converter to PC*

*NOTE*: Do not forget to remove resistor after firmware setup. If you do not make changes according to manual, it will be necessary to use SWD to fix it.

==== STMCubeProgrammer
#figure(
    caption:text(size: 15pt)[1. Select UART for connection],
    supplement: [],
    placement: none,
    numbering: none,
    outlined: false,
    image("pic/1_st.png")
)
#figure(
    caption:text(size: 15pt)[2. Select correct COM port and connect to the device],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/2_st.png", width: 90%)
)
#figure(
    caption:text(size: 15pt)[3. Click on OB icon on side bar\ and then open "_User Configuration_"],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/4_st.png", width: 90%)
)

#figure(
    caption:text(size: 15pt)[4. Uncheck `nBOOT_SEL` option \ SET `NRST_MODE` to *2* \ Apply settings],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/5_st.png", width: 90%)
)

#figure(
    caption:text(size: 15pt)[5. Upload FW],
    supplement: [],
    numbering: none,
    outlined: false,
    placement: none,
    image("pic/6_st.png", width: 90%)
)

*POWER OFF THE DEVICE, AND REMOVE RESISTOR FROM BOOT0!*

= Local mode
This mode enables quick circuit diagnostics without requiring a computer connection. It offers the following features:
- *Logic Probe*: Instant visualization of signal states (HIGH/LOW) on connected channels.
- *Edge Detection*: Recognition of both rising edge and falling edge signal transitions.
- *Pulse Generation*: Continuous pulsing
- *Manual Logic Level Control*: Set fixed HIGH/LOW levels on selected channels.
To switch between modes, hold the button for approximately 500 ms. The RGB LEDs indicate the active mode by displaying its assigned color when the button is released.
=== Activation
*SOP8 Package, DO NOT CONNECT ANYTHING TO PA0. IT WILL PUT DEVICE TO RESET!*\
Local mode is enabled when the device is powered without UART computer connection. To activate it:
1. Connect only the power supply to the assembled circuit (no data/USB connection).
2. Upon startup, the device detects a logic LOW (0) on the receive (RX) pin, which is reserved for computer communication.
    - _Note_: When connected to a computer, the RX pin defaults to logic HIGH (1), disabling Local mode.
3. A startup RGB LED color sequence (e.g., alternating green-blue-red) will play to visually confirm Local mode is active.

== Logic probe mode
This mode is designed for diagnosing logic circuits. It enables real-time monitoring of logic states (HIGH/LOW) across two independent channels via color-coded LED indicators. The channels are assigned to pins PB7 (Channel 1) and PA0 (Channel 2). Ideal for quick signal verification.
===== LED Behavior:
- Mode indicator: #highlight(fill: aqua)[AQUA]
===== Channel Selection: 
    - Toggle Channel 1 and Channel 2 by _pressing_ the button.
    - Selected channel is confirmed by the LED color:
        - *Channel 1*: #highlight(fill:purple)[PURPLE]
        - *Channel 2*: #highlight(fill:yellow)[YELLOW]
===== State Detection:
    - *HIGH (Logic 1)*: #highlight(fill: green)[GREEN]
    - *LOW (Logic 0)*: #highlight(fill: red)[RED]
    - Floating/Intermediate State: LED turns OFF
===== PIN Assignments:
- *Channel 1*: `PB7`
    - _SOP8_ package: Pin 1
    - _TSSOP20_ package: Pin 1
- *Channel 2*: `PA0`
    - _SOP8_ package: Pin 4
    - _TSSOP20_ package: Pin 7

== Generating periodic pulses
This mode produces periodic pulses (1 Hz) with button-controlled activation. Output state visualized through persistent LED feedback.

===== LED Behavior:
- Mode indicator: #highlight(fill: white)[WHITE]
- Active state: Maintains #highlight(fill: white)[WHITE] continuously

===== Operation:
- Press button to toggle between:
    - ON: Signal generation active
    - OFF: Output disabled

===== PIN Assignment:
- Signal output: `PA0`
    - _SOP8_ package: Pin 4
    - _TSSOP20_ package: Pin 7

== Rise edge detection
This mode detects rising edge on input pin. Time-limited visual confirmation provided upon detection.

===== LED Behavior:
- Mode indicator: #highlight(fill: blue)[BLUE]
- Detection confirmation: #highlight(fill: green)[GREEN] (1 second)

===== Operation:
- Continuous monitoring of PA0 pin for rising edge
- When pulse detected, #highlight(fill: green)[GREEN] LED color is on for 1 second

===== PIN Assignment:
- Signal input: `PA0`
    - _SOP8_ package: Pin 4
    - _TSSOP20_ package: Pin 7

== Falling edge detection
This mode detects falling edge on input pin. Time-limited visual confirmation provided upon detection.

===== LED Behavior:
- Mode indicator: #highlight(fill: purple)[PURPLE]
- Detection confirmation: #highlight(fill: green)[GREEN] (1 second)

===== Operation:
- Continuous monitoring of PA0 pin for rising edge
- When pulse detected, #highlight(fill: green)[GREEN] LED color is on for 1 second

===== PIN Assignment:
- Signal input: `PA0`
    - _SOP8_ package: Pin 4
    - _TSSOP20_ package: Pin 7






= Terminal mode
This mode enables deeper circuit diagnostic with PC control. It offers the following features:
1. Basic mode (SOP8/TSSOP20)
    - *Voltage measure*
    - *Logic Level measure*
    - *Resistence measure*
    - *Manual logic Level Control*
    - *Frequency measure*
    - *Pulse detection*
2. Advanced mode (TSSOP20 Only)
    - *Neopixel communication*
        - Monitor 
        - RGB LED tester
    - *Shift register tester*
    - *UART*
        - Monitor
        - Symbol sequence sender
    - *I2C Bus*
        - Monitor (without interaction)
        - Simulating slave
        - Simulating master
        - Address scanner
        - SSD1306 communication tester
    - *SPI*
        - Monitor (without interaction)
        - Simulating master
        - SSD1306 communication tester
=== Activation
*SOP8 Package, DO NOT CONNECT ANYTHING TO PA0 BEFORE CONNECTING POWER SUPPLY. IT WILL PUT DEVICE TO RESET!*\
Terminal mode is enabled when the device is powered with UART computer connection. To activate it:
1. Connect the power supply and RX/TX of UART channel to PC.
2. RGB LED will be red.
3. Connect to device via serial communication. Open PuTTY app and connect to specific COM (WIN) or /dev/ttyUSBx (Linux) with `115200` baudrate 8N1 (@putty-app)
4. After connection press *R* to start

#figure(
    caption: [PuTTY app],
    image("pic/putty_settings.png"),

)<putty-app>


== Universal keys
- *[R]* - reload page to fix broken ansi sequences
- *[Q]* - return to menu

== PINOUT
Colors of wires show location of possible channel connection. If text is colored, (#highlight(fill: red)[PA0/7]), pin is used for functionality.

#v(10pt)
#grid(
    columns:2,
    [#figure(
        caption:[Voltage channels SOP8],
        image("pic/sop8_fr.png"))<sop_voltage>], 
    [#figure(
        caption:[Voltage channels TSSOP20],
        image("pic/tssop20_fr.png"))<tssop_voltage>],
)
#v(10pt)
== MENU
The menu is divided into basic functions and advanced functions. In the *SOP8* version only the basic function is available. By pressing letter on keyboard written next to name, function can be selected.
#v(10pt)
#grid(
    columns:2,
    figure(
        caption:[Basic menu],
        image("pic/tui_main.png")),
    figure(
        caption:[Advanced menu],
        image("pic/tui_main_advanced.png")
    )
)
#v(10pt)

== Basic Mode
=== Page Voltmeter/Ohmmeter
==== Voltmeter and Logic Levels
===== Pinout
- #highlight(fill: blue)[SOP8(PB7/1)|TSSOP20(PB7/1)]: Measures Channel 1
- #highlight(fill: red)[SOP8(PA0/4)|TSSOP20(PA0/7)]: Measures Channel 2
- #highlight(fill: yellow)[(-------------------)|TSSOP20(PA1/8)]: Measures Channel 3

===== Control
- *[1-3]* - Used to turn the respective measurement channel on or off.
- *[S]* -Confirms (saves) the current on/off settings for the channels.
- *[M]* - Switches between Voltmeter and Ohmmeter mode.

==== Description
The voltmeter allows measurements on multiple channels. Different device versions may vary in the number of available measurement channels. The page also displays the Reference Voltage value, which is used for calibrating the measured voltage on individual channels.

In addition to the measured value, an indicator of its logic level is displayed for each channel:
- If *H* is displayed: The voltage on the channel corresponds to a High level. 
- If *L* is displayed: The voltage on the channel corresponds to a Low level.
- If *?* is displayed: The channel status is indeterminate (e.g., the input is floating, or in a transitional state).
- If the symbol *x* is displayed instead of a numerical voltage value for a channel, it means that the channel is currently turned off.

_Example_: To turn off channel 1, press the [1] key. To confirm and save this change, subsequently press the [S] key
#figure(
    placement:none,
    caption: [Voltmeter page],
  image("pic/tui_voltmetr.png", width: 80%)
)

==== Ohmmeter
===== Pinout
- #highlight(fill: blue)[SOP8(PB7/1)|TSSOP20(PB7/1)]: Measures resistence

===== Control
- *[E]* - Enter or exit edit mode for the base resistor (R1) value.
- *[0-9]* - Enter digits for the R1 value.
- *[X]* - Delete the last entered digit for R1 / Clear R1 value.
- *[M]* - Change mode to Voltmeter.

==== Description
Pressing the *[M]* key changes the current mode to *Ohmmeter mode*. The Ohmmeter page illustrates how to connect a resistor to perform the measurement. The reference voltage (*Vdd*) used for the calculation is displayed on this page.

A known base resistor (R1) is used for the calculation of the unknown measured resistance. The value of this base resistor R1 can be changed by first pressing the *[E]* key to enter edit mode. The numerical value can then be entered using the number keys *[0-9]* and can be corrected using the *[X]* key (to delete digits or clear the input). Pressing *[E]* again exits edit mode.

The calculated value of the measured resistor is displayed in a green field on the screen.

#v(10pt)
#figure(
  placement:none,
  caption: [Ohmmeter page],
  image("pic/tui_ohm.png", width: 80%)
)
#v(10pt)


#pagebreak()
=== Page Signal detection
==== Frequency

===== Pinout
- #highlight(fill: red)[SOP8(PA0/4)|TSSOP20(PA0/7)]: Measures frequency

===== Control
- *[T]* - Change gate time (measurement period).
- *[M]* - Changes the mode (e.g., to Rise edge, Fall edge, or the next main function).

==== Description
In this mode, the device measures several characteristics of an incoming signal:
- Frequency
- Reciprocal frequency#footnote[The difference between frequency and reciprocal frequency lies in the calculation method and resulting unit (Hz vs. seconds).]
- Pulse width
- Duty cycle (or active pulse time)

The *[T]* key allows the user to cycle through different gate time settings, which defines the period over which the frequency or other time-related parameters are measured. A longer gate time can provide more accurate readings for lower frequencies but takes longer, while a shorter gate time is faster and better for higher frequencies.

#figure(
  placement:none,
  caption: [Frequency page],
  image("pic/tui_freq.png", width: 80%)
)

==== Pulse Detection

===== Pinout
- #highlight(fill: red)[SOP8(PA0/4)|TSSOP20(PA0/7)]: Detects pulses


===== Control
- *[D]* - Clears the flag indicating a caught signal.
- *[M]* - Changes the mode (e.g., to Rise edge, Fall edge, or the next main function).

===== Description
Pressing the *[M]* key cycles between *Rise edge* mode, *Fall edge* mode and back to *Frequency*.
- *Rise edge* mode is designed to catch a rising edge of a signal.
- *Fall edge* mode is designed to catch a falling edge of a signal.

If the selected type of edge is detected on the input pin, a "Flag" will be set to `TRUE` on the display. This flag indicates that an event has occurred. To reset or clear this flag (e.g., to prepare for catching a new pulse), press the *[D]* key.

#v(10pt)
#figure(
  placement:none,
  caption: [Pulse page],
  image("pic/tui_pulse.png", width: 80%)
)
#v(10pt)
#pagebreak()
=== Page Signal generator

===== Pinout
- #highlight(fill: red)[SOP8(PA0/4)|TSSOP20(PA0/7)]: Generates signal

===== Control
- *[E]* - Enter or exit edit mode for pulse width.
- *[Y]* - Enter or exit edit mode for repeat count.
- *[T]* - Toggle signal generation mode (Pulse Up / Pulse Down).
- *[S]* - Send the configured signal.
- *[1-9]* - Enter digits for the value currently being edited (pulse width or repeat count).
- *[X]* - Delete the last entered digit for the value currently being edited.
- *[D]* - Send periodic signal.

===== Description
This page allows for the generation of digital signals on the output pin. The user can define the pulse width and the number of times the signal should repeat.

The signal generation has two modes, which can be toggled using the *[T]* key:
- *Pulse Up Mode*: In this mode, the output pin (PA0) defaults to a *LOW LEVEL*. When the signal is triggered using the *[S]* key, the pin will switch to a *HIGH LEVEL* for the duration specified by the "pulse width" setting.
- *Pulse Down Mode*: In this mode, the output pin (PA0) defaults to a *HIGH LEVEL*. When the signal is triggered using the *[S]* key, the pin will switch to a *LOW LEVEL* for the duration specified by the "pulse width" setting.

To set the pulse width, press the *[E]* key to enter edit mode, then use the number keys *[1-9]* to input the desired value. The *[X]* key can be used to delete digits. Press *[E]* again to confirm or exit edit mode.
Similarly, the repeat count (how many times the pulse is generated) can be set by pressing the *[Y]* key to enter edit mode for the repeat count, then using the number keys *[1-9]* and *[X]*. Press *[Y]* again to confirm or exit.

After configuring the desired parameters, pressing the *[S]* key will send the signal sequence.

After pressing *[D]* key, signal is generated permanetly with frequency showed on display. Frequency can be edit via period.

#v(10pt)
#figure(
  placement:none,
  caption: [GPIO SET Levels page],
  image("pic/tui_gen.png", width: 80%)
)
#v(10pt)
#pagebreak()


== Advanced Mode
This mode is available only on STM32G030 with packaging TSSOP20.

=== Page Logic Level

===== Pinout
- #highlight(fill: yellow)[TSSOP20(PA1/8)]: Toggle output
- #highlight(fill: green)[TSSOP20(PA2/9)]: Toggle output
- #highlight(fill: gray)[TSSOP20(PA3/10)]: Toggle output

===== Control
- *[1-3]* - Toggle output

===== Description
This page allows to toggle output pin.

#v(10pt)
#figure(
  placement:none,
  caption: [Generate signal page],
  image("pic/tui_levels.png", width: 80%)
)
#v(10pt)
#pagebreak()


=== Page SHIFT Register

===== Pinout
- #highlight(fill: red)[PA0/7]: Data signal output for the shift register.
- #highlight(fill: blue)[PB7/1]: Clock signal output for the shift register.
- #highlight(fill: yellow)[PA1/8]: RCLK signal, typically for *74HC595* compatible shift registers.

===== Control
- *[1-8]* - Toggle the bit value (0 or 1) at the corresponding position in the 8-bit sequence.
- *[S]* - Send all bits of the configured 8-bit sequence to the shift register.
- *[D]* - Send 1 bit selected by cursor of the configured 8-bit sequence to the shift register.
- *[F]* - Move cursor back to the start (green cursor)
- *[G]* - RCLK signal is sent

===== Description
This feature allows the user to define and transmit an 8-bit data sequence to an external shift register. The sequence of 1s and 0s can be constructed by pressing keys *[1]* through *[8]* to toggle the state of each respective bit.

Once the desired 8-bit sequence is set, pressing the *[S]* key will send it to the shift register. The data is sent via the `PA0` pin, synchronized with a clock signal on the `PB7` pin. For specific shift register models like the *74HC595*, the `PA1` pin can be used to control the RCLK, which dictates when data are put on the paralel outputs.
When *[D]* is pressed, bit selected by green color is send to the register.  *[F]* returns green cursors back to the start.

#v(10pt)
#figure(
  placement:none,
  caption: [Shift register page],
  image("pic/tui_register.png", width: 80%)
)
#v(10pt)
=== Page NEOPIXEL
==== Read communication

===== Pinout
- #highlight(fill: red)[PA0/7]: Neopixel data input (for Read mode).

===== Control
- *[M]* - Change operating mode (e.g., Read Neopixel / Write Neopixel).

===== Description
This feature provides an interface for interacting with Neopixel (WS2812B compatible) addressable LEDs, supporting both reading from and writing data to them. The operating mode can be switched by pressing the *[M]* key.

In Read mode, the device attempts to read data from a Neopixel signal connected to the `PA0` pin. If a signal is detected and successfully interpreted, the corresponding value or information is shown on the display. Values are showed as decimal value from 0-255 for every color element.

#v(10pt)
#figure(
  placement:none,
  caption: [Neopixel page],
  image("pic/tui_neopixel_read.png", width: 80%)
)
#v(10pt)
==== Send testing communication
===== Pinout
- #highlight(fill:fuchsia)[PB6/20]: Neopixel data output

===== Control
- *[0-9]* - Enter numerical value (e.g., 0-255 for color components).
- *[X]* - Delete the last entered digit or clear the current value.
- *[M]* - Change operating mode (e.g., Read Neopixel / Write Neopixel).
- *[K]* - Enter/exit edit mode for values, or select which Neopixel/color component to edit.
- *[L]* - Move cursor or navigate between editable fields (e.g., R, G, B components or individual pixels).
- *[S]* - Send the configured data to the Neopixel strip (in Write mode).

===== Description
This feature provides an interface for interacting with Neopixel (WS2812B compatible) addressable LEDs, supporting both reading from and writing data to them. The operating mode can be switched by pressing the *[M]* key.

In Write mode, the user can define the color for each RGB (Red, Green, Blue) component of one or more Neopixels. Each color component typically accepts a value from 0 to 255.
- Press *[K]* to enter edit mode for the color values or to select which specific Neopixel or color component (R, G, or B) is currently being edited.
- Use *[L]* to move the cursor or navigate between different editable fields or Neopixels.
- The numerical keys *[0-9]* are used to input the desired values (decimal 0-255) for the selected component, and *[X]* can be used to delete or correct the input.
Once the color data for the Neopixel(s) is configured as desired, pressing the *[S]* key transmits this data to the connected Neopixel strip via the `PB6` pin.

#v(10pt)
#figure(
  placement:none,
  caption: [Neopixel page],
  image("pic/tui_neopixel.png", width: 80%)
)
#v(10pt)
=== Page UART

==== Send testing data 

===== Pinout
- #highlight(fill:green)[PA2/9 (MCU TX)]: UART Transmit pin.

===== Control
- Settings mode
    - *[T]* - Enter or exit the UART Settings mode.
    - *[0-9]* - Edit baudrate (add digit to the end).
    - *[X]* - Delete the last entered digit of baudrate.
    - *[O]* - Edit the 'SEND BYTES' parameter (number of bytes to send in Write mode).
    - *[U]* - Select Parity.
    - *[Y]* - Select Word Length.
    - *[I]* - Select/edit Number of Stop Bits (in Settings mode).
- Edit send value
    - *[,]* - Enter/exit edit values mode.
    - *[.]* - Move cursor to next byte
    - *[Any]* - set letter to selected byte
- *[M]* - Cycle between Read and Write.
- *[S]* - Send the configured data bytes (in Write mode).
- *[G]* - Reset function (incase of freeze)

===== Description
This feature provides a Universal Asynchronous Receiver/Transmitter (UART) interface for serial communication, allowing the device to send data. It also includes a mode for configuring the UART parameters. 

This mode allows the user to compose and transmit a data message.
- A buffer of up to 10 bytes is available for composing the message.
- Use *[T]* to entry Settings mode, where user can set baudrate, number of sent bytes, wordlen, parity etc.
- Use *[,]* to enter edit value mode and by pressing any letter will be written to the byte. Use *[.]* to move to the next byte (GREEN TEXT is cursor)
- Pressing *[S]* sends the specified number of data bytes via the `PA2 (MCU TX)` pin.

*Settings Mode:*
Accessed by pressing the *[T]* key, this mode allows configuration of the UART communication parameters to match the connected device:
- *Baud Rate*: Can be entered by *[0-9]* and deleted by *[X]*.
- *Word Length*: Configured using the *[Y]* key to cycle through options (e.g., 7, 8, or 9 bits).
- *Parity*: Configured using the *[U]* key to select the parity type (e.g., None, Even, Odd).
- *Number of Stop Bits*: Configured using the *[I]* key to select the number of stop bits (e.g., 1 or 2).

#v(10pt)
#figure(
  placement:none,
  caption: [UART write page],
  image("pic/tui_uart_write.png", width: 80%)
)
#v(10pt)

==== Monitor UART

===== Pinout
- #highlight(fill:gray)[PA3/10 (MCU RX)]: UART Receive pin.

===== Control
- Settings mode
    - *[T]* - Enter or exit the UART Settings mode.
    - *[0-9]* - Edit baudrate (add digit to the end).
    - *[X]* - Delete the last entered digit of baudrate.
    - *[O]* - Edit the 'SEND BYTES' parameter (number of bytes to send in Write mode).
    - *[U]* - Select Parity.
    - *[Y]* - Select Word Length.
    - *[I]* - Select/edit Number of Stop Bits (in Settings mode).
- *[M]* - Cycle between Read and Write.
- *[G]* - Reset function (incase of freeze)

===== Description
This feature provides a Universal Asynchronous Receiver/Transmitter (UART) interface for serial communication, allowing the device to monitor actual comunication. It also includes a mode for configuring the UART parameters. 

In this mode, the device listens for and displays incoming characters or data bytes received on the `PA3 (MCU RX)` pin.

*Settings Mode:*
Accessed by pressing the *[T]* key, this mode allows configuration of the UART communication parameters to match the connected device:
- *Baud Rate*: Can be entered by *[0-9]* and deleted by *[X]*.
- *Word Length*: Configured using the *[Y]* key to cycle through options (e.g., 7, 8, or 9 bits).
- *Parity*: Configured using the *[U]* key to select the parity type (e.g., None, Even, Odd).
- *Number of Stop Bits*: Configured using the *[I]* key to select the number of stop bits (e.g., 1 or 2).

#v(10pt)
#figure(
  placement:none,
  caption: [UART send page],
  image("pic/tui_uart_read.png", width: 80%)
)
#v(10pt)
#pagebreak()
=== Page I2C
==== Address scanner
===== Pinout
- #highlight(fill: blue)[PB8/1 (SCL)]: I2C Clock line (SCL).
- #highlight(fill: black)[#text(fill:white)[PB9/2 (SDA)]]: I2C Data line (SDA).

===== Control
- *[M]* - Changes the I2C operating mode (to select address scanning mode).

===== Description
This function is used to detect active devices connected to the I2C bus. After selecting the address scanning mode (using the *[M]* key until the corresponding screen is displayed). 

Scanning I2C bus addresses is a fundamental diagnostic step. This function allows the user to verify whether connected devices are communicating correctly and are detectable on the bus. 
#v(10pt)
#figure(
  placement:none,
  supplement:[],

  caption: [Example: Found address `0X3C`],
  image("pic/tui_i2c_scan.png")
)
#v(10pt)

==== I2C Master

===== Pinout
- #highlight(fill: blue)[PB8/1 (SCL)]: I2C Clock line (SCL).
- #highlight(fill: black)[#text(fill:white)[PB9/2 (SDA)]]: I2C Data line (SDA)..

===== Control
- Settings mode
    - *[T]* - Enter/exit settings mode  
    - *[0-F]* - Enter hexadecimal digits (0-9, A-F) for I2C address.
    - *[X]* - Delete the last entered hexadecimal digit of I2C address.
    - *[Y]* - change the number of data bytes to be sent or received.
    - *[U]* - Switch between Read and Write operations.
- Edit send value
    - *[0-F]* - Enter hexadecimal digits (0-9, A-F) for selected byte.
    - *[X]* - Delete the last entered hexadecimal digit of selected byte.
    - *[K]* - Enter/exit edit mode for values of data bytes.
    - *[L]* - Move cursor to navigate between bytes positions.
- *[M]* - Cycle through the different I2C operating modes.
- *[S]* - send/receive data (Master mode)
- *[G]* - Reset function (incase of freeze)




===== Description
This feature provides an Inter-Integrated Circuit (I2C) interface, allowing the device to communicate with other I2C devices. User can change settings like address, and read write mode

The device acts as an I2C master, initiating communication with slave devices on the bus.
- The user must specify the target slave address to communicate with (edited in Settings mode *[0-F]*, *[X]*).
- The *[U]* key is used to toggle between Read (master receives data from slave) and Write (master sends data to slave) operations in Settings mode.
- The *[Y]* key allows setting the number of data bytes to be transmitted or received in Settings mode.
- For write operations, data bytes are entered using *[K]*, *[L]*, *[0-F]*, *[X]*.
- Pressing *[S]* initiates the configured data transfer (read or write) with the specified slave device.
#v(10pt)
#figure(
  placement:none,
  supplement:[],
  caption: [Example: Master send `0x30` with read bit, got replied `0X43` from slave],
  image("pic/tui_i2c_write.png")
)
#v(10pt)
==== I2C Monitor

===== Pinout
- #highlight(fill: aqua)[PA7/14]: SDA.
- #highlight(fill: black)[#text(fill:orange)[PA5/12]]: SCL

===== Control
- *[M]* - Cycle through the different I2C operating modes.
- *[G]* - Reset function (incase of freeze)

===== Description
This feature provides an Inter-Integrated Circuit (I2C) interface, allowing the device to monitor communication between devices on the bus. To reset comunication press *[G]*.

#v(10pt)
#figure(
  placement:none,
  supplement:[],
  caption: [Example: Master send `0x30` with read bit, got replied `0X43` from slave],
  image("pic/tui_i2c_monitor.png")
)
#v(10pt)

==== I2C SSD1306 display test

===== Pinout
- #highlight(fill: blue)[PB8/1 (SCL)]: I2C Clock line (SCL).
- #highlight(fill: black)[#text(fill:white)[PB9/2 (SDA)]]: I2C Data line (SDA)

===== Control
- Settings mode
    - *[T]* - Enter/exit settings mode  
    - *[0-F]* - Enter hexadecimal digits (0-9, A-F) for I2C address.
    - *[X]* - Delete the last entered hexadecimal digit of I2C address.
- *[M]* - Cycle through the different I2C operating modes.
- *[S]* - start test
- *[G]* - Reset function (incase of freeze)

===== Description
This is a specialized mode for testing SSD1306 OLED displays.
    - Upon entering this mode and pressing the *[S]* key, the device sends pre-defined commands to an SSD1306 display (which is expected to be at a known I2C address or found via scan) to activate all pixels. This provides a quick visual confirmation of the display's operation. The device acts as an I2C master, initiating communication with slave devices on the bus.
- The user must specify the target slave address to communicate with (edited in Settings mode *[0-F]*, *[X]*).
- Pressing *[S]* initiates the configured data transfer (read or write) with the specified slave device.
#v(10pt)
#figure(
  placement:none,
  supplement:[],
  caption: [Example: display i2c test page],
  image("pic/tui_i2c_ssd1306.png")
)
#v(10pt)

#pagebreak()

=== Page SPI
==== Master mode

===== Pinout
- #highlight(fill: orange)[PA5/12 (SCK)]: SPI Clock line.
- #highlight(fill: rgb("#ba0000"))[#text(fill: white)[PA6/13 (MISO)]]: SPI Master In, Slave Out line.
- #highlight(fill: aqua)[PA7/14 (MOSI)]: SPI Master Out, Slave In line.
- #highlight(fill: purple)[PB0/15 (NSS)]: SPI Chip Select / Slave Select line.

===== Control
- Settings mode
    - *[T]* - Enter/exit settings mode for additional SPI parameters.
    - *[Y]* - Toggle SPI Clock Phase (CPHA).
    - *[U]* - Toggle SPI Clock Polarity (CPOL).
    - *[P]* - Toggle Read/Write operation.
    - *[I]* - Edit the number of data bytes to send or receive.
- Edit values mode
    - *[0-F]* - Enter hexadecimal digits (0-9, A-F) for data bytes.
    - *[X]* - Delete the last entered hexadecimal digit or clear the current value.
    - *[L]* - Move cursor to navigate between editable fields (e.g., data bytes).
    - *[K]* - Enter/exit edit mode for data bytes or other parameters.
- *[M]* - Cycle through SPI operating modes (e.g., Slave, Master, SSD1306 Test).
- *[S]* - Initiate data transfer.
- *[G]* - Reset function (incase of freeze)


===== Description
This feature provides a Serial Peripheral Interface (SPI) for communication, enabling the device to act as an SPI master. Data is typically entered in hexadecimal format using keys *[0-F]* and *[X]*. The *[K]* and *[L]* keys facilitate editing and navigation. Key SPI communication parameters such as Clock Polarity (CPOL) and Clock Phase (CPHA) can be directly adjusted using the *[U]* and *[Y]* keys, respectively In Settings mode.

The device acts as an SPI master, initiating and controlling communication with SPI slave devices.
- The user can configure data to be written to the slave or specify data to be read.
- The *[P]* key toggles between Read and Write operations in Settings mode.
- The number of bytes for the transfer can be set using the *[I]* key in settings mode.
- SPI Clock Polarity (CPOL) and Clock Phase (CPHA) can be configured using the *[U]* and *[Y]* keys to ensure compatibility with the slave device's requirements in settings mode.
- Data bytes for writing are typically entered using *[K]* to select/edit, *[L]* to navigate (if multiple bytes), and *[0-F]* *[X]* for value input.
- Pressing *[S]* initiates the data transfer with the connected SPI slave device.
#v(10pt)
#figure(
  placement:none,
  supplement:[],
  caption: [Example: Master send `0x25 0x02 0xFF 0x08` to slave device],
  image("pic/tui_spi_master.png")
)
#v(10pt)


==== Monitor

===== Pinout
- #highlight(fill: orange)[PA5/12 (SCK)]: SPI Clock line.
- #highlight(fill: rgb("#ba0000"))[#text(fill: white)[PA6/13 (MISO)]]: SPI Master In, Slave Out line.
- #highlight(fill: aqua)[PA7/14 (MOSI)]: SPI Master Out, Slave In line.

===== Control
- Settings mode
    - *[T]* - Enter/exit settings mode for additional SPI parameters.
    - *[Y]* - Toggle SPI Clock Phase (CPHA).
    - *[U]* - Toggle SPI Clock Polarity (CPOL).
    - *[I]* - Edit the number of data bytes to send or receive.
- *[M]* - Cycle through SPI operating modes (e.g., Slave, Master, SSD1306 Test).
- *[G]* - Empty buffer

===== Description
This feature provides a Serial Peripheral Interface (SPI) for communication, enabling the device to act as an SPI slave (monitoring device). Data is typically showe as hexadecimal format Key SPI communication parameters such as Clock Polarity (CPOL) and Clock Phase (CPHA) can be directly adjusted using the *[U]* and *[Y]* keys, respectively In Settings mode.

In this mode, the device emulates an SPI slave. It will listen to an external SPI master and respond accordingly. Data received from the master can be viewed, and data to be sent to the master (when the master initiates a read) can often be pre-loaded or configured.
- SPI Clock Polarity (CPOL) and Clock Phase (CPHA) can be configured using the *[U]* and *[Y]* keys to ensure compatibility with the slave device's requirements in settings mode.

#v(10pt)
#figure(
  placement:none,
  supplement:[],
  caption: [Example: Monitor detected `0x25 0x02 0xFF 0x08` communication],
  image("pic/tui_spi_monitor.png")
)
#v(10pt)

==== SPI SSD1306 display test

===== Pinout
- #highlight(fill: orange)[PA5/12 (SCK)]: SPI Clock line.
- #highlight(fill: rgb("#ba0000"))[#text(fill: white)[PA6/13 (MISO)]]: SPI Master In, Slave Out line.
- #highlight(fill: aqua)[PA7/14 (MOSI)]: SPI Master Out, Slave In line.
- #highlight(fill: purple)[PB0/15 (NSS)]: SPI Chip Select / Slave Select line.

===== Control
- Settings mode
    - *[T]* - Enter/exit settings mode  
    - *[Y]* - Toggle SPI Clock Phase (CPHA).
    - *[U]* - Toggle SPI Clock Polarity (CPOL).
- *[S]* - start test
- *[G]* - Reset function (incase of freeze)

===== Description
This is a specialized mode for testing SSD1306 OLED displays.
    - Upon entering this mode and pressing the *[S]* key, the device sends pre-defined commands to an SSD1306 display via SPI to activate all pixels. This provides a quick visual confirmation of the display's operation. The device acts as an SPI master, initiating communication with slave device.
- Pressing *[S]* initiates the configured data transfer (read or write) with the specified slave device.
#v(10pt)
#figure(
  placement:none,
  supplement:[],
  caption: [Example: display i2c test page],
  image("pic/tui_spi_ssd1306.png")
)
#v(10pt)


