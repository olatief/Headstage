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
 * @brief Gazell Link Layer Host example
 * @defgroup gzll_host_example Gazell Link Layer Host example
 * @{
 * @ingroup nrf_examples
 *
 * @brief This example listens for data and writes the first byte (byte 0) of the
 * received payloads to P0. 
 *
 * Protocol parameters such as addresses and channels are specified 
 * in @ref gazell_examples/params/gzll_params.h. 
 *
 * The project @ref gzll_device_example can be used as a counterpart for transmitting the data.
 * 
*/

#include "gzll_mcu.h"
#include "gzll.h"

void main(void)
{
  uint8_t payload[GZLL_MAX_PAYLOAD_LENGTH];

  mcu_init();
  gzll_init();

  // Set P0 as output
  P0DIR = 0;                                          

  EA = 1;
  
  // Enter host mode (start monitoring for data)
  gzll_rx_start();                                          
  
  while(true)
  {      
    // If data received
    if(gzll_rx_fifo_read(payload, NULL, NULL))
    {
      // Write received payload[0] to port 0
      P0 = payload[0];
    }
  } 
}

/**  @} */