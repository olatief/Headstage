/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * $LastChangedRevision: 5679 $
 */

/** @file
 * @brief Enhanced ShockBurst Primary Transmitter example
 * @defgroup esb_ptx_example Enhanced ShockBurst Primary Transmitter (PTX) example
 * @{
 * @ingroup nrf_examples
 * 
 * @brief This example sends packets continuously. The contents of P0 is 
 * sent in the first payload byte (byte 0). 
 *
 * The example shows the minimum required setup for transmitting packets to a 
 * primary receiver (PRX) device.  
 * 
 * The following default radio parameters are being used:
 * - RF channel 2
 * - 2 Mbps data rate
 * - TX address 0xE7E7E7E7E7 
 * - 1 byte CRC  
 * 
 * The project @ref esb_prx_example can be used as a counterpart for receiving the data.
 * 
*/

#ifdef MCU_NRF24LE1
#include <reg24le1.h>
#include <hal_spi.h>
#endif

#ifdef MCU_NRF24LU1P
#include <Nordic\reg24lu1.h>
#endif

#include <nordic_common.h>
#include <intrins.h>
#include <stdint.h>
#include <stdbool.h>
#include "hal_nrf.h"
#include "hal_nrf_hw.h"

#define MAXPAYLOAD 27
#define MAXBLOCKS  4
#define CALC_PAYLOAD_PTR(bl) payload+bl*MAXPAYLOAD


sbit P0_0 = P0^0;
sbit P0_4 = P0^4;
sbit P0_5 = P0^5;
sbit P0_6 = P0^6;
sbit P0_7 = P0^7;

// Global variables
data bool radio_busy;
data uint8_t temp = 0;
data uint8_t block = 0;
data uint8_t byteCnt = MAXBLOCKS*MAXPAYLOAD;
data uint8_t blockAcquired = 0;
data uint8_t blockNeedsSending = 0;
data uint8_t payload[MAXBLOCKS*MAXPAYLOAD];
data uint8_t data* payloadspi;
data uint8_t data* payloadtx;

void main()
{

  #ifdef MCU_NRF24LU1P
     USBSLP = 0x01;  // disable usb
     P0DIR = (1<<3)|(1<<1)|(1<<0);
	 P0ALT = 0x0F;
	 P0EXP = 0x02; // Slave SPI for P0
	 SSCONF = 0x01; //Enable slave SPI
	 
	 // Enable radio SPI
	 RFCTL = 0x10;
	  I3FR = 1;             // rising edge SPI ready detect
	  P0_0 = 0;
	  INTEXP = 0x01; //Slave SPI Interrupt
  #else
  	P0DIR =  ~((1<<4)|(1<<0));
  	SPISCON0 = 0x40;
	I3FR = 1;             // rising edge SPI ready detect
	P0_0 = 0;
  	INTEXP = 0x01; //Slave SPI Interrupt
  	
	SPISCON0 |= 0x01;
  #endif

  // Enable the radio clock
  RFCKEN = 1;
  SPI = 1; // Enable SPI Interrupt
  // Enable RF interrupt
  RF = 1; 

 //  ET0 = 1; // enable timer interrupt
  // EX0 = 1; 
  // Enable global interrupt
  EA = 1;

  // Power up radio
  	hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
	hal_nrf_enable_dynamic_ack(true); // lets us disable ack, saves time.
	hal_nrf_write_reg (SETUP_RETR, 0);
	hal_nrf_write_reg (EN_AA, 0);
//	hal_nrf_set_output_power(HAL_NRF_18DBM);
//	hal_nrf_set_auto_retr(0,250); // disable auto_retry
// 	hal_nrf_close_pipe(HAL_NRF_ALL);	

  hal_nrf_write_tx_payload(payload, MAXPAYLOAD); 
  hal_nrf_write_tx_payload(payload, MAXPAYLOAD);
  CE_HIGH(); // Sends packet 
  while(1)
  {

	while (!blockNeedsSending);
  	
	// Write payload to radio TX FIFO
	while(radio_busy);			 
	
	blockNeedsSending = 0;
	
	P0_4 = 1;
				//payload+block*MAXPAYLOAD;
	switch(block)
	{
	  case 0:
		payloadtx = CALC_PAYLOAD_PTR(2);
		break;
	  case 1:
		payloadtx = CALC_PAYLOAD_PTR(0);
		break;
	  case 2:
		payloadtx = CALC_PAYLOAD_PTR(1);
		break;
	}

    hal_nrf_write_tx_payload(payloadtx, MAXPAYLOAD);

	radio_busy = true;
	
	P0_4 = 0;
		
    // Toggle radio CE signal to start transmission 
    CE_HIGH();
  //  CE_PULSE();				  
   
  }                                           
} 

	void slave_spi() interrupt INTERRUPT_SPI
	{
	  P0_5 = 1;
	  
	  if(byteCnt==0)
	  {
		byteCnt = MAXPAYLOAD;
		++block;
		blockNeedsSending = 1;
		if(block==3)
		{
			payloadspi = payload;
			block=0;
		}
	  } 
	 
	  *((unsigned char data *)(payloadspi)) = SSDATA;
	  payloadspi++;
	  byteCnt--;
	  P0_5 = 0;
	}

// Radio interrupt
void rf_irq() interrupt INTERRUPT_RFIRQ
{
  uint8_t data irq_flags;
   P0_4 = 1;
  // Read and clear IRQ flags from radio
    CSN_LOW();
  // retval = hal_nrf_rw(W_REGISTER + reg);

  RFDAT = W_REGISTER + STATUS;
  RFSPIF = 0;     // ! IMPORTANT ! Clear RF SPI ready flag
                  // after data written to RFDAT..
  while(!RFSPIF); // wait for byte transfer finished
    
  irq_flags = RFDAT;   // return SPI read value


  RFDAT = (BIT_6|BIT_5|BIT_4);
  RFSPIF = 0;     // ! IMPORTANT ! Clear RF SPI ready flag
                  // after data written to RFDAT..
  while(!RFSPIF); // wait for byte transfer finished
    
  // retval = RFDAT;   // return SPI read value

 //  hal_nrf_rw(value);
  CSN_HIGH();
  irq_flags = irq_flags & (BIT_6|BIT_5|BIT_4);
 // irq_flags = hal_nrf_get_clear_irq_flags(); 

  switch(irq_flags)
  {
    // Transmission success
    case (1 << HAL_NRF_TX_DS):
      radio_busy = false;
      // Data has been sent
      break;
    // Transmission failed (maximum re-transmits)
    case (1 << HAL_NRF_MAX_RT):
      // When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO. 
      // If the packet is to be discarded this must be done manually by flushing the TX FIFO.
      // Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
      // (Will only be possible after the radio irq flags are cleared) 
      
	  // hal_nrf_flush_tx();
        CSN_LOW();
		RFDAT = FLUSH_TX;
  		RFSPIF = 0;     // ! IMPORTANT ! Clear RF SPI ready flag
                  // after data written to RFDAT..
  		while(!RFSPIF); // wait for byte transfer finished
  		
  		CSN_HIGH();

	  radio_busy = false;
      break;
  }

  P0_4 = 0;
}