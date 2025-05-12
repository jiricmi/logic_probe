#import "./template/template/template.typ": *
#import "@preview/fletcher:0.5.4" as fletcher: diagram, node, edge
#set math.equation(numbering: "(1)")
// add pwsh syntax highlighting and make it the default lang for blocks

#show: template.with(
  meta: (
    title: "Manual for STM32 diagnostic logic probe", author: (
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

#set par(first-line-indent: 0em, spacing: 2.5em, justify: true)

= Build
== STM32G030 SOP8
#todo[doplnit podrobnosti]
#v(10pt)
#grid(
    columns: 2,
    figure(
        placement:none,
        caption:[Diagram SOP8 ve Fritzingu],
        image("pic/sop8_hw_diagram.jpg")
    ),
    figure(
        placement:none,
        caption:[Diagram SOP8],
        image("pic/sop8_hw.png")
    )
)
#v(10pt)

== STM32G030 TSSOP20
#v(10pt)
#grid(
    columns: 2,
    figure(
        placement:none,
        caption:[Diagram TSSOP20 ve Fritzingu],
        image("pic/tssop20_hw_diagram.jpg")
    ),
    figure(
        placement:none,
        caption:[Diagram TSSOP20],
        image("pic/tssop20_hw.png")
    )
)
#v(10pt)
= Local mode
This mode enables quick circuit diagnostics without requiring a computer connection. It offers the following features:
- *Logic Probe*: Instant visualization of signal states (HIGH/LOW) on connected channels.
- *Edge Detection*: Recognition of both rising edge and falling edge signal transitions.
- *Pulse Generation*: Continuous pulsing
- *Manual Logic Level Control*: Set fixed HIGH/LOW levels on selected channels.
To switch between modes, hold the button for approximately 500 ms. The RGB LEDs indicate the active mode by displaying its assigned color when the button is released.
=== Activation
Local mode is enabled when the device is powered without UART computer connection. To activate it:
1. Connect only the power supply to the assembled circuit (no data/USB connection).
2. Upon startup, the device detects a logic LOW (0) on the receive (RX) pin, which is reserved for computer communication.
    - _Note_: When connected to a computer, the RX pin defaults to logic HIGH (1), disabling Local mode.
3. A startup RGB LED color sequence (e.g., alternating green-blue-red) will play to visually confirm Local mode is active.

== Logic probe mode
This mode is designed for diagnosing logic circuits. It enables real-time monitoring of logic states (HIGH/LOW) across two independent channels via color-coded LED indicators. The channels are assigned to pins PB7 (Channel 1) and PA0 (Channel 2). Ideal for quick signal verification.
===== LED Behavior:
- Mode indicator: #highlight(fill: aqua)[AQUA]
- *Channel Selection*: 
    - Toggle Channel 1 and Channel 2 by _pressing_ the button.
    - Selected channel is confirmed by the LED color:
        - *Channel 1*: #highlight(fill:purple)[PURPLE]
        - *Channel 2*: #highlight(fill:yellow)[YELLOW]
- *State Detection*:
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
- LED Color: #highlight(fill: white)[WHITE]
- PA0 is used for generating pulses 
- after *button press* generation is turned on, another press generation is turned off. While on LED is WHITE.

== Rise edge detection
- LED Color: #highlight(fill: blue)[BLUE]
- PA0 is used for detection of rising edge
- if edge is detected, #highlight(fill: green)[Green] color is indicated. After one second led is turned off.

== Falling edge detection
- LED Color: #highlight(fill: purple)[Purple]
- PA0 is used for detection of falling edge
- if edge is detected, #highlight(fill: green)[Green] color is indicated. After one second led is turned off.

= Terminal mode
This mode is used for deeper identification of the problem.
== Universal keys
- *[R]* - reload page to fix broken ansi sequences
- *[Q]* - return to menu

== How to enable
Terminal mode is enabled by connecting device through converter. After connecting device to USB, open terminal which supports ANSI sequences. Recommended application is *PUTTY*.
#v(10pt)
#figure(
    placement:none,
    caption:[Putty settings],
    image("pic/putty_settings.png")
)
#v(10pt)
Select **Serial** as connection type. Enter *115200* as **Speed** of the serial communication and enter correct type of serial line. Usually on linux `/dev/ttyUSB0`.

