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
 * $LastChangedRevision: 5718 $
 */

/** @file
 * @brief ADC single step mode example
 * @defgroup adc_single_step_example ADC single step mode example
 * @{
 * @ingroup nrf_examples
 *
 * @brief This example samples P00 using ADC single step mode and outputs the 8 bit
 * sample value on P1.
 *
*/

#include <Nordic\reg24le1.h>
#include <stdint.h>
#include "hal_adc.h"

void main()
{
  // Set P1 as output
  P1DIR = 0;                                   

  // Configure ADC
  hal_adc_set_input_channel(HAL_ADC_INP_AIN0);
  hal_adc_set_reference(HAL_ADC_REF_VDD); 
  hal_adc_set_input_mode(HAL_ADC_SINGLE);          
  hal_adc_set_conversion_mode(HAL_ADC_SINGLE_STEP);
  hal_adc_set_resolution(HAL_ADC_RES_8BIT);
  hal_adc_set_data_just(HAL_ADC_JUST_RIGHT); 

  while(1)
  {
    hal_adc_start();                                // Start the ADC
    while( hal_adc_busy() )                         // Wait for the ADC to finish a conversion
      ;
    P1 = hal_adc_read_LSB();                        // Write the ADC result to P0
  }
} 
