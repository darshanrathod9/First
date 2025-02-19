
#include <reg52.h>

#define PRESSED 		0
#define RELEASED 		1
#define ON 					0
#define OFF 				1
#define INPUT 			1
#define OUTPUT 			0

sbit BT1 = P0^0;
sbit BT2 = P0^1;
sbit Relay1 = P2^0;
sbit Relay2 = P2^1;
sbit TLED = P2^5;
sbit LED1 = P2^6;
sbit LED2 = P2^7;
 

void Uart_Init();
void send_str(unsigned char *str);
void send_char(unsigned char chr);
void init_Timer0();
void start_timer();
void stop_timer();
void time_up();
void Toggle_state();

unsigned char incoming_com=0;
char temp;
unsigned int sec=0, min=0, num=0;
int MINUTES=1, toggle=0;

void Uart_Init()
{
	 SCON = 0x50; 						// SCON: mode 1, 8-bit UART, enable receive 
	 TMOD = 0x20; 						// TMOD: timer 1, mode 2, 8-bit 
	 TH1 = 0xFD;							// TH1: for 9600 baud
	 TR1 = 1; 								//TR1: timer 1 run 
}

void send_str (unsigned char *str)
{
		while(*str)
		{
			 send_char(*str++);
		}	
		send_char(0x0D);
}


void send_char (unsigned char chr)
{
	 SBUF = chr;
	 while (TI==0); 					//Wait until the character is completely sent 
	 TI=0; 										//Reset the Transmit Interrupt flag 
}


void init_Timer0() 
{
    TMOD |= 0x01;   // Set Timer 0 to Mode 1 (16-bit timer)
    TH0 = 0x4B;     // Load the high byte for a 50ms delay
    TL0 = 0xFD;     // Load the low byte for a 50ms delay
    ET0 = 1;        // Enable Timer 0 interrupt
    EA = 1;         // Enable global interrupts
}

void start_timer()
{
		sec = 0;
		min = 0;
		send_str("Timer started");
		TR0 = 1;        // Start Timer 0
}

void stop_timer()
{
		TR0 = 0;        // Stop Timer 0		
		sec = 0;
		min = 0;
		TLED = 0;
		MINUTES = 1;
		send_str("Timer stopped");
}

void time_up()
{
		TR0 = 0;
		sec = 0;
		min = 0;
		MINUTES = 1;		
		TLED = 0;
		switch(toggle)
		{
			case 1:										// Turn ON relay 
				Relay1 = ON;
				break;
				
			case 0:										// Turn OFF relay 		
				Relay1 = OFF;
				break;
		}
}


void timer0_isr() interrupt 1 
{
    static unsigned int clock = 0;
    clock++;
		
		if(clock == 14) 					// When the count reaches 20 (20 * 50ms = 1 second), practically 14 clock -> 1 sec
		{
			if(sec == 59)
			{
				min++;
				sec = 0;
			}
			
			if(min >= MINUTES)
			{
					time_up();
			}
			
			clock = 0; 				// Reset the count for the next second
			sec++;						// INcrement the seconds 
			TLED = ~TLED;
		}
		
    TF0 = 0; 									// Clear the interrupt flag
}

