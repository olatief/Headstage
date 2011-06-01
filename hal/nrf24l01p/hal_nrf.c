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
 * $LastChangedRevision: 5717 $
 */

 /** @file
  * @brief Implementation of hal_nrf
 */
/*lint -t2 */
/*lint -esym( 534, hal_nrf_write_reg ) */
/*lint -esym( 534, hal_nrf_rw ) */
/*lint -e708 */
/*lint -e46 */

#include <stdint.h>
#include <stdbool.h>

#include "nordic_common.h"
#include "hal_nrf.h"

sbit P0_0 = P0^0;
sbit P0_4 = P0^4;
sbit P0_5 = P0^5;
sbit P0_6 = P0^6;
sbit P0_7 = P0^7;

#define SET_BIT(pos) ((uint8_t) (1<<( (uint8_t) (pos) )))
#define UINT8(t) ((uint8_t) (t))

/** Basis function read_reg.
 * Use this function to read the contents
 * of one radios register.
 *
 * @param reg Register to read
 * @return Register contents
*/
static uint8_t hal_nrf_read_reg(uint8_t reg);

/** Basis function write_reg.
 * Use this function to write a new value to
 * a radio register.
 *
 * @param reg Register to write
 * @param value New value to write
 * @return Status register
*/
static uint8_t hal_nrf_write_reg(uint8_t data reg, uint8_t data value);

/** Basis function, read_multibyte register .
 * Use this function to read multiple bytes from
 * a multibyte radio-register
 *
 * @param reg Multibyte register to read from
 * @param *pbuf Pointer to buffer in which to store read bytes to
 *
 * @return pipe# of received data (MSB), if operation used by a hal_nrf_read_rx_pload
 * @return length of read data (LSB), either for hal_nrf_read_rx_pload or
 * for hal_nrf_get_address.
*/
//uint16_t hal_nrf_read_multibyte_reg(uint8_t data reg, uint8_t data *pbuf);

/** Basis function, write_multibyte register.
 * Use this function to write multiple bytes to
 * a multiple radio register.
 *
 * @param reg Register to write
 * @param *pbuf pointer to buffer in which data to write is
 * @param length \# of bytes to write
*/
static void hal_nrf_write_multibyte_reg(uint8_t reg, const uint8_t data *pbuf, uint8_t data length);

/**
 * Typedef for the CONFIG register. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		uint8_t prim_rx : 1;
		uint8_t pwr_up : 1;
		uint8_t crc0 : 1;
		uint8_t en_crc : 1;
		uint8_t mask_max_rt : 1;
		uint8_t mask_tx_ds : 1;
		uint8_t mask_rx_dr : 1;
		const uint8_t : 1;
	} bits;
} config_t;

/**
 * Typedef for the EN_AA, EN_RXADDR and DYNPD registers. Contains all the 
 * bitaddressable settings in the bits struct and the value sent to the radio 
 * in the uint8_t 
 */
typedef union {
  uint8_t value;
  struct {
    uint8_t pipe_0 : 1;
    uint8_t pipe_1 : 1;
    uint8_t pipe_2 : 1;
    uint8_t pipe_3 : 1;
    uint8_t pipe_4 : 1;
    uint8_t pipe_5 : 1;
    const uint8_t : 2;
  } bits;
} en_pipes_t;

