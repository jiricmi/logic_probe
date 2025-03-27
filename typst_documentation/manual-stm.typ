#import "./template/template/template.typ": *
#import "@preview/fletcher:0.5.4" as fletcher: diagram, node, edge
#set math.equation(numbering: "(1)")
// add pwsh syntax highlighting and make it the default lang for blocks

#show: template.with(
  meta: (
    title: "Manual for diagnostic logic probe", author: (
      name: "Milan Jiříček", email: "jiricmi1@fel.cvut.cz", url: "https://github.com/jiricmi/logic_probe",
    ), bachelor: true, diff_usage: true, supervisor: "doc. Ing. Jan Fischer, CSc.", faculty: "Faculty of Electrical Engineering", department: "Deparment of measurement", study-programme: "Open Informatics",
  ), print: false, lang: "en",
  abstract-en: [
     ],
  abstract-cz: [
      ],
  acknowledgement: [
  ],
)

#outline(depth: 3)

= Build
== STM32G030 SOP8
Zatím jen zrychla, zde budou obrázky
- PIN 1 slouží jako kanál pro nesložité věci, PB7
- PIN 4 slouží jako kanál PA0, nejlepší pin POZOR JE ZDE NRESET
- PIN 2 a 3 napájení
- PIN 5 a 6 UART periferie, na TX pull down resistor 10k
- PIN 7 tlačítko, které na pin přivádí 3,3V. Interní pulldown
- PIN 8 RGB NEOPIXEL

== STM32G030 TSSOP20
== Raspberry Pi Pico
= Firmware
== Local mode
This mode is used to quickly identify the problem without having to connect to a desktop computer. It allows the following actions:
    - *analysing circuit by simple logic probe*
    - *detecting rise or fall edge*
    - *setting logic levels on channels*
By holding the button for a long time (approx. 500 ms) it is possible to switch between modes. The modes are differentiated by RGB LEDs, where the colour of the mode is displayed when the button is released.

=== How to enable
Local mode is enabled by the user connecting the assembled circuit only to the power supply. This means that the mode is activated when after switching on the device it detects a *logic zero* on the *recieve pin*, which is intended for communication with the computer. When the communication with the computer is connected, the default is logic one. 

The activation of the mode can be detected so that when switched on, a sequence of colours on the RGB LEDs is triggered.

== Logic probe mode
- LED Color:#highlight(fill: orange)[ORANGE]
- Channels are switched with *button press*. After press, LED shows color of the selected channel.
    - Channel 1: #highlight(fill:purple)[PURPLE]
    - Channel 2: #highlight()[YELLOW]
- LED indicates if state is HIGH by #highlight(fill: green)[GREEN] color, LOW state by #highlight(fill: red)[RED] color, and interstate by led turned off. 
- *SOP8* - logic probe can be use on PB7(CH1) and PA0(CH2) pins.
== Logic levels
- LED Color: #highlight(fill: white)[WHITE]
- Channels are switched with *button double press*. After double press, LED shows color of the selected channel.
    - Channel 1: #highlight(fill:purple)[PURPLE]
    - Channel 2: #highlight()[YELLOW]
- Logic levels are changed with *button press*. After press, level on channel is toggled. State of the channel is indicated by LED.

- *SOP8* - logic levels can be toggled on PB7(CH1) and PA0(CH2) pins.
#pagebreak
== Rise edge detection
- LED Color: #highlight(fill: purple)[PURPLE]
- PA0 is used for detection of rising edge
- if edge is detected, #highlight(fill: green)[Green] color is indicated. After one second led is turned off.

== Falling edge detection
- LED Color: #highlight(fill: aqua)[AQUA]
- PA0 is used for detection of falling edge
- if edge is detected, #highlight(fill: green)[Green] color is indicated. After one second led is turned off.

== Terminal mode
This mode is used for deeper identification of the problem.
=== Universal keys
- *[R]* - reload page to fix broken ansi sequences
- *[Q]* - return to menu

