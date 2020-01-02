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
* 4x male/female jumper wires (RPi -> Breadboard)
* 4x male/male jumper wires (Breadboard)
* 2x Resistors (10 K)
* 1x LED

## Software requirements

* [MPLAB](https://www.microchip.com/mplab/mplab-x-ide) to compile the source code
* [pickle](https://wiki.kewl.org/dokuwiki/projects:pickle#installation) to program the controller via raspberry

## Pinout (RPi)

Use the below pinout to connect controllers serial port to the RPi

**This applies only to PIC18F26K20 - Other chips may be differenty**

[PIC18F26K20 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/40001303H.pdf)

```
R-PI                              PICMicro       GPIO header     .pickle
====                              ========       ===========     =======
3V3-------------------------------VDD--+         PIN 1 (P1-01)
                                       |
GND-------------------------------VSS  R1        PIN 19
                                       |
                                  VPP--+         PIN 7           VPP=4
                                  RA4--+         PIN 6
                                       |
                                       R1
                                       |
                                       LED
                                       |
                                      ///
GPIO14----------------------------RC7/RX/DT      PIN 18
GPIO15----------------------------RC6/TX/CK      PIN 17
                                  PGM--R1-+      PIN 15          PGM=22
                                           |
                                          ///
(R1) 10K !MCLR pull-up
(R2) 10K where appropriate (PGM is not present on all devices).
```

**PLEASE NOTE:**

This pinout does not allow you to program the the controller.
To program please follow the steps from the [pickle project](https://wiki.kewl.org/dokuwiki/projects:pickle) page

## Build pickle

To build pickle on your Raspberry Pi, please follow the installation instructions from the installation link

## Program the firmware

To program the firmware onto your controller, all previous steps are necessary.

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

To test the serial chip connection, I recommend to install minicom on your RPi using the following command

```
sudo apt-get install minicom
```

Once minicom is installed use the following command to establish the connection

```
minicom -b 9600 -o -D /dev/serial0
```

By entering "sos" the controller should response with "...---..." and LED should blink properly
