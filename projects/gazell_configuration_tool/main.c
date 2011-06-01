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
 * Nordic Semiconductor Gazell Link Layer Configuration Tool.  
 * 
 */

/** @defgroup nordic_protocol_gzllconfig Gazell Link Layer Configuration Tool
@{
@ingroup nordic_protocol_gzll 
*/ 

#include "buttons.h"
#include "gzll_mcu.h"
#include "gzll.h"
#include "hal_clk.h"             
#include "lib_display.h"
#include "stdio.h"
#include "string.h"
#include "assertions.h"

void assert_handler(uint16_t line, char* file)
{
  line = line; 
  file = file; 
}

/*-----------------------------------------------------------------------------
  Misc. internal function prototypes
-----------------------------------------------------------------------------*/

/**
  Function configuring application specific MCU parameters.
*/
void app_init();

/**
  Function for reading buttons and handle button debounce. 
*/
uint8_t buttons_read();

/**
  Function executing radio (gazell) related operations. 
*/
bool com_execute();

/**
  Function executing application tasks such as menu handling and
  LCD update. 
*/
void app_execute(uint8_t buttons);

/**
  Function committing the following operation:

  a = (dat + 1) mod limit
*/
uint16_t inc_mod(uint16_t dat, uint16_t limit);

/**
  Function committing the following operation:

  a = (dat - 1) mod limit
*/
uint16_t dec_mod(uint16_t dat, uint16_t limit);

/**
  Function updating line0 and line1 on LCD if content
  modified.
*/
void lcd_update(char * l0, char * l1);

void delay_ms(uint16_t ms);
void t2_init();

/*-----------------------------------------------------------------------------
  Prototypes, main menu functions
-----------------------------------------------------------------------------*/

/**
  "Operation" menu. 
*/
void mm_mode_select(char * lcd_l0, char * lcd_l1, uint8_t buttons);

/**
  "Test setup" menu. 
*/
void mm_test_app_setup(char * lcd_l0, char * lcd_l1, uint8_t buttons);

/**
  "Gzll params" menu. 
*/
void mm_gzll_params(char * lcd_l0, char * lcd_l1, uint8_t buttons);

/**
  "Statistics" menu. 
*/
void mm_statistics(char * lcd_l0, char * lcd_l1, uint8_t buttons);

/**
  "Channel conf" menu. 
*/
void mm_channel_setup(char * lcd_l0, char * lcd_l1, uint8_t buttons);

/*-----------------------------------------------------------------------------
  Main menu definition
-----------------------------------------------------------------------------*/

typedef enum
{
  MM_MODE_SELECT,
  MM_TEST_APP_SETUP,  
  MM_STATISTICS,
  MM_GZLL_PARAMS,
  MM_CHANNEL_SETUP,
  MM_LAST_PAGE
}main_menu_pages_t;

code char main_menu_header[][17] =
{
  "* Mode select  *",
  "* Test setup   *",
  "* Statistics   *",
  "* Gzll params  *",
  "* Channel conf *"
};

/*-----------------------------------------------------------------------------
  Submenu: Operation
-----------------------------------------------------------------------------*/

#define MM_MODE_SELECT_HEADER "Operation:"
#define MM_MODE_SELECT_DEVICE "Device"
#define MM_MODE_SELECT_HOST "Host"
#define DEFAULT_DEVICE_MODE true

/*-----------------------------------------------------------------------------
  Submenu: Test setup
-----------------------------------------------------------------------------*/

typedef enum
{
  TX_PACKET_INTERVAL,
  TX_PL_LENGTH,
  ACK_PL_LENGTH,
  TX_PIPE
}app_setup_t;
#define APP_SETUP_SIZE 4

code char app_setup_menu_header[][17] =
{
  "TX interval:",
  "TX PL len:",
  "ACK TX PL len:",
  "TX pipe:"
};

xdata uint16_t app_setup[APP_SETUP_SIZE] =
{
  8,
  10,
  0,              
  0,
};

code uint16_t app_setup_maximum[APP_SETUP_SIZE] = 
{
  0xffff,
  GZLL_MAX_FW_PAYLOAD_LENGTH,
  GZLL_MAX_ACK_PAYLOAD_LENGTH,
  5
};

/*-----------------------------------------------------------------------------
  Submenu: Gazell parameters
-----------------------------------------------------------------------------*/

