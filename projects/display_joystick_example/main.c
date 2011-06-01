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
 * @brief nRFgo display and joystick example
 * @defgroup lib_display_example nRFgo LCD display and joystick example
 * @{
 * @ingroup nrf_examples
 *
 * @brief Example application using the nRF6350 LCD and joystick module for the nRFgo Motherboard.
 * The application reads the input from the joystick and displays the joystick direction
 * in the LCD display.
 * 
 * The application can be used with nRF24LE1 only.    
 * 
*/

#include <Nordic\reg24le1.h>
#include <stdint.h>
#include "lib_display.h"

void main(void)
{
  lib_display_js_states_t js_status = 0, js_status_last = 0;

  // Initialize the LCD display
  lcd_init();

  // Clear the display and print a welcome message
  lcd_write_string("Welcome to the", 0, 0);
  lcd_write_string("joystick test", 1, 0);

  // Wait for any joystick movement before continuing 
  while(js_get_status() == JS_BUTTON_NONE)
  ;

  while(1)
  { 
    // Get the current status of the joystick
    js_status = js_get_status();

    // See if the status has changed from the last time
    if( js_status != js_status_last )
    {
      // Check the status and print a suitable text on the display
      switch( js_status  )
      {
        case JS_BUTTON_NONE:
          lcd_clear();
          break;
        case JS_BUTTON_PUSH:
          lcd_clear();
          lcd_write_string("Push", 0, 6);
          break;
        case JS_BUTTON_RIGHT:
          lcd_clear();
          lcd_write_string("Right", 0, 11);
          break;
        case JS_BUTTON_LEFT:
          lcd_clear();
          lcd_write_string("Left", 0, 0);
          break;
        case JS_BUTTON_UP:
          lcd_clear();
          lcd_write_string("Up", 0, 7);
          break;
        case JS_BUTTON_DOWN:
          lcd_clear();
          lcd_write_string("Down", 1, 6);
          break;
      }
      js_status_last = js_status;
    }    
  } 
}

                  
