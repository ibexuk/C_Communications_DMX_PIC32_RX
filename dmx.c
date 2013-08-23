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





#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define AP_DMX
#include "dmx.h"



//********************************
//********************************
//********** INTERRUPTS **********
//********************************
//********************************
void __ISR(_UART_2_VECTOR, ipl5) Uart2InterruptHandler (void) 		//(ipl# must match the priority level assigned to the irq where its enabled)
{
	static BYTE data;
	static BYTE status;
	static WORD dmx_rx_channel_number;
	static BYTE waiting_for_header = 1;
	static BYTE waiting_for_break = 1;


	Nop();

	/*
	if (
	(INTGetEnable(INT_SOURCE_UART_TX(DMX_UART_NAME))) &&
	(INTGetFlag(INT_SOURCE_UART_TX(DMX_UART_NAME)))
	)
	{
		//----------------------------------
		//----------------------------------
		//---------- TX INTERRUPT ----------
		//----------------------------------
		//----------------------------------
		while (UARTTransmitterIsReady(DMX_UART_NAME))
		{

			//UARTSendDataByte(DMX_UART_NAME, comms_tx_buffer[comms_tx_byte++]);

			
		} //while (UARTTransmitterIsReady(DMX_UART_NAME))
		INTClearFlag(INT_SOURCE_UART_TX(DMX_UART_NAME));		//Do after sending bytes
	}
	*/


	if (
	(INTGetEnable(INT_SOURCE_UART_ERROR(DMX_UART_NAME))) &&
	(INTGetFlag(INT_SOURCE_UART_ERROR(DMX_UART_NAME)))
	)
	{
		//----------------------------------------
		//----------------------------------------
		//---------- RX ERROR INTERRUPT ----------
		//----------------------------------------
		//----------------------------------------

		status = UARTGetLineStatus(DMX_UART_NAME);
		if (status & UART_PARITY_ERROR)
		{
			waiting_for_break = 1;
		}
		if (status & UART_OVERRUN_ERROR)			//OERR must be cleared, clearing will reset RX FIFO
		{
			DMX_UART_STATUS = (DMX_UART_STATUS & ~UART_OVERRUN_ERROR);		//Clear OERR bit
			waiting_for_break = 1;
		}
		if (status & UART_FRAMING_ERROR)
		{
			//--------------------------
			//----- BREAK RECEIVED -----
			//--------------------------
			waiting_for_break = 0;
			waiting_for_header = 1;				//Flag waiting for header

			dmx_rx_channel_number = 1;			//Reset the channel counter
		}
    	
    	//Clear RX buffer of bad data
		while (UARTReceivedDataIsAvailable(DMX_UART_NAME))
			data = UARTGetDataByte(DMX_UART_NAME);
		INTClearFlag(INT_SOURCE_UART_RX(DMX_UART_NAME));
		INTClearFlag(INT_SOURCE_UART_ERROR(DMX_UART_NAME));
		
	}



	if (
	(INTGetEnable(INT_SOURCE_UART_RX(DMX_UART_NAME))) &&
	(INTGetFlag(INT_SOURCE_UART_RX(DMX_UART_NAME)))
	)
	{
		//----------------------------------
		//----------------------------------
		//---------- RX INTERRUPT ----------
		//----------------------------------
		//----------------------------------
		while (UARTReceivedDataIsAvailable(DMX_UART_NAME))
		{
			//----- GET DATA BYTE -----
			data = UARTGetDataByte(DMX_UART_NAME);

			if (waiting_for_header)
			{
				//---------------------------
				//----- HEADER RECEIVED -----
				//---------------------------
				if (data == DMX_HEADER_STANDARD)
				{
					//------------------------------------
					//----- STANDARD HEADER RECEIVED -----
					//------------------------------------
					waiting_for_header = 0;
				}

				//<<<<< PUT ANY OTHER HEADER TYPES HERE

				else
				{
					//-----------------------------------
					//----- INVALID HEADER RECEIVED -----
					//-----------------------------------
					waiting_for_break = 1;			//Non recognised header - ignore packet
				}
			}
			else if (waiting_for_break)
			{
				//------------------------------------
				//----- WE ARE WAITING FOR BREAK -----
				//------------------------------------

				//Ignore rx until break is received

			}
			else
			{
				//-------------------------
				//----- DATA RECEIVED -----
				//-------------------------

				//RESET THE TIMEOUT TIMER
				dmx_rx_active_10ms_timer = 120;

				//GET THE DATA BYTE IF IN OUR CHANNEL RANGE
				if (
					(dmx_rx_channel_number >= dmx_start_address) &&
					(dmx_rx_channel_number < (dmx_start_address + DMX_NO_OF_CHANS_TO_RX))
					)
				{
					//THIS CHANNEL IS IN OUR RX WINDOW
					dmx_rx_buffer[dmx_rx_channel_number - dmx_start_address] = data;
				}

				//FLAG NEW FRAME RECEVIED IF WE HAVE RECEIVED ALL THE CHANNELS WE WANT TO RECEIVE
				if (dmx_rx_channel_number == (dmx_start_address + DMX_NO_OF_CHANS_TO_RX - 1))
					dmx_new_frame_received = 1;

				//INCREMENT THE CHANNEL NUMBER READY FOR THE NEXT BYTE
				dmx_rx_channel_number++;
				if (dmx_rx_channel_number > 512)
				{
					waiting_for_break = 1;
				}

			}
		}
		INTClearFlag(INT_SOURCE_UART_RX(DMX_UART_NAME));				//Do after reading data as any data in RX FIFO will stop irq bit being cleared
	}
}



