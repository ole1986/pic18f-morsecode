/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include <stdio.h>
#include "string.h"
#include "ctype.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */
#define SBIT_TXEN     5
#define SBIT_TXSYNC   4
#define SBIT_SPEN     7
#define SBIT_CREN     4

struct morsecodes {
    char letter;
    char *code;
};

const unsigned int morse_dot_speed = 50; // morse dot speed in milliseconds
const unsigned int morse_dash_speed = morse_dot_speed * 3;

struct morsecodes mcodes[] = {
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},
    
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    {'0', "-----"}
};

void UART_Init(int baudRate)
{
    TRISC=0x80;            // Configure Rx pin as input and Tx as output  
    
    TXSTA=(1<<SBIT_TXEN);  // Asynchronous mode, 8-bit data & enable transmitter
    RCSTA=(1<<SBIT_SPEN) | (1<<SBIT_CREN);  // Enable Serial Port and 8-bit continuous receive
    
    BRGH = 1;
    
    if (BRGH) {
        SPBRG = (4000000UL/(long)baudRate/16)-1;      // baud rate @20Mhz Clock
    } else {
        SPBRG = (4000000UL/(long)baudRate/64)-1;      // baud rate @20Mhz Clock
    }
    
}

void UART_TxChar(char ch)
{
    while(TXIF==0);    // Wait till the transmitter register becomes empty
    TXIF=0;            // Clear transmitter flag
    TXREG=ch;          // load the char to be transmitted into transmit reg
}

void UART_TxString(char *str, int length)
{
    int i = 0;
    for (i = 0; i<length && str[i]!='\0' ; i++) {
        UART_TxChar(str[i]);
    }
}

char UART_RxChar()
{
    while(RCIF==0);    // Wait till the data is received 
    RCIF=0;            // Clear receiver flag
    return(RCREG);     // Return the received data to calling function
}

void UART_RxString(char *ptr_stringPointer_u8)
{
	char ch;
	while(1)
	{
		ch=UART_RxChar();    //Receive a char
        
		if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
		{						     //once enter key is pressed null terminate the string
			*ptr_stringPointer_u8=0;           //and break the loop
            // send CRLF
            UART_TxString("\r\n", 2);
			break;				  
		}
        
        UART_TxChar(ch);     //Echo back the received char
        
		*ptr_stringPointer_u8=ch;              //copy the char into string and increment the pointer
		ptr_stringPointer_u8++;
	}
}

void morseLED(char *code) {
    int l = strlen(code);
    for(int i = 0; i < l && code[i]!='\0'; i++) {
        LATAbits.LATA4 = 1;
        if(code[i] == '.') {
            // short
            __delay_ms(morse_dot_speed);
        } else if(code[i] == '-') {
            // long
            __delay_ms(morse_dash_speed);
        }
        LATAbits.LATA4 = 0;
        __delay_ms(morse_dot_speed);
    }
}

char *extractWord(char *morsetext, int i, int j) {
    char portion[50] = " (";
    char *p = strncat(portion, &morsetext[0]+j, i - j);
    p = strcat(portion, ") ");
    return p;
}

char *getMorseCode(char letter) {
    // convert letter to uppercase
    char uppercaseLetter = toupper(letter);
    int len = sizeof(mcodes);
    for(int i = 0; i < len; i++) {
        if (mcodes[i].letter == uppercaseLetter)
            return mcodes[i].code;
    }
    return "";
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    
    char *code = getMorseCode('B');
    
    UART_Init(9600);
    
    TRISA = 0;
        
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    
    while(true)
    {
        char morsetext[100];
#ifdef DEBUG
        strcpy(morsetext, "SOS");
#else
        UART_RxString(morsetext);
#endif
        
        
        int l = strlen(morsetext);
        
        int j = 0;
        
        for(int i = 0; i < l; i++) {
            char *code = getMorseCode(morsetext[i]);
            if(morsetext[i] == ' ') {
                
                // assuming a space is a word space
                char *word = extractWord(morsetext, i, j);
                UART_TxString(word, strlen(word));
                j = i + 1;
                __delay_ms(morse_dot_speed * 7);
                continue;
            }
            morseLED(code);
            UART_TxString(code, strlen(code));
            __delay_ms(morse_dash_speed);
            
        }
        
        char *word = extractWord(morsetext, l, j);
        UART_TxString(word, strlen(word));
        
        UART_TxString("\r\n", 2);
        LATAbits.LATA4 = 0;
    }
}