#include <stdio.h>				//The usual setup stuff
#include <stdlib.h>				//^
#include <xc.h>					//^
#include <stdbool.h> 
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#define _XTAL_FREQ 4000000		//Here you put the frequency that you have configured your device to run at

unsigned char data[] = "Hello";	//Declare the array 'data[]' and load it with "Hello"
volatile unsigned int flag = 0;
volatile char counter = '0';
volatile unsigned int receivedAlert = '0';

void sendCharBT(char *alertType){
        while(!TXIF);
        TXREG = alertType;
        __delay_ms(5);
        while(!TXIF);
        TXREG = '\r';
        __delay_ms(5);
        while(!TXIF);
        TXREG = '\n';
        __delay_ms(5);
}
//
void delay(){
    for(int i = 0; i < 2048; i++){
        for(int j = 0; j < 30; j++){   //100                                
        }
    }
}
//
//void turnOnBlue(){
//    RA0=1;
//    RA1=0;
//    RA2=0;
//}
//void turnOnGreen(){
//    RA0=0;
//    RA1=1;
//    RA2=0;
//}
//void turnOnRed(){
//    RA0=0;
//    RA1=0;
//    RA2=1;
//}
void turnOff(){
    PORTA = 0x00;
}

void sendCrimeAlert(){
    RA2=1;
    sendCharBT('1');
    delay();
    turnOff();
}
void sendHealthAlert(){
    RA0=1;
    sendCharBT('2');
    delay();
    turnOff();
}
void sendSuspiciousAlert(){
    RA1=1;
    sendCharBT('3');
    delay();
    turnOff();
}

void __interrupt () ISR(void)
{
	if(RCIF)  // If UART Rx Interrupt
	{
		if(OERR) // If over run error, then reset the receiver
		{
			CREN = 0;
			CREN = 1;
		}
//		while(!TXIF);
//        TXREG = RCREG;
        
        if (RCREG >= '0' && RCREG <= '3'){
            receivedAlert = RCREG;
        }
        __delay_ms(5);
	}
    if(INTF){
        //sendCharBT('D');
        if (flag == 0){
           // sendCharBT('E');
            if (!(RB5)){
               // sendCharBT('F');
                counter = counter + 1;
                __delay_ms(5);
            }
        }
//        else{
//            flag = 0;
//            sendCharBT('0');
//        }
//        for(int i = 0; i < 1024; i++){
//            for(int j = 0; j < 5; j++){                
//            }
//        }

        INTF = 0;
    }
}

int main(int argc, char** argv) {		//The usual main
    char alert = '0';
    unsigned int time = 0;
    PORTA = 0;
    TRISA = 0x00;
    TRISB = 0b0100111;					//Configure the TX and RX pins as inputs
    SPBRG = 0x19;						//Load Baud Rate Gen with decimal 25 (hex 19) - forks for 9600 baud with 4MHz oscillator and high speed mode enabled
    BRGH = 1;							//Enable high speed
    SPEN = 1;							//Enable serial port
    TXEN = 1;							//Enable transmission
    GIE  = 1;  							// Enable global interrupts
    PEIE = 1;
    CREN  = 1;						// Enable reception
	SREN  = 0;						// No effect
	TXIE  = 0;						// Disable tx interrupts
	RCIE  = 1;						// Enable rx interrupts
    nRBPU = 0;
    INTEDG = 1;
    INTE = 1;
    
    while(1){
        //sendCharBT(counter);
        if (receivedAlert == '1'){
                RA2=1;
        }
        else{
            if (receivedAlert == '2'){
            RA0=1;
            }
            else{
                if (receivedAlert == '3'){
                        RA1=1;
                }
                else{
                    RA0=0;
                    RA1=0;
                    RA2=0;
                }
            }
        }

        if(!(RB5) && flag == 0){
           // sendCharBT('A');
            
            for(int i = 0; i < 2048; i++){
                for(int j = 0; j < 50; j++){                                    
                }
            }
//            while(!(RB5))
            //TODO check if cancel
    //        sendCharBT(counter);
            if(counter == '1'){            
                //sendSuspiciousAlert();
                flag = 1;
                receivedAlert = '3';
            }
            else{
                if(counter == '2'){
                    //sendHealthAlert();
                    flag = 1;
                    receivedAlert = '2';
                }
                else{
                    if (counter == '3'){
                        //sendCrimeAlert();
                        flag = 1;
                        receivedAlert = '1';
                    }
                }
            }
            alert = receivedAlert;
            counter = '0';
            
        }
        else if (flag ==1 && !(RB5) && !(RB0)){
             //sendCharBT('C');
            unsigned int i;
            for(i =0;!(RB5) && !(RB0) && i < 3; i = i+1){
                //sendCharBT('i');
                __delay_ms(1000);
            }
            if (i >= 2){
                PORTA = 0x00;                
                sendCharBT('0');
                receivedAlert = '0';
                RB3 = 1;
                __delay_ms(250);
                RB3 = 0;
                alert = receivedAlert;
                counter = '0';
                __delay_ms(1000);
                flag = 0;
            }            
        }
        else if (alert >= '1' && alert <= '3'){
            if(time < 1000){ //2000
                time++;
            }
            else{
                time = 0;
                if (alert == '1'){
                    sendCrimeAlert();
                }
                else if (alert == '2'){
                    sendHealthAlert();
                }
                else if (alert == '3'){
                    sendSuspiciousAlert();
                }
                 alert = '0';
            }
            counter = '0';
        }
    __delay_ms(5);        
    }
    return (EXIT_SUCCESS);				//Usual C language return 0
}