code char gzll_param_menu_header[][17] =
{
  "DEVICE_MODE:",
  "TX_TIMEOUT:",
  "TX_ATT..SNC_ON:",
  "TX_ATT..SNC_OFF:",
  "HOST_MODE:",
  "RX_PIPES:",
  "RX_TIMEOUT:",
  "TX_M1_CYCLE_PER:",
  "RX_PERIOD:",
  "RX_PERIOD_MOD:",
  "RX_CH_HOLD_PER:",
  "CRYPT_PIPES:",
  "OUTPUT_POWER:",
  "PDOWN_IDLE_EN:",
  "MAX_SYNC_PERIOD:",
  "COL_CH_SW_LIM:",
};

code char gzll_output_power[][17] =
{
  "-18 dBm",
  "-12 dBm",
  "-6 dBm",
  "0 dBm"
};
  
/*-----------------------------------------------------------------------------
  Submenu: Statistics
-----------------------------------------------------------------------------*/

typedef enum
{
  TX_PL_CNT,
  TX_TRY_CNT,
  AVG_TX_TRIES,
  MAX_TX_TRIES,
  TX_CH_SWITCHES,
  TX_BYTE_CNT,
  RX_PL_CNT,
  RX_BYTE_CNT  
}statistics_t;
#define STATISTICS_SIZE 8

code char statistics_menu_header[][17] =
{
  "TX payloads:",
  "TX attempts:",
  "Avg TX attempts:",
  "Max TX attempts:",
  "TX ch switches:",
  "TX bytes:",
  "RX payloads:",
  "RX bytes:",
};