After connection, press letter `R` on keyboard and menu appear.
#pagebreak(weak:true)
== PINOUT
Colors of wires show location of connect devices. If some text is colored, like #highlight(fill: red)[PA0], that is pin 4(SOP8)/pin 7(TSSOP20).
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
=== Voltmeter/Ohmmeter
==== Voltmeter
Voltmeter shows different channels. Versions differs in the number of channels. Page shows *Reference voltage*, which is used for calibrating channel voltage.
- In paranthesses is shown which pin is used as channel.
- *[1-4]* - Toggle channel on or off
- *[S]* - Apply settings of toggling channels
- *[M]* - switch between Voltmeter and ohmmeter

Red arrow points to the logic level of voltage. 
- If shows *H*, channel is in high level
- If shows *L*, channel is in low level
- If shows *?*, channel is in interstate
If channel has *x* instead of value, it is turned off. To turn off or turn on the channel, it is nesessary to press key of the number.

*Example*: For turn off the channel 1 press *1* on the keyboard and then *S* for saving the value.
#figure(
    placement:none,
    caption: [Voltmeter page],
  image("pic/tui_voltmetr.png", width: 80%)
)

@sop_voltage and @tssop_voltage shows, which pins are used for channels. SOP8 has #highlight(fill: blue)[channel 1(PB7)] and #highlight(fill: red)[channel 2(PA0)], but TSSOP20 has also #highlight(fill: yellow)[channel 3(PA1)] and #highlight(fill: green)[channel 4(PA2)].

==== Ohmmeter
By pressing *M*, in mode is changed to *Ohmmeter mode*. Ohmmeter page, illustrate, how to connect resistor to measure Ohms. Next to *Vdd* is showed, what reference voltage is used.
R1 resistor is based. this resistor can be changed by pressing *E*. Value can be entered with number keys and delete with *X*. In green field is calculated value of measured resistor. Resistance is measured on #highlight(fill: blue)[Channel 1(PB7)].
- *[E]* - edit base resistor and exit edit mode
- *[0-9]* - changing value
- *[X]* - delete number
- *[M]* - change mode to voltmeter
#v(10pt)
#figure(
    placement:none,
    caption: [Ohmmeter page],
  image("pic/tui_ohm.png", width: 80%)
)
#v(10pt)

=== Signal detect
==== Frequency
Frequency is measured on pin #highlight(fill: red)[PA0]. Device can measure, frequency, reciprocial frequency#footnote[Frequency and reciprocial frequency difference is in way of calculating.], width of pulse and duty time.
By key *T*, user can change gate time.
- *[T]* - change gate time
- *[M]* - change mode
#figure(
    placement:none,
    caption: [Frequency page],
  image("pic/tui_freq.png", width: 80%)
)

===== Pulse
Pressing *M* will change mode to pulse up or down mode. Pulse up will catch rising edge and pulse down will catch falling edge. If edge is catched, Flag will be changed to TRUE. To delete flag, press *D*. Pulse is catched on #highlight(fill: red)[PA0].

- *[D]* - delete flag of catched signal
- *[M]* - change mode
#v(10pt)
#figure(
    placement:none,
    caption: [Pulse page],
  image("pic/tui_pulse.png", width: 80%)
)
#v(10pt)

=== Signal generator
- *[E]* - edit pulse width or end edit mode
- *[Y]* - edit repeat count or end edit mode
- *[T]* - toggle mode from pulse up to pulse down
- *[S]* - send signal
- *[1-9]* - change pulse width
- *[X]* - delete pulse width number
On #highlight(fill: red)[PA0] pin can be send signal with specific pulse width. Width can be edited by *E*.
There are two modes. When pulse up is choosed, default pin setup is *LOW LEVEL*. When signal is sent *HIGH LEVEL* is set for specified time. 
When pulse down is choosed, default pin setup is *HIGH LEVEL* and when signal is sent *LOW LEVEL* is set for specific time. By changing repeat counter, user can generate signal x times.
#v(10pt)
#figure(
    placement:none,
    caption: [Generate signal page],
  image("pic/tui_gen.png", width: 80%)
)
#v(10pt)

== Advanced Mode
This function is only on *TSSOP20*!
=== SHIFT Register
Pin #highlight(fill: red)[PA0] is used for send data and pin #highlight(fill: blue)[PB7] is used as clock signal. In case of *74HC595* pin #highlight(fill: yellow)[PA1] is used as Output enable.

User can change sequence for shift register which will be sent.
- *[1-8]* - Toggle 1 or 0 on specified position
- *[S]* - send sequence

