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
 * $LastChangedRevision: 6037 $
 */

/** @file
 * @brief Gazell pairing library Host example 
 * @defgroup gzll_host_w_dyn_pair_example Gazell pairing library Host example 
 * @{
 * @ingroup nrf_examples
 *
 * @brief Gazell Link Layer Host using the Gazell Pairing Library for adding 
 * dynamic pairing functionality. 
 *
 * The example is monitoring for pairing requests as well as normal user data. 
 * Pairing will be granted to any device sending a pairing request. 
 * If user data is received the first payload byte (byte 0) is written to P0. 
 *
 * The project @ref gzll_device_w_dyn_pair_example can be used as a counterpart 
 * for transmitting data.
 *
 */

#include "gzll_mcu.h"
#include "gzll.h"
#include "gzp.h"

void main(void)
{
  uint8_t payload[GZLL_MAX_PAYLOAD_LENGTH];

  mcu_init();
  gzll_init();
  gzp_init();
  gzp_pairing_enable(true);

  // Open pipe 2. (Pipe 0 and 1 are reserved by pairing library).
  gzll_set_param(GZLL_PARAM_RX_PIPES, gzll_get_param(GZLL_PARAM_RX_PIPES) | (1 << 2));
  
  // Set P0 as output
  P0DIR = 0;                                          

  EA = 1;

  // Enter host mode (start monitoring for data)
  gzll_rx_start();                                          
  
  while(true)
  { 
    // If gzpair_host_execute() returns true, a pairing request has been received
    gzp_host_execute();

    // If Host ID request received
    if(gzp_id_req_received())
    {
      // Always grant request
      gzp_id_req_grant();
    }
   
    // If any data received (plaintext on pipe 2 or encrypted through Gazell pairing library)
    if((gzll_get_rx_data_ready_pipe_number() == 2) || (gzp_crypt_user_data_received()))
    {
      // Plaintext data received? 
      if(gzll_rx_fifo_read(payload, NULL, NULL))
      {
        // Write received payload[0] to port 0
        P0 = payload[0];
      }
      else
      {
        // Read data from Gazell pairing library
        gzp_crypt_user_data_read(payload, NULL);
        // Write received payload[0] to port 0
        P0 = payload[0];
      }
    }   
  }          
}

/** @} */