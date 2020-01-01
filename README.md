PIC18F Morse code serial example

This example is used to send data through UART using pin 18 and 17 (RX,TX) and translate it into morse codes.
Once the submitted string ends with a "\n" or "\r" the magic starts on pin 6 (RA4)

In addition to this the serial port will return the morse codes as string ("." for short / "-" for long)

Project Owner(s): Ole Koeckemann
Author(s): Ole Koeckemann
Hardware Platform: PIC18F26K20
Debuggers Used: Simulator
Programmers Used:
MPLAB Version: 5.30
C Compiler Version: XC 8
Final Checksum:

FILES INCLUDED IN THE PROJECT TREE:

system.h - Contains custom oscillator configuration function prototypes,
reset source evaluation function prototypes, and non-peripheral
microcontroller initialization function prototypes.  It also may contain
system level #define macros and variables.  This file is included
by system.c

configuration_bits.c - Contains device configuration bit macros.  Refer to
the comments in configuration_bits.c for instructions on where to get
information about device configuration bits and their corresponding macros.

interrupts.c - This file contains example stubs for interrupts.  The user would
put the interrupt vectors for their application in interrupts.c.

main.c - This is the main code for the project.  global variables and the
main() function are located in main.c  The user would put their primary program
flow in main.c, and the operation should be contained in main.c with an
infinite loop.

system.c - Contains custom oscillator configuration functions, reset source
evaluation functions, and non-peripheral microcontroller initialization
functions.  Functions in system.c would often be called from main.c during
device initialization.

FILES INCLUDED IN THE PROJECT BUT NOT IN THE PROJECT TREE:

#include <htc.h>             /* Global Header File */
#include <stdint.h>          /* For uint8_t definition */
#include <stdbool.h>         /* For true/false definition */
#include "string.h"          /* for strlen method */
#include "ctype.h"           /* for toupper method */

These files come with the HiTech compiler.  Check the compiler documentation for
more information on these files.
