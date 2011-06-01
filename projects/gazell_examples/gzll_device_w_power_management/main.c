/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 5774 $
 */

/** @file
 * @brief Gazell Link Layer Device with power management example
 * @defgroup gzll_device_w_power_man_example Gazell Link Layer Device with power management example
 * @{
 * @ingroup nrf_examples
 *
 * @brief This example sends packets every 20 ms and uses "register retention"
 * power save mode for minimizing power consumption.
 *
 * The contents of P0 is sent in the first payload byte (byte 0).
 *
 *
 * Protocol parameters such as addresses and channels are specified 
 * in @ref gazell_examples/params/gzll_params.h. 
 *
 * The project @ref gzll_host_example can be used as a counterpart for receiving the data.
 * 
*/

#include "gzll_mcu.h"
#include "gzll.h"
#include "hal_clk.h"

void main(void)
{
  uint8_t poll_cnt = 0;
  uint8_t payload[GZLL_MAX_PAYLOAD_LENGTH];

  mcu_init();
  gzll_init();

  // Always run on 16MHz crystal oscillator
  hal_clk_set_16m_source(HAL_CLK_XOSC16M);

  // Use Gazell Link Layer synchronous device mode 2 for minimizing 
  // transmit attempts
  gzll_set_param(GZLL_PARAM_DEVICE_MODE, 2);

  EA = 1;

  while(true)
  {      
    // If gazell link layer idle
    if(gzll_get_state() == GZLL_IDLE)
    {
      // Send a packet for every 20 wakeup, equals every ~20 ms  
      if(poll_cnt > 20)
      {
        // Read P0 and put in payload[0]
        payload[0] = ~P0;
        // Transmits 1 byte ( payload[0] ) to pipe 0 address
        gzll_tx_data(&payload[0], 1, 0);   
        poll_cnt = 0;
      }
    }

    EA = 0;
    // If radio not active, enter "register retention"
    if(!gzll_radio_active())
    {
      PWRDWN = 0x04; // Enter "register retention", will wake up on pre-tick
    }
    PWRDWN = 0x07;   // Enter "standby", will wake up on tick
    EA = 1; 
    
    poll_cnt++;
  }  
}

/** @} */