xdata uint32_t statistics[STATISTICS_SIZE] =
{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/*-----------------------------------------------------------------------------
  LCD messages
-----------------------------------------------------------------------------*/

#define MESSAGE_RUN_L0 "    Running!"
#define MESSAGE_RUN_L1 " "  

#define MESSAGE_STOP_L0 "    Stopped!"
#define MESSAGE_STOP_L1 " "  

#define MESSAGE_STATISTICS_CLR_L0 "   Statistics"
#define MESSAGE_STATISTICS_CLR_L1 "   cleared!"  

/*-----------------------------------------------------------------------------
  Main application
-----------------------------------------------------------------------------*/
 
/**
Gazell Link Layer Configuration tool main application.
*/
void main(void)
{
  bool radio_activity;
  uint8_t buttons;
  
  mcu_init();
 
  gzll_init();
  app_init();
  lcd_init();
     
  EA = 1;

  app_execute(0);

  while(1)
  {       
    buttons = buttons_read();
    radio_activity = com_execute();
   
    if(buttons || radio_activity)
    {
      app_execute(buttons);       
    }
  }
}

/*-----------------------------------------------------------------------------
  Global variables
-----------------------------------------------------------------------------*/

// For communication between com_execute() and app_execute() 
xdata bool radio_run = false;

// Holding whether device/host operation 
xdata bool device_mode = DEFAULT_DEVICE_MODE;

xdata bool gzll_timer_tick;

// Variables managed by 1ms timer ISR 
xdata uint16_t cnt_1ms = 0;
xdata uint8_t button_db = 0;

/*-----------------------------------------------------------------------------
  Function implementations
-----------------------------------------------------------------------------*/

void app_init()
{
  t2_init();
}

bool com_execute()
{
  xdata bool retval = false; 
  xdata uint8_t user_data[32];
  xdata uint16_t temp;
  xdata uint8_t t_length, t_pipe, curr_gzll_state;
  static xdata uint8_t prev_gzll_state = 0xff;

  // If Device operation
  if(device_mode)
  {    
    // Get Gazell state
    curr_gzll_state = gzll_get_state();

    if(curr_gzll_state == GZLL_IDLE) 
    {
      // If state transition from TRANSMIT to IDLE
      if(prev_gzll_state == GZLL_DEVICE_ACTIVE)
      {
        retval = true;
        
        if(gzll_tx_success())
        {
          statistics[TX_PL_CNT]++;
          statistics[TX_BYTE_CNT] += app_setup[TX_PL_LENGTH];          
          temp = gzll_get_tx_attempts();
          statistics[TX_TRY_CNT] += temp;
          statistics[AVG_TX_TRIES] = (uint16_t)(((float)statistics[TX_TRY_CNT] * 100) / statistics[TX_PL_CNT] );
          if(statistics[MAX_TX_TRIES] < temp)
          {
            statistics[MAX_TX_TRIES] = temp;
          }
          statistics[TX_CH_SWITCHES] += gzll_get_tx_channel_switches();
        } 
                                    
        if(gzll_rx_fifo_read(user_data, &t_length, &t_pipe))
        {
          statistics[RX_PL_CNT]++;
          statistics[RX_BYTE_CNT] += t_length;
        }
      }

      if(radio_run)
      {
        EA = 0;
        if(cnt_1ms >= app_setup[TX_PACKET_INTERVAL] )
        {  
          gzll_timer_tick = false;
          cnt_1ms = 0;
          EA = 1;
   
          while(!gzll_timer_tick)         // Improves synchronization to start in synch with protocol timer 
          ;
          gzll_tx_data(user_data, app_setup[TX_PL_LENGTH], app_setup[TX_PIPE]);
          curr_gzll_state = GZLL_DEVICE_ACTIVE; 
        }
        EA = 1;
      }
      prev_gzll_state = curr_gzll_state;               
    }
  }
  // Host operation
  else
  {
    if(gzll_rx_fifo_read(user_data, &t_length, &t_pipe))
    {
      retval = true;
      statistics[RX_PL_CNT]++;
      statistics[RX_BYTE_CNT] += t_length;
      
      t_length = app_setup[ACK_PL_LENGTH];
      if(t_length)
      {
        if(gzll_ack_payload_write(user_data, t_length, t_pipe))
        {          
          statistics[TX_PL_CNT]++;
          statistics[TX_BYTE_CNT] += t_length;      
        }
      }
    }
  }
  
  return retval;
}

void app_execute(uint8_t buttons)
{
  static xdata main_menu = 0;  
  xdata uint8_t lcd_line0[17], lcd_line1[17], i;

/*-----------------------------------------------------------------------------
  Evaluate menu independent buttons
-----------------------------------------------------------------------------*/

  if(buttons & (MAIN_MENU_INC | MAIN_MENU_DEC))
  {
    if(buttons & MAIN_MENU_INC)
    {
      main_menu = inc_mod(main_menu, MM_LAST_PAGE - 1);
    }
  
    if(buttons & MAIN_MENU_DEC)
    {
      main_menu = dec_mod(main_menu, MM_LAST_PAGE - 1);
    }   
    lcd_update(main_menu_header[main_menu], " ");  
    delay_ms(DELAY_SHORT_MESSAGE);
  }

  if(buttons & RUN_STOP)
  {
    if(radio_run)
    {
      radio_run = false;     
      gzll_goto_idle();
      lcd_update(MESSAGE_STOP_L0, MESSAGE_STOP_L1);  
      delay_ms(DELAY_SHORT_MESSAGE);
    }
    else
    {
      if(!device_mode)
      {
        gzll_rx_start();
      }
      radio_run = true;
      lcd_update(MESSAGE_RUN_L0, MESSAGE_RUN_L1);  
      delay_ms(DELAY_SHORT_MESSAGE);
    }
  }

  if(buttons & CLEAR_STATISTICS)
  {
    lcd_update(MESSAGE_STATISTICS_CLR_L0, MESSAGE_STATISTICS_CLR_L1);  
    delay_ms(DELAY_SHORT_MESSAGE);

    for(i = 0; i < STATISTICS_SIZE; i++)
    {
      statistics[i] = 0;
    }
  }

/*-----------------------------------------------------------------------------
  Run submenu dependent operations
-----------------------------------------------------------------------------*/

  switch(main_menu)
  {
    case MM_MODE_SELECT:
      mm_mode_select(lcd_line0, lcd_line1, buttons);
      break;
    case MM_TEST_APP_SETUP:
      mm_test_app_setup(lcd_line0, lcd_line1, buttons);
      break;
    case MM_GZLL_PARAMS:
      mm_gzll_params(lcd_line0, lcd_line1, buttons);
      break;
    case MM_STATISTICS:
      mm_statistics(lcd_line0, lcd_line1, buttons);
      break;
    case MM_CHANNEL_SETUP:
      mm_channel_setup(lcd_line0, lcd_line1, buttons);
      break;
  }

  lcd_update(lcd_line0, lcd_line1);  
} 

void lcd_update(char * l0, char * l1)
{
  xdata uint8_t  n0, n1;
  xdata char temp_l0[17], temp_l1[17]; 

  static xdata char curr_l0[17] = {0}, curr_l1[17] = {0}; 

  n0 = strlen(l0);
  n1 = strlen(l1);

  strcpy(temp_l0, l0);
  strcpy(temp_l1, l1);


  // Pad with white spaces at end of line
  for(; n0 < 16; n0++)
  {
    temp_l0[n0] = ' ';
  }
  temp_l0[16] = 0;
  
  for(; n1 < 16; n1++)
  {
    temp_l1[n1] = ' ';
  }
  temp_l1[16] = 0;

  if(strcmp(curr_l0, temp_l0) != 0)
  {
    strcpy(curr_l0, temp_l0);
    lcd_write_string(curr_l0, 0, 0);
  }
  
  if(strcmp(curr_l1, temp_l1) != 0)
  {
    strcpy(curr_l1, temp_l1);
    lcd_write_string(curr_l1, 1, 0);
    delay_ms(5);
  }
}

void mm_mode_select(char * lcd_l0, char * lcd_l1, uint8_t buttons)
{
  if((buttons & VALUE_INC) || (buttons & VALUE_DEC))
  {
    device_mode = !device_mode; 
  } 
     
  strcpy(lcd_l0, MM_MODE_SELECT_HEADER);

  if(device_mode)
  {
    sprintf(lcd_l1, MM_MODE_SELECT_DEVICE);     
  }
  else
  {
    sprintf(lcd_l1, MM_MODE_SELECT_HOST);       
  } 
}

void mm_test_app_setup(char * lcd_l0, char * lcd_l1, uint8_t buttons)
{
  static xdata uint8_t app_setup_page = 0;
  
  if(buttons & SUB_MENU_INC)
  {
    app_setup_page = inc_mod(app_setup_page, APP_SETUP_SIZE - 1);
  }

  if(buttons & SUB_MENU_DEC)
  {
    app_setup_page = dec_mod(app_setup_page, APP_SETUP_SIZE - 1);
  }

  if((buttons & VALUE_INC))
  {
    app_setup[app_setup_page] = inc_mod(app_setup[app_setup_page], app_setup_maximum[app_setup_page]);             
  }
  
  if((buttons & VALUE_DEC))
  {
    app_setup[app_setup_page] = dec_mod(app_setup[app_setup_page], app_setup_maximum[app_setup_page]);             
  }
  
  strcpy(lcd_l0, app_setup_menu_header[app_setup_page]);
  sprintf(lcd_l1, "%u", app_setup[app_setup_page]);
}

void mm_gzll_params(char * lcd_l0, char * lcd_l1, uint8_t buttons)
{
  static xdata uint8_t gzll_param_page = 0; 

  if(buttons & SUB_MENU_INC)
  {
    gzll_param_page = inc_mod(gzll_param_page, GZLL_DYN_PARAM_SIZE - 1);
  }

  if(buttons & SUB_MENU_DEC)
  {
    gzll_param_page = dec_mod(gzll_param_page, GZLL_DYN_PARAM_SIZE - 1);
  }

  if((buttons & VALUE_INC))
  {
    gzll_set_param(gzll_param_page, inc_mod(gzll_get_param(gzll_param_page),  gzll_get_param_max(gzll_param_page)));             
  }
  
  if((buttons & VALUE_DEC))
  {
    gzll_set_param(gzll_param_page, dec_mod(gzll_get_param(gzll_param_page),  gzll_get_param_max(gzll_param_page)));             
  }

  strcpy(lcd_l0, gzll_param_menu_header[gzll_param_page]);
  
  if(gzll_param_page == GZLL_PARAM_OUTPUT_POWER)
  {
    strcpy(lcd_l1, gzll_output_power[gzll_get_param(gzll_param_page)]);  
  }
  else
  {
    sprintf(lcd_l1, "%u", gzll_get_param(gzll_param_page));
  }
}

void mm_statistics(char * lcd_l0, char * lcd_l1, uint8_t buttons)
{
  static xdata uint8_t statistics_page = 0;

  if(buttons & SUB_MENU_INC)
  {
    statistics_page = inc_mod(statistics_page, STATISTICS_SIZE - 1);
  }

  if(buttons & SUB_MENU_DEC)
  {
    statistics_page = dec_mod(statistics_page, STATISTICS_SIZE - 1);
  }
  
  strcpy(lcd_l0, statistics_menu_header[statistics_page]);

  // Avg tx tries should be presented as a floating point number
  if(statistics_page == AVG_TX_TRIES)
  {
    sprintf(lcd_l1, "%.2f", (float)statistics[AVG_TX_TRIES] / 100);  
  }  
  else
  {
    sprintf(lcd_l1, "%lu", statistics[statistics_page]);
  }
}

void mm_channel_setup(char * lcd_l0, char * lcd_l1, uint8_t buttons)
{
  static xdata uint8_t channel_tab_shadow[GZLL_MAX_CHANNEL_TAB_SIZE] = GZLL_DEFAULT_CHANNEL_TAB;
  static xdata uint16_t channel_tab_size = GZLL_DEFAULT_CHANNEL_TAB_SIZE;
  static xdata uint16_t channel_page = GZLL_DEFAULT_CHANNEL_TAB_SIZE;
 
  if(buttons & SUB_MENU_INC)
  {
    channel_page = inc_mod(channel_page, channel_tab_size);
  }

  if(buttons & SUB_MENU_DEC)
  {
    channel_page = dec_mod(channel_page, channel_tab_size);
  }

  if(buttons & VALUE_DEC)
  {
    if(channel_page == channel_tab_size)
    {
      channel_tab_size = dec_mod(channel_tab_size, GZLL_MAX_CHANNEL_TAB_SIZE);
      channel_page = channel_tab_size;
    }
    else
    {
      channel_tab_shadow[channel_page] = dec_mod(channel_tab_shadow[channel_page], 120);
    }

    gzll_set_channels(channel_tab_shadow, channel_tab_size);
  }
  
  if(buttons & VALUE_INC)
  {
    if(channel_page == channel_tab_size)
    {
      channel_tab_size = inc_mod(channel_tab_size, GZLL_MAX_CHANNEL_TAB_SIZE);
      channel_page = channel_tab_size;
    }
    else
    {
      channel_tab_shadow[channel_page] = inc_mod(channel_tab_shadow[channel_page], 120);
    }
    
    gzll_set_channels(channel_tab_shadow, channel_tab_size);
  }
  
  if(channel_page == channel_tab_size)
  {
    strcpy(lcd_l0, "No of channels:");  
    sprintf(lcd_l1, "%u", channel_tab_size);
  }
  else
  {
    sprintf(lcd_l0, "RF channel: %u", channel_page);
    sprintf(lcd_l1, "%u", (uint16_t)channel_tab_shadow[channel_page]);
  }
}

void t2_init()
{
  T2CON = 0x11;                             // Reload mode 0, osc / 12 
  T2 = CRC = ~((1000 * 4) / 3);             // Set up period for timer 2
  
  ET2 = 1;
}
  
static void t2_interrupt(void) interrupt 5
{
  TF2 = 0;
  
  cnt_1ms++;
  if(button_db)
  {
    button_db--;
  }
}

static void rtc_interrupt(void) interrupt 13
{     
  gzll_timer_tick = true;
  gzll_timer_isr_function();                         
}

void delay_ms(uint16_t ms)
{
  uint16_t i, cnt;
  for(i = 0; i < ms; i++)
  {
    cnt = 1137;
    while(cnt--)
      ;
  }
}

uint8_t buttons_read()
{
  static xdata uint8_t hold = 0;

  uint8_t temp;
  
  if(button_db == 0)
  {
    temp = BUTTONS;
    temp = ~temp;
     
    if(temp)
    {
      if(hold < BUTTON_HOLD_THRESHOLD)
      {
        ET2 = 0;
        hold++;
        button_db = BUTTONS_DEBOUNCE_SLOW; 
      }
      else
      {
        button_db = BUTTONS_DEBOUNCE_FAST; 
      }
      ET2 = 1;
    }
    else
    {
      hold = 0;
    }
  
    return temp;
  }
  else
  {
    return 0;
  }
}

uint16_t inc_mod(uint16_t dat, uint16_t limit)
{
  dat++;
  if(dat > limit)
  {
    dat = 0;
  }
  return dat;
}

uint16_t dec_mod(uint16_t dat, uint16_t limit)
{
  if(dat == 0)
  {
    dat = limit;
  }
  else
  {
    dat--;
  }
  return dat;
}

/**
@}
*/
