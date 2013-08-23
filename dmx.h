/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		PIC32 DMX Receive Driver





/*
##### ADD THIS TO YOUR APPLICATION INITIALISE #####

	//----- SETUP UART # -----
	//Used for: DMX
    UARTConfigure(DMX_UART_NAME, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(DMX_UART_NAME, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(DMX_UART_NAME, DMX_UART_LINE_CONTROL);
    UARTSetDataRate(DMX_UART_NAME, PERIPHERAL_CLOCK_FREQUENCY, 250000);
    UARTEnable(DMX_UART_NAME, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    
	INTSetVectorPriority(INT_VECTOR_UART(DMX_UART_NAME), INT_PRIORITY_LEVEL_5);				//1=lowest priority to 7=highest priority.  ISR function must specify same value
	//INTClearFlag(INT_SOURCE_UART_TX(DMX_UART_NAME));
	//INTEnable(INT_SOURCE_UART_TX(DMX_UART_NAME), INT_ENABLED);
	INTClearFlag(INT_SOURCE_UART_RX(DMX_UART_NAME));
	INTEnable(INT_SOURCE_UART_RX(DMX_UART_NAME), INT_ENABLED);
	INTClearFlag(INT_SOURCE_UART_ERROR(DMX_UART_NAME));
	INTEnable(INT_SOURCE_UART_ERROR(DMX_UART_NAME), INT_ENABLED);




##### ADD THIS TO YOUR HEARTBEAT INTERRUPT SERVICE FUNCTION #####
	//----- HERE EVERY 10 mSec -----
	if (dmx_rx_active_10ms_timer)
		dmx_rx_active_10ms_timer--;

*/



//##### USING THE DRIVER #####
/*
	dmx_start_address =			//1 - 512

	if (dmx_rx_active_10ms_timer)
	{
		//DMX RX IS ACTIVE

	}

	if (dmx_new_frame_received)
	{
		dmx_new_frame_received = 0;

		= dmx_rx_buffer[0];		//Get the DMX data starting from the DMX start address
	}
*/





//********************************
//********************************
//********** DO DEFINES **********
//********************************
//********************************
#ifndef AP_DMX_INIT		//Do only once the first time this file is used
#define	AP_DMX_INIT


#define	DMX_UART_NAME					UART2		//Also set UART ID IN "void __ISR"
#define	DMX_UART_STATUS					U2STA
#define DMX_UART_LINE_CONTROL			(UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_2)

#define	DMX_HEADER_STANDARD				0x00

#define	DMX_NO_OF_CHANS_TO_RX			6			//How many channels to receive starting from dmx_start_address

#endif



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef AP_DMX
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------



//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)



#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef AP_DMX
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
volatile WORD dmx_rx_active_10ms_timer = 0;
WORD dmx_start_address = 0;
volatile BYTE dmx_rx_buffer[DMX_NO_OF_CHANS_TO_RX];
volatile BYTE dmx_new_frame_received = 0;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern volatile WORD dmx_rx_active_10ms_timer ;
extern WORD dmx_start_address;
extern BYTE volatile dmx_rx_buffer[DMX_NO_OF_CHANS_TO_RX];
extern BYTE volatile dmx_new_frame_received;



#endif