#v(10pt)
#figure(
    placement:none,
    caption: [Neopixel page],
  image("pic/tui_register.png", width: 80%)
)
#v(10pt)
=== NEOPIXEL
Neopixel perif, can be read or wrote.
Modes can be changed by pressing *M*.
Neopixel is read on pin #highlight(fill: red)[PA0]. When signal is detected. Value is showed on the display. Data can be send to Neopixel by pin #highlight(fill:fuchsia)[PA13]. 

On write, user can choose color from 0 to 255 for every color element.
By pressing *S* selected signal will be sent.
- *[0-9]* - Change value
- *[X]* - Delete value
- *[M]* - Change mode
- *[K]* - Edit values
- *[L]* - Move cursor
- *[S]* - Send neopixel data
#v(10pt)
#figure(
    placement:none,
    caption: [Neopixel page],
  image("pic/tui_neopixel.png", width: 80%)
)
#v(10pt)

=== UART
UART peripheral, can be read and write.
Modes can be changed by pressing *M*.
MCU #highlight(fill:green)[TX] and MCU #highlight(fill: gray)[RX] is connected to PA2 and PA3. Read mode shows letters which are accepted. Write mode has 10 bytes and parameter `SEND BYTES` set how many bytes are sent. In settings mode, user can change word length, parity, number of stopbits and baudrate. In value mode and settings mode can be edit by numkeys and letter `X`.
By pressing *S* selected signal will be sent.
- *[0-9]* - Change value
- *[X]* - Delete value
- *[M]* - Change mode
- *[K]* - Edit values
- *[L]* - Move cursor
- *[S]* - Send data
- *[T]* - Edit settings
- *[O]* - Count bytes to send
- *[Y]* - Word length
- *[U]* - Switch between parity
- *[I]* - Number of stopbit
#v(10pt)
#figure(
    placement:none,
    caption: [UART page],
  image("pic/tui_uart.png", width: 80%)
)
#v(10pt)

=== I2C
I2C peripheral can be read and write.
Modes can be changed by pressing *M*.
#highlight(fill: blue)[SCL] use PB8 and #highlight(fill: black)[#text(fill:white)[SDA]] use PB9.

First mode is address scan, where probe shows, which adress on I2C is active. Second mode is slave mode, where probe is behaving as slave. User can set I2C address of the slave. Third mode is master mode, where probe is behaving as master. To selected adress, probe send entered data by user. Fourth mode is testing *SSD1306* display, where after pressing *S*, display set all pixels active.
- *[0-F]* - Change hex value
- *[X]* - Delete value
- *[M]* - Change mode
- *[K]* - Edit values
- *[L]* - Move cursor
- *[S]* - Send data
- *[T]* - Edit settings
- *[Y]* - Change bytes to send
- *[U]* - Switch read write
#v(10pt)
#grid(
    columns: 2,
    figure(
        placement:none,
        caption: [I2C Scan address page],
        image("pic/sop8_i2c_scan.png")
    ),
    figure(
        placement:none,
        caption: [I2C Slave page],
        image("pic/sop8_i2c_slave.png")
    )
)
#grid(
    columns: 2,
    figure(
        placement:none,
        caption: [I2C Master page],
        image("pic/sop8_i2c_master.png")
    ),
    figure(
        placement:none,
        caption: [I2C display testing page],
        image("pic/sop8_i2c_display.png")
    )
)
#v(10pt)

=== SPI
SPI peripheral can be read and write.
Modes can be changed by pressing *M*.
#highlight(fill: orange)[SCK] use PA5, #highlight(fill: rgb("#ba0000"))[#text(fill: white)[MISO]] use PA6, #highlight(fill: aqua)[MOSI] use PA7 a #highlight(fill: purple)[NSS] use PB0.

First mode is slave mode, where probe is behaving as slave. second mode is master mode, where probe is behaving as master. Fourth mode is testing *SSD1306* display, where after pressing *S*, display set all pixels active.
- *[0-F]* - Change hex value
- *[X]* - Delete value
- *[M]* - Change mode
- *[K]* - Edit values
- *[L]* - Move cursor
- *[S]* - Send data
- *[T]* - Edit settings
- *[Y]* - Switch Phase
- *[U]* - switch Polarity
- *[P]* - read/write
- *[I]* - Change bytes to send

#v(10pt)
#grid(
    columns: 2,
    figure(
        placement:none,
        caption: [SPI Slave page],
        image("pic/sop8_spi_slave.png")
    ),
    figure(
        placement:none,
        caption: [SPI Display test page],
        image("pic/sop8_spi_display.png")
    )
)
#figure(
        placement:none,
        caption: [SPI Master page],
        image("pic/sop8_spi_master.png")
    )
#v(10pt)


