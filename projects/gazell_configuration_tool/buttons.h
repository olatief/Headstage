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
 *
 * Button definitions for the GazellGo development tool.  
 * 
 */
 
#ifndef GG_BUTTONS_H__
#define GG_BUTTONS_H__

#define BUTTONS P2
#define JS_DEBOUNCE 150
#define BUTTONS_DEBOUNCE_SLOW 170  // ms
#define BUTTON_HOLD_THRESHOLD (1200 / BUTTONS_DEBOUNCE_SLOW)
#define BUTTONS_DEBOUNCE_FAST 1    
#define DELAY_SHORT_MESSAGE 600  // ms

#define MAIN_MENU_DEC (1 << 0)
#define MAIN_MENU_INC (1 << 1)
#define SUB_MENU_DEC  (1 << 2)
#define SUB_MENU_INC  (1 << 3)
#define VALUE_DEC (1 << 4)
#define VALUE_INC (1 << 5)
#define CLEAR_STATISTICS (1 << 6)
#define RUN_STOP (1 << 7)

#endif