/**
 * Typedef for the SETUP_AW register. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		uint8_t aw : 2;
		const uint8_t : 6;		
	} bits;
} setup_aw_t;

/**
 * Typedef for the SETUP_RETR register. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		uint8_t arc : 4;
		uint8_t ard : 4;
	} bits;
} setup_retr_t;

/**
 * Typedef for the RF_CH register. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		uint8_t rf_ch : 7;
		const uint8_t : 1;
	} bits;
} rf_ch_t;

/**
 * Typedef for the RF_SETUP register. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		const uint8_t : 1;
		uint8_t rf_pwr : 2;
		uint8_t rf_dr_high : 1;
		uint8_t pll_lock : 1;
		uint8_t rf_dr_low : 1;
    const uint8_t : 1;
    uint8_t cont_wave : 1;
	} bits;
} rf_setup_t;

/**
 * Typedef for the RX_PW_Px registers. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		uint8_t rx_pw : 6;
		const uint8_t : 2;
	} bits;
} rx_pw_t;

/**
 * Typedef for the FEATURE register. Contains all the bitaddressable 
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
  uint8_t value;
	struct {
		uint8_t en_dyn_ack : 1;
		uint8_t en_ack_pay : 1;
		uint8_t en_dpl : 1;
		const uint8_t : 5;
	} bits;
} feature_t;

void hal_nrf_set_operation_mode(hal_nrf_operation_mode_t op_mode)
{
  config_t config;
  config.value = hal_nrf_read_reg (CONFIG);

  if(op_mode == HAL_NRF_PRX)
  {
    config.bits.prim_rx = 1;
  }
  else
  {
    config.bits.prim_rx = 0;
  }

  hal_nrf_write_reg (CONFIG, config.value);
}

void hal_nrf_set_power_mode(hal_nrf_pwr_mode_t pwr_mode)
{
  config_t config;
  config.value = hal_nrf_read_reg (CONFIG);

  if(pwr_mode == HAL_NRF_PWR_UP)
  {
    config.bits.pwr_up = 1;
  }
  else
  {
    config.bits.pwr_up = 0;
  }
  
  hal_nrf_write_reg (CONFIG, config.value);
}

void hal_nrf_set_crc_mode(hal_nrf_crc_mode_t crc_mode)
{
  config_t config;
  config.value = hal_nrf_read_reg (CONFIG);

	switch (crc_mode)
	{
		case HAL_NRF_CRC_OFF:
			config.bits.en_crc = 0;
			break;
		case HAL_NRF_CRC_8BIT:
			config.bits.en_crc = 1;
			config.bits.crc0 = 0;
			break;
		case HAL_NRF_CRC_16BIT:
			config.bits.en_crc = 1;
			config.bits.crc0 = 1;
			break;
		default:
			break;
	} 

  hal_nrf_write_reg (CONFIG, config.value);
}

void hal_nrf_set_irq_mode(hal_nrf_irq_source_t int_source, bool irq_state)
{
  config_t config;
  config.value = hal_nrf_read_reg (CONFIG);

	switch (int_source)
	{
		case HAL_NRF_MAX_RT:
			config.bits.mask_max_rt = irq_state ? 0 : 1;
      break;
    case HAL_NRF_TX_DS:
      config.bits.mask_tx_ds = irq_state ? 0 : 1;
      break;
    case HAL_NRF_RX_DR:
      config.bits.mask_rx_dr = irq_state ? 0 : 1;
      break;
  }
  
  hal_nrf_write_reg (CONFIG, config.value);
}

uint8_t hal_nrf_get_clear_irq_flags(void)
{
  uint8_t retval;
  
  retval = hal_nrf_write_reg (STATUS, (BIT_6|BIT_5|BIT_4));

  return (retval & (BIT_6|BIT_5|BIT_4));
}

void hal_nrf_clear_irq_flag(hal_nrf_irq_source_t int_source)
{
  hal_nrf_write_reg (STATUS, SET_BIT(int_source));
}

uint8_t hal_nrf_get_irq_flags(void)
{
  return hal_nrf_nop() & (BIT_6|BIT_5|BIT_4);
}

void hal_nrf_open_pipe(hal_nrf_address_t pipe_num, bool auto_ack)
{
  en_pipes_t en_rxaddr;
  en_pipes_t en_aa;
  en_rxaddr.value = hal_nrf_read_reg (EN_RXADDR);
  en_aa.value = hal_nrf_read_reg (EN_AA);

  switch(pipe_num)
  {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_PIPE2:
    case HAL_NRF_PIPE3:
    case HAL_NRF_PIPE4:
    case HAL_NRF_PIPE5:
      en_rxaddr.value = en_rxaddr.value | SET_BIT(pipe_num);

      if(auto_ack)
      {
        en_aa.value = en_aa.value | SET_BIT(pipe_num);
      }
      else
      {
        en_aa.value = en_aa.value & ~SET_BIT(pipe_num);
      }
      break;

    case HAL_NRF_ALL:
      en_rxaddr.value = ~(BIT_6|BIT_7);
      
      if(auto_ack)
      {
        en_aa.value = ~(BIT_6|BIT_7);
      }
      else
      {
        en_aa.value = 0;
      }
      break;
      
    default:
      break;
  }

  hal_nrf_write_reg (EN_RXADDR, en_rxaddr.value);
  hal_nrf_write_reg (EN_AA, en_aa.value);
}

void hal_nrf_close_pipe(hal_nrf_address_t pipe_num)
{
  en_pipes_t en_rxaddr;
  en_pipes_t en_aa;
  en_rxaddr.value = hal_nrf_read_reg (EN_RXADDR);
  en_aa.value = hal_nrf_read_reg (EN_AA);

  switch(pipe_num)
  {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_PIPE2:
    case HAL_NRF_PIPE3:
    case HAL_NRF_PIPE4:
    case HAL_NRF_PIPE5:
      en_rxaddr.value = en_rxaddr.value & ~SET_BIT(pipe_num);
      en_aa.value = en_aa.value & ~SET_BIT(pipe_num);
      break;
    
    case HAL_NRF_ALL:
      en_rxaddr.value = 0;
      en_aa.value = 0;
      break;
      
    default:
      break;
  }

  hal_nrf_write_reg (EN_RXADDR, en_rxaddr.value);
  hal_nrf_write_reg (EN_AA, en_aa.value);
}

void hal_nrf_set_address(const hal_nrf_address_t address, const uint8_t *addr)
{
  switch(address)
  {
    case HAL_NRF_TX:
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
      hal_nrf_write_multibyte_reg(W_REGISTER + RX_ADDR_P0 + (uint8_t) address, addr, hal_nrf_get_address_width());
      break;
    case HAL_NRF_PIPE2:
    case HAL_NRF_PIPE3:
    case HAL_NRF_PIPE4:
    case HAL_NRF_PIPE5:
      hal_nrf_write_reg (RX_ADDR_P0 + (uint8_t) address, *addr);
      break;

    default:
      break;
  }
}

uint8_t hal_nrf_get_address(uint8_t address, uint8_t *addr)
{
  switch (address)
  {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_TX:
      return hal_nrf_read_multibyte_reg (address, addr);
    default:
      *addr = hal_nrf_read_reg(RX_ADDR_P0 + address);
      return 1;
  }
}

void hal_nrf_set_auto_retr(uint8_t retr, uint16_t delay)
{
  setup_retr_t setup_retr;
  setup_retr.bits.ard = (delay / 250) - 1;
  setup_retr.bits.arc = retr;

  hal_nrf_write_reg (SETUP_RETR, setup_retr.value);
}

void hal_nrf_set_address_width(hal_nrf_address_width_t aw)
{
  setup_aw_t setup_aw;
  setup_aw.bits.aw = (uint8_t)aw - 2;

  hal_nrf_write_reg (SETUP_AW, setup_aw.value);
}

uint8_t hal_nrf_get_address_width (void)
{
  return hal_nrf_read_reg (SETUP_AW) + 2;
}

void hal_nrf_set_rx_payload_width(uint8_t pipe_num, uint8_t pload_width)
{
  hal_nrf_write_reg (RX_PW_P0 + pipe_num, pload_width);
}

uint8_t hal_nrf_get_pipe_status(uint8_t pipe_num)
{
  en_pipes_t en_rxaddr;
  en_pipes_t en_aa;
  uint8_t en_rx_r, en_aa_r;
  
  en_rxaddr.value = hal_nrf_read_reg (EN_RXADDR);
  en_aa.value = hal_nrf_read_reg (EN_AA);

  switch (pipe_num)
  {
    case 0:
      en_rx_r = en_rxaddr.bits.pipe_0;
      en_aa_r = en_aa.bits.pipe_0;
      break;
    case 1:
      en_rx_r = en_rxaddr.bits.pipe_1;
      en_aa_r = en_aa.bits.pipe_1;
      break;
    case 2:
      en_rx_r = en_rxaddr.bits.pipe_2;
      en_aa_r = en_aa.bits.pipe_2;
      break;
    case 3:
      en_rx_r = en_rxaddr.bits.pipe_3;
      en_aa_r = en_aa.bits.pipe_3;
      break;
    case 4:
      en_rx_r = en_rxaddr.bits.pipe_4;
      en_aa_r = en_aa.bits.pipe_4;
      break;
    case 5:
      en_rx_r = en_rxaddr.bits.pipe_5;
      en_aa_r = en_aa.bits.pipe_5;
      break;
    default:
      en_rx_r = 0;
      en_aa_r = 0;
      break;
  }
  
  return (uint8_t)(en_aa_r << 1) + en_rx_r;
}

uint8_t hal_nrf_get_auto_retr_status(void)
{
  return hal_nrf_read_reg(OBSERVE_TX);
}

uint8_t hal_nrf_get_packet_lost_ctr(void)
{
  return ((hal_nrf_read_reg(OBSERVE_TX) & (BIT_7|BIT_6|BIT_5|BIT_4)) >> 4);
}

uint8_t hal_nrf_get_rx_payload_width(uint8_t pipe_num)
{
  uint8_t pw;

  switch (pipe_num)
  {
    case 0:
      pw = hal_nrf_read_reg (RX_PW_P0);
      break;      
    case 1:
      pw = hal_nrf_read_reg (RX_PW_P1);
      break;
    case 2:
      pw = hal_nrf_read_reg (RX_PW_P2);
      break;
    case 3:
      pw = hal_nrf_read_reg (RX_PW_P3);
      break;
    case 4:
      pw = hal_nrf_read_reg (RX_PW_P4);
      break;
    case 5:
      pw = hal_nrf_read_reg (RX_PW_P5);
      break;
    default:
      pw = 0;
      break;
  }

  return pw;
}

void hal_nrf_set_rf_channel(uint8_t channel)
{
  rf_ch_t rf_ch;
  rf_ch.bits.rf_ch = channel;
  
  hal_nrf_write_reg (RF_CH, rf_ch.value);
}

void hal_nrf_set_output_power(hal_nrf_output_power_t power)
{
  rf_setup_t rf_setup;
  rf_setup.value = hal_nrf_read_reg (RF_SETUP);

  rf_setup.bits.rf_pwr = (uint8_t)power;
  
  hal_nrf_write_reg (RF_SETUP, rf_setup.value);
}

void hal_nrf_set_datarate(hal_nrf_datarate_t datarate)
{
  rf_setup_t rf_setup;
  rf_setup.value = hal_nrf_read_reg (RF_SETUP);
  
  switch (datarate)
  {
    case HAL_NRF_250KBPS:
      rf_setup.bits.rf_dr_low = 1;
      rf_setup.bits.rf_dr_high = 0;
      break;
    case HAL_NRF_1MBPS:
      rf_setup.bits.rf_dr_low = 0;
      rf_setup.bits.rf_dr_high = 0;
      break;
    case HAL_NRF_2MBPS:
    default:
      rf_setup.bits.rf_dr_low = 0;
      rf_setup.bits.rf_dr_high = 1;
      break;
  }
  
  hal_nrf_write_reg (RF_SETUP, rf_setup.value);
}

bool hal_nrf_rx_fifo_empty(void)
{
  return (bool)((hal_nrf_read_reg(FIFO_STATUS) >> RX_EMPTY) & 1);
}

bool hal_nrf_rx_fifo_full(void)
{
  return (bool)((hal_nrf_read_reg(FIFO_STATUS)>> RX_FULL) & 1);
}

bool hal_nrf_tx_fifo_empty(void)
{
  return (bool)((hal_nrf_read_reg(FIFO_STATUS) >> TX_EMPTY) & 1);
}

bool hal_nrf_tx_fifo_full(void)
{
  return (bool)((hal_nrf_read_reg(FIFO_STATUS) >> TX_FIFO_FULL) & 1);
}

uint8_t hal_nrf_get_tx_fifo_status(void)
{
  return ((hal_nrf_read_reg(FIFO_STATUS) & ((1<<TX_FIFO_FULL)|(1<<TX_EMPTY))) >> 4);
}

uint8_t hal_nrf_get_rx_fifo_status(void)
{
  return (hal_nrf_read_reg(FIFO_STATUS) & ((1<<RX_FULL)|(1<<RX_EMPTY)));
}

uint8_t hal_nrf_get_fifo_status(void)
{
  return hal_nrf_read_reg(FIFO_STATUS);
}

uint8_t hal_nrf_get_transmit_attempts(void)
{
  return (hal_nrf_read_reg(OBSERVE_TX) & (BIT_3|BIT_2|BIT_1|BIT_0));
}

bool hal_nrf_get_carrier_detect(void)
{
  return hal_nrf_read_reg(CD) & 1;
}

void hal_nrf_activate_features(void)
{return;}

void hal_nrf_setup_dynamic_payload (uint8_t setup)
{
  en_pipes_t dynpd;
  dynpd.value = setup & ~0xC0;

  hal_nrf_write_reg (DYNPD, dynpd.value);
}

void hal_nrf_enable_dynamic_payload(bool enable)
{
  feature_t feature;
  feature.value = hal_nrf_read_reg (FEATURE);
  feature.bits.en_dpl = (enable) ? 1 : 0;

  hal_nrf_write_reg (FEATURE, feature.value);
}

void hal_nrf_enable_ack_payload(bool enable)
{
  feature_t feature;
  feature.value = hal_nrf_read_reg (FEATURE);
  feature.bits.en_ack_pay = (enable) ? 1 : 0;

  hal_nrf_write_reg (FEATURE, feature.value);
}

void hal_nrf_enable_dynamic_ack(bool enable)
{
  feature_t feature;
  feature.value = hal_nrf_read_reg (FEATURE);
  feature.bits.en_dyn_ack = (enable) ? 1 : 0;

  hal_nrf_write_reg (FEATURE, feature.value);
}

void hal_nrf_write_tx_payload(const uint8_t data *tx_pload, uint8_t data length)
{
  hal_nrf_write_multibyte_reg(W_TX_PAYLOAD, tx_pload, length);
}

void hal_nrf_write_tx_payload_noack(const uint8_t data *tx_pload, uint8_t data length)
{
  hal_nrf_write_multibyte_reg(W_TX_PAYLOAD_NOACK, tx_pload, length);
}

void hal_nrf_write_ack_payload(uint8_t pipe, const uint8_t data *tx_pload, uint8_t data length)
{
  hal_nrf_write_multibyte_reg(W_ACK_PAYLOAD | pipe, tx_pload, length);
}

uint8_t hal_nrf_read_rx_payload_width()
{
  return hal_nrf_read_reg(R_RX_PL_WID);
}

uint16_t hal_nrf_read_rx_payload(uint8_t data *rx_pload)
{
  return hal_nrf_read_multibyte_reg(UINT8(HAL_NRF_RX_PLOAD), rx_pload);
}

uint8_t hal_nrf_get_rx_data_source(void)
{
  return ((hal_nrf_nop() & (BIT_3|BIT_2|BIT_1)) >> 1);
}

void hal_nrf_reuse_tx(void)
{
  CSN_LOW();
  hal_nrf_rw(REUSE_TX_PL);
  CSN_HIGH();
}

bool hal_nrf_get_reuse_tx_status(void)
{
  return (bool)((hal_nrf_get_fifo_status() & (1<<TX_REUSE)) >> TX_REUSE);
}

void hal_nrf_flush_rx(void)
{
  CSN_LOW();
  hal_nrf_rw(FLUSH_RX);
  CSN_HIGH();
}

void hal_nrf_flush_tx(void)
{
  CSN_LOW();
  hal_nrf_rw(FLUSH_TX);
  CSN_HIGH();
}

uint8_t hal_nrf_nop(void)
{
  uint8_t retval;

  CSN_LOW();
  retval = hal_nrf_rw(NOP);
  CSN_HIGH();

  return retval;
}

void hal_nrf_set_pll_mode(bool pll_lock)
{
  rf_setup_t rf_setup;
  rf_setup.value = hal_nrf_read_reg (RF_SETUP);
  rf_setup.bits.pll_lock = (pll_lock) ? 1 : 0;

  hal_nrf_write_reg(RF_SETUP, rf_setup.value);
}

void hal_nrf_enable_continious_wave (bool enable)
{
  rf_setup_t rf_setup;
  rf_setup.value = hal_nrf_read_reg (RF_SETUP);
  rf_setup.bits.cont_wave = (enable ? 1 : 0);

  hal_nrf_write_reg(RF_SETUP, rf_setup.value);
}

static uint8_t hal_nrf_read_reg(uint8_t data reg)
{
  uint8_t temp;

  CSN_LOW();
  hal_nrf_rw(reg);
  temp = hal_nrf_rw(0);
  CSN_HIGH();

  return temp;
}

uint8_t hal_nrf_write_reg(uint8_t data reg, uint8_t data value)
{
  uint8_t data retval;
  
  CSN_LOW();
  // retval = hal_nrf_rw(W_REGISTER + reg);

  RFDAT = W_REGISTER + reg;
  RFSPIF = 0;     // ! IMPORTANT ! Clear RF SPI ready flag
                  // after data written to RFDAT..
  while(!RFSPIF); // wait for byte transfer finished
    
  retval = RFDAT;   // return SPI read value


  RFDAT = value;
  RFSPIF = 0;     // ! IMPORTANT ! Clear RF SPI ready flag
                  // after data written to RFDAT..
  while(!RFSPIF); // wait for byte transfer finished
    
  // retval = RFDAT;   // return SPI read value

 //  hal_nrf_rw(value);
  CSN_HIGH();

  return retval;
}

uint16_t hal_nrf_read_multibyte_reg(uint8_t data reg, uint8_t data *pbuf)
{
  uint8_t data ctr, length;
  switch(reg)
  {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_TX:
      length = ctr = hal_nrf_get_address_width();
      CSN_LOW();
      hal_nrf_rw(RX_ADDR_P0 + reg);
      break;
      
    case HAL_NRF_RX_PLOAD:
      if( (reg = hal_nrf_get_rx_data_source()) < 7)
      {
        length = ctr = hal_nrf_read_rx_payload_width();
        CSN_LOW();
        hal_nrf_rw(R_RX_PAYLOAD);
      }
      else
      {
        ctr = length = 0;
      }
      break;

    default:
      ctr = length = 0;
      break;
  }

  while(ctr--)
  {
    *pbuf++ = hal_nrf_rw(0);
  }

  CSN_HIGH();

  return (((uint16_t) reg << 8) | length);
}

static void hal_nrf_write_multibyte_reg( uint8_t data cmd, const uint8_t data *pbuf, uint8_t data length)
{
  data uint8_t dummy;

  CSN_LOW();

  HAL_NRF_HW_SPI_WRITE(cmd);

  for(; length > 0; --length, ++pbuf)
 {
   // next = *(pbuf + i);
    while(HAL_NRF_HW_SPI_BUSY);  // wait for byte transfer finished
    ;
    dummy = HAL_NRF_HW_SPI_READ();
    HAL_NRF_HW_SPI_WRITE(*pbuf);    
  }
 
  while(HAL_NRF_HW_SPI_BUSY);  // wait for byte transfer finished
  ;
  dummy = HAL_NRF_HW_SPI_READ();
  CSN_HIGH();
}
