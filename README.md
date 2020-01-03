# PIC18F morse code serial example

This example program demostrates how to use a pic18f26K20 to send data through UART using pin 18 and 17 (RX,TX) and translate it into morse codes.

Once an ASCII text (E.g. "SOS\n") is submitted over serial the controller translates the input into morse codes on pin 6 (RA4).
In addition to this the serial port will receive the morse codes from the controller ("." for short / "-" for long).

```
Welcome to minicom 2.7.1

OPTIONS: I18n 
Compiled on Aug 13 2017, 15:25:34.
Port /dev/serial0, 21:00:01

Press CTRL-A Z for help on special keys

sos
...---... (sos) 
```

## Hardware requirements

* 1x PIC18F controller (E.g. PIC18F26K20) or any other compatible chip
* 1x Raspberry Pi
* 1x Breadboard
* 8x male/female jumper wires (RPi -> Breadboard)
* 4x male/male jumper wires (Breadboard)
* 2x Resistors (10 K)
* 4x Resistors (470 R) - programming
* 1x LED

## Software requirements

* [MPLAB](https://www.microchip.com/mplab/mplab-x-ide) to compile the source code
* [pickle](https://wiki.kewl.org/dokuwiki/projects:pickle#installation) to program the controller via raspberry

## Pinout (RPi)

<a href="https://github.com/ole1986/pic18f-morsecode/raw/master/images/board.jpg" target="_blank">
  <img src="https://github.com/ole1986/pic18f-morsecode/raw/master/images/board.jpg" width="30%">
</a>

*A screenshot of the breadboard pinout for both serial and programmer incl. LED*

**This applies only to PIC18F26K20 - Other chips may be different**

[PIC18F26K20 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/40001303H.pdf)

### Programming

```
R-PI                              PICMicro       GPIO header     .pickle
====                              ========       ===========     =======

3V3-------------------------------VDD--+         PIN 1 (P1-01)
                                      |
GND-------------------------------VSS  R1        PIN 19
                                      |
GPIO04--------R3-470R-------------VPP--+         PIN 7           VPP=4

GPIO25--------R4-470R-------------PGC            PIN 22          PGC=25

GPIO24--------R5-470R-------------PGD            PIN 18          PGD=24

GPIO22--------R6-470R-------------PGM--R2-+      PIN 15          PGM=22
                                         |
                                        ///
(R1) 10K !MCLR pull-up
(R2) 10K where appropriate (PGM is not present on all devices).
(R3..R6) 470R.
```

Source: [pickle project page](https://wiki.kewl.org/dokuwiki/projects:pickle)

### Serial

To use the serial connection (UART) two additional jumper wires need to be connected plus one wire for the LED to blink

```
R-PI                              PICMicro       GPIO header     .pickle
====                              ========       ===========     =======
                                  RA4--+         PIN 6
                                       |
                                       R1
                                       |
                                       LED
                                       |
                                      ///
GPIO14----------------------------RC7/RX/DT      PIN 18
GPIO15----------------------------RC6/TX/CK      PIN 17

(R1) 10K or 1K
```

## Build pickle

To build pickle on your Raspberry Pi, please follow [this installation instructions](https://wiki.kewl.org/dokuwiki/projects:pickle#installation)

## Program the firmware

To program the firmware onto your controller, all previous steps are necessary and the programming pinout must be used

Download the latest morsecode firmware [from here](https://github.com/ole1986/pic18f-morsecode/raw/master/dist/XC8_18F87K22_BUILD/production/pic18f16K20.X.production.hex) with your raspberry pi.

Add a ".pickle" file into the same folder with the following content

```
DEVICE=RPI2
SLEEP=1
BITRULES=0x1000
# !MCLR/VPP        - PI/O G04
VPP=4
# PGM              - PI/O G22
PGM=22
# PGC CLOCK        - PI/O G25
PGC=25
# PGD DATA_I/O     - PI/O G24
PGD=24
# Debug level (0=NONE, 1=ERROR, 10=INFO)
DEBUG=0
```

Install the firmware using pickle command "p16" from your RPi

```
p16 program pic18f16K20.X.production.hex
```

## Test the program

To test the serial chip connection, the serial pinout must be applied.
Also I recommend to install minicom on your RPi using the following command

```
sudo apt-get install minicom
```

Once minicom is installed use the following command to establish the connection

```
minicom -b 9600 -o -D /dev/serial0
```

By entering "sos" the controller should response with "...---..." and LED should blink properly