=== How to enable
Terminal mode is enabled by connecting device through converter. After connecting device to USB, open terminal which supports ANSI sequences. Recommended application is *PUTTY*.
#image("pic/putty_settings.png")
Select **Serial** as connection type. Enter *115200* as **Speed** of the serial communication and enter correct type of serial line. Usually on linux `/dev/ttyUSB0`.

After connection, press letter `R` on keyboard and menu appear.
=== MENU
Doplním podrobněji po změně meny na jednoduché a advanced menu
=== Voltmeter/Ohmmeter
Voltmeter shows different channels. Versions differs in the number of channels. Page shows **Reference voltage**, which is used for calibrating channel voltage.
- *[1-4]* - Toggle channel on or off
- *[S]* - Apply settings of toggling channels
- *[M]* - switch between Voltmeter and ohmmeter

Red arrow points to the logic level of voltage. 
- If shows *H*, channel is in high level
- If shows *L*, channel is in low level
- If shows *?*, channel is in interstate
If channel has *x* instead of value, it is turned off. To turn off or turn on the channel, it is nesessary to press key of the number.

*Example*: For turn off the channel 1 press *1* on the keyboard and then *S* for saving the value 
#image("pic/voltmeter_manual.png", width: 100%)
==== Ohmmeter
By pressing *M*, mode is changed to *Ohmmeter mode*. Ohmmeter page, illustrate, how to connect resistor to measure Ohms. Next to *Vcc* is showed, what reference voltage is used.
R1 resistor is based. this resistor can be changed by pressing *E*. Value can be entered with number keys and delete with *X*. In green field is calculated value of measured resistor.
- *[E]* - edit base resistor and exit edit mode
- *[1-9]* - changing value
- *[X]* - delete number

#image("pic/ohmmeter_manual.png", width: 80%)
=== Signal detect
==== Frequency
Frequency is measured on pin PA0. Device can measure, frequency, width of pulse and duty time.
By key *T*, user can change gate time.
- *[T]* - change gate time
- *[M]* - change mode
#image("pic/signal_detect_manual.png")

==== Pulse
Pressing *M* will change mode to pulse up or down mode. Pulse up will catch rising edge and pulse down will catch falling edge. If edge is catched, Flag will be changed to #highlight(fill: green)[TRUE].
To delete flag, press *D*.
- *[D]* - delete flag of catched signal
- *[M]* - change mode
#image("pic/signal_detect_pulse_manual.png")
=== Signal generator
- *[E]* - edit pulse width or end edit mode
- *[T]* - toggle mode from pulse up to pulse down
- *[S]* - send signal
- *[1-9]* - change pulse width
- *[X]* - delete pulse width number
On PA0 pin can be send signal with specific pulse width. Width can be edited by *E*.
There are two modes. When pulse up is choosed, default pin setup is *LOW LEVEL*. When signal is sent *HIGH LEVEL* is set for specified time. 
When pulse down is choosed, default pin setup is *HIGH LEVEL* and when signal is sent *LOW LEVEL* is set for specific time.
#image("pic/signal_generator_manual.png")
=== NEOPIXEL
Neopixel perif, can be read or wrote.
Modes can be changed by pressing *M*.
Neopixel is read on pin PA0. When signal is detected. Value is showed on the display.

On write, user can choose color from 0 to 255 for every color element.
By pressing *S* selected signal will be sent.
- *[M]* - Change mode
- *[E]* - Edit R or exit edit mode
- *[T]* - Edit G or exit edit mode
- *[Y]* - Edit B or exit edit mode
- *[S]* - Send neopixel data
#image("pic/neopixel_manual.png", width: 60%)
=== SHIFT Register
Pin PA0 is used for send data and pin PB7 is used as clock signal.

User can change sequence for shift register which will be sent.
- *[1-8]* - Toggle 1 or 0 on specified position
- *[S]* - send sequence

#image("pic/shift_register_manual.png")
