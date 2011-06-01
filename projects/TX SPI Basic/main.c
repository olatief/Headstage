#include <reg24le1.h>
#include <intrins.h>
#include <stdint.h>

#include "nordic_common.h"  

#include "hal_nrf.h"
#include "hal_nrf_hw.h"
#include "hal_spi.h"

  sbit P0_0 = P0^0;
  sbit P0_4 = P0^4;
  sbit P0_5 = P0^5;
  sbit P0_6 = P0^6;
  sbit P0_7 = P0^7;

	data uint8_t blockNeedsSending = 0;
	data uint8_t temp = 0; 
	data bool radio_busy;

void main()
{

 	P0DIR =  ~((1<<4)|(1<<0));
  	// hal_spi_slave_init( HAL_SPI_MODE_0, HAL_SPI_LSB_MSB);
	 SPISCON0 = 0x40;
	  I3FR = 1;             // rising edge SPI ready detect
  P0_0 = 0;
  INTEXP = 0x01; //Slave SPI Interrupt

  //  ET0 = 1; // enable timer interrupt
  // EX0 = 1; 

	SPISDAT = 0xAA;
	SPISDAT = 0xAA;
  	SPI = 1; // Enable SPI Interrupt
	
  	SPISCON0 |= 0x01;

	    // Enable the radio clock
//  RFCKEN = 1;
  	// Enable RF interrupt
//	RF = 1; 

	// Enable global interrupt
  	EA = 1;
  	// Power up radio
  	hal_nrf_set_power_mode(HAL_NRF_PWR_UP);

	while(1)
	{
		while(radio_busy);
	    hal_nrf_write_tx_payload('a', 1);
			
	    // Toggle radio CE signal to start transmission 
	    CE_PULSE();				  
		
	    radio_busy = true;
	}
}

	void slave_sp() interrupt INTERRUPT_SERIAL // SPI interrupt??
	{
		
		P0_0 = 1;
		SPISDAT = 0xCC;
		temp = SPISDAT;
		
		_nop_();
		_nop_();
		_nop_();
		P0_0 = 0;	
	}	
	// Radio interrupt
void rf_irq() interrupt INTERRUPT_RFIRQ
{
  uint8_t irq_flags;
 //  P0_0 = 1;
  // Read and clear IRQ flags from radio
  irq_flags = hal_nrf_get_clear_irq_flags(); 

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
      hal_nrf_flush_tx();
      radio_busy = false;
      break;
  }

//  P0_0 = 0;
}