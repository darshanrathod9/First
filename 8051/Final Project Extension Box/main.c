#include "main.h"

unsigned int state1,state2,flag1=1,flag2=1,once1=1,once2=1;

void setup()
{
	Uart_Init();
	init_Timer0();
	
	TLED = 0;
	LED1 = 0;
	LED2 = 0;
	BT1 = 1;					// BT 1 -> INPUT
	BT2 = 1;					// BT 1 -> INPUT
	Relay1 = 0;				// SET AS OUTPUT
	Relay2 = 0;				// SET AS OUTPUT 			
}

void main()
{
	setup();

	Relay1 = 1;					// Relay 1 -> OFF
	Relay2 = 1;					// Relay 1 -> OFF
	
	while(1)
	{
		if(RI)
		{
			incoming_com = SBUF;		// incoming bluetooth message
			temp = incoming_com;
			RI=0;
		}												// Initial state of the MCU 
		
		if(flag1 == 1)					// Initial state of Relay1
		{
			state1 = BT1;
			switch(state1)
			{
				case 0:
					break;
				
				case 1:
					flag1 = 0;
					break;
			}
		}
																			// Manual and Bluetooth controlled switch module starts here
		if(BT1 == PRESSED && flag1 == 0)				// Manual switch 1 state check and take action
		{
			if(once1 == 1)
			{
				Relay1 = ON;
				once1 = 0;
				LED1 = 1;
			}
			else
			{
				switch(temp)										// Bluetooth controlled if switch is on 
				{
					case 'A':
						Relay1 = ON;
						send_str("Relay1 ON");
						LED1 = 1;
						break;
					
					case 'B':
						Relay1 = OFF;
						send_str("Relay1 OFF");
						LED1 = 0;
						break;			
				}
			}
		}
		else if(BT1 == RELEASED || flag1 == 1)					// completely off if the switch is off
		{
			if(temp == 'A' || temp == 'B')
			{
				send_str("First Turn on the Button 1");
			}
			Relay1 = OFF;
			once1 = 1;
			LED1 = 0;
		}

		switch(temp)
		{
			case 'C':
				Relay2 = ON;
				send_str("Relay2 ON");
				LED2 = 1;	
				break;
		
			case 'D':
				Relay2 = OFF;
				send_str("Relay2 OFF");
				LED2 = 0;
				break;	

			case 'G':										// turn off after the timer is done
				toggle = 0;
				break;
			
			case 'H':				
				toggle = 1;								// turn on after the timer is done 
				break;	
			
			case 'S':										// Stop timer
				stop_timer();
				break;
			
			case 'T':										// Start timer
				start_timer();
				break;			
		
			case 'U':
				MINUTES += 5;							// add 5 min to current time 
				send_str(" Added 5 min");
				break;
			
			case 'V':
				MINUTES += 15;						// add 15 min to current time 
				send_str(" Added 15 min");
				break;
			
			case 'W':
				MINUTES += 60;						// add 60 min to current time 
				send_str(" Added 60 min");
				break;
			
			case 'X':
				MINUTES -= 5;							// reduce 5 min to current time 
				send_str(" Reduced 5 min");
				break; 
			
			case 'Y':
				MINUTES -= 15;						// reduce 15 min to current time 
				send_str(" Reduced 15 min");
				break;
			
			case 'Z':
				MINUTES -= 60;						// reduce 60 min to current time
				send_str(" Reduced 60 min");
				break;
		}
				
		temp=0;
		incoming_com = 0;
	}
}