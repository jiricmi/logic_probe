#import "./template/template/template.typ": *
#import "@preview/fletcher:0.5.4" as fletcher: diagram, node, edge
#set math.equation(numbering: "(1)")
// add pwsh syntax highlighting and make it the default lang for blocks

#show: template.with(
  meta: (
    title: "Manual for Raspberry Pi Pico diagnostic logic probe", author: (
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
#figure(
    caption:text(size: 15pt)[1. Put Raspberry Pi Pico to edge of the breadboard with GPIO pins supporting ADC facing inside],
    supplement: [],
    placement: none,
    numbering: none,
    outlined: false,
    image("pic/rpi_real.jpg")
)





== Firmware setup
1. Connect RPI Pico to the the PC via USB
2. Check if new external storage was added 
3. Move firmware file (logic_probe_rpi.u2f) to external storage
4. Reconnect device

= Terminal mode
This mode enables deeper circuit diagnostic with PC control. It offers the following features:
    - *Voltage measure*
    - *Logic Level measure*
    - *Resistence measure*
    - *Manual logic Level Control*
    - *Frequency measure*
    - *Pulse detection*
=== Activation
Terminal mode is enabled when the device is powered with UART computer connection. To activate it:
1. Raspberry Pi Pico to PC via USB cable.
2. Led will flash green.
3. Connect to device via serial communication. Open PuTTY app and connect to specific COM (WIN) or /dev/ttyUSBx (Linux) with `115200` baudrate 8N1 (@putty-app)
4. After connection press *R* to start

#figure(
    caption: [PuTTY app],
    image("pic/putty_settings.png"),

)<putty-app>

== Pinout
#figure(
    placement: none,
        caption:[Picture from RPI manual\ (https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf)],
        image("pic/rpi_device.jpg.png"))

== Universal keys
- *[R]* - reload page to fix broken ansi sequences
- *[Q]* - return to menu
#pagebreak()
== MENU
By pressing letter on keyboard written next to name, function can be selected.
#v(10pt)
#figure(
        placement: none,
        caption:[Basic menu],
        image("pic/tui_main.png"))
#v(10pt)



== Basic Mode
=== Page Voltmeter/Ohmmeter
==== Voltmeter and Logic Levels
===== Pinout
- (GP26/31): Measures Channel 1
- (GP27/32): Measures Channel 2
- (GP28/34): Measures Channel 3

===== Control
- *[1-3]* - Used to turn the respective measurement channel on or off.
- *[S]* -Confirms (saves) the current on/off settings for the channels.
- *[M]* - Switches between Voltmeter and Ohmmeter mode.

==== Description
The voltmeter allows measurements on multiple channels. Different device versions may vary in the number of available measurement channels. 

In addition to the measured value, an indicator of its logic level is displayed for each channel:
- If *H* is displayed: The voltage on the channel corresponds to a High level. 
- If *L* is displayed: The voltage on the channel corresponds to a Low level.
- If *?* is displayed: The channel status is indeterminate (e.g., the input is floating, or in a transitional state).
- If the symbol *x* is displayed instead of a numerical voltage value for a channel, it means that the channel is currently turned off.

_Example_: To turn off channel 1, press the [1] key. To confirm and save this change, subsequently press the [S] key
#figure(
    placement:none,
    caption: [Voltmeter page],
  image("pic/tui_voltmetr_rpi.png", width: 80%)
)

==== Ohmmeter
===== Pinout
- (GP26/31): Measures Channel 1

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
  image("pic/tui_ohm_rpi.png", width: 80%)
)
#v(10pt)


#pagebreak()
=== Page Signal detection
==== Frequency

===== Pinout
- (GP21/27): Measures frequency

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
  image("pic/tui_freq_rpi.png", width: 80%)
)

==== Pulse Detection

===== Pinout
- (GP21/27): Measures frequency

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
- (GP17/22): Generates signal
- (GP16/21): Generates permanent frequency
===== Control
- *[E]* - Enter or exit edit mode for pulse width.
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
S
After configuring the desired parameters, pressing the *[S]* key will send the signal sequence.

After pressing *[D]* key, signal is generated permanetly with frequency showed on display. Frequency can be edit via period.

#v(10pt)
#figure(
  placement:none,
  caption: [GPIO SET Levels page],
  image("pic/tui_gen_rpi.png", width: 80%)
)
#v(10pt)
#pagebreak()
=== Page Logic Level

===== Pinout
- (GP18/24): Toggle output
- (GP19/25): Toggle output
- (GP20/26): Toggle output

===== Control
- *[1-3]* - Toggle output

===== Description
This page allows to toggle output pin.

#v(10pt)
#figure(
  placement:none,
  caption: [Generate signal page],
  image("pic/tui_levels_rpi.png", width: 80%)
)
#v(10pt)


