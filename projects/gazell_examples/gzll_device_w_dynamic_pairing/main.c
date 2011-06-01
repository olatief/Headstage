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
 * @brief Gazell pairing library Device example 
 * @defgroup gzll_device_w_dyn_pair_example Gazell pairing library Device example 
 * @{
 * @ingroup nrf_examples
 *
 * @brief Gazell Link Layer Device using the Gazell pairing library for adding 
 * dynamic pairing functionality. 
 *
 * The application tries to send packets continuously. If a packet transmission
 * fails (either times out or encryption failed), the Device will attempt pairing 
 * to a Host by sending a pairing request, consisting of an "address request" and a
 * "Host ID" request.
 *
 * If the Device is paired to a Host, pairing data will be stored in non 
 * volatile memory.
 *
 * During normal packet transmission the contents of P0 is sent in the 
 * first payload byte (byte 0). 
 *
 * The application alternates between sending the packets encrypted
 * through the pairing library or directly as plaintext using pipe 2.
 *
 * The project @ref gzll_host_w_dyn_pair_example can be used as a counterpart 
 * for receiving the data.
 */

#include "gzll_mcu.h"
#include "gzll.h"
#include "gzp.h"

void main(void)
{
  bool send_crypt_data = false;
  bool tx_success = false;
  gzp_id_req_res_t id_req_status;
  uint8_t payload[GZLL_MAX_PAYLOAD_LENGTH];
 
  mcu_init();
  
  // Initialize Gazell Link Layer
  gzll_init();
  
  // Initialize Gazell Pairing Library
  gzp_init();

  EA = 1;

  while(true)
  {      
    payload[0] = ~P0;
    
    // Send every other packet as encrypted data  
    if(send_crypt_data)
    {
      // Send encrypted packet using the Gazell pairing library
      tx_success = gzp_crypt_data_send(payload, GZP_ENCRYPTED_USER_DATA_MAX_LENGTH);
    }
    else
    {
      // Send packet as plaintext on pipe 2
      gzll_tx_data(payload, GZLL_MAX_FW_PAYLOAD_LENGTH, 2);   
      while(gzll_get_state() != GZLL_IDLE)
      ;
      tx_success = gzll_tx_success();
    }
    send_crypt_data = !send_crypt_data;

    // If data transfer failed
    if(!tx_success)
    {
      // Send "system address request". Needed for sending any user data to Host.
      gzp_address_req_send();
      
      // Send "Host ID request". Needed for sending encrypted user data to host.
      id_req_status = gzp_id_req_send();
    }

    // If waiting for Host to grant or reject ID request
    if(id_req_status == GZP_ID_RESP_PENDING)
    {
      // Send new ID request for fetching response
      id_req_status = gzp_id_req_send();
    }
  }  
}

/** @} */