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
 * @brief USB register layout and interrupts
 *
 * This file contain:
 * - the USB-controller register layout
 * - the USB-controller interrupts towards the MCU
 *
 * The usb_map_t structure is set to point at xdata address 0x0000
 */
#ifndef USB_MAP_H__
#define USB_MAP_H__

#ifdef ALLOCATE_USB_MAP
#define EXTERN
#define _AT_ _at_
#else
#define EXTERN extern
#define _AT_ ;/ ## /
#endif

#define USB_EP_DEFAULT_BUF_SIZE 0x20 // (32)

EXTERN xdata volatile uint8_t out5buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC440;
EXTERN xdata volatile uint8_t in5buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC480;
EXTERN xdata volatile uint8_t out4buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC4C0;
EXTERN xdata volatile uint8_t in4buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC500;
EXTERN xdata volatile uint8_t out3buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC540;
EXTERN xdata volatile uint8_t in3buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC580;
EXTERN xdata volatile uint8_t out2buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC5C0;
EXTERN xdata volatile uint8_t in2buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC600;
EXTERN xdata volatile uint8_t out1buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC640;
EXTERN xdata volatile uint8_t in1buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC680;
EXTERN xdata volatile uint8_t out0buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC6C0;
EXTERN xdata volatile uint8_t in0buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC700;
EXTERN xdata volatile uint8_t out8data                         _AT_ 0xC760;
EXTERN xdata volatile uint8_t in8data                          _AT_ 0xC768;
EXTERN xdata volatile uint8_t out8bch                          _AT_ 0xC770;
EXTERN xdata volatile uint8_t out8bcl                          _AT_ 0xC771;
EXTERN xdata volatile uint8_t bout1addr                        _AT_ 0xC781;
EXTERN xdata volatile uint8_t bout2addr                        _AT_ 0xC782;
EXTERN xdata volatile uint8_t bout3addr                        _AT_ 0xC783;
EXTERN xdata volatile uint8_t bout4addr                        _AT_ 0xC784;
EXTERN xdata volatile uint8_t bout5addr                        _AT_ 0xC785;
EXTERN xdata volatile uint8_t binstaddr                        _AT_ 0xC788;
EXTERN xdata volatile uint8_t bin1addr                         _AT_ 0xC789;
EXTERN xdata volatile uint8_t bin2addr                         _AT_ 0xC78A;
EXTERN xdata volatile uint8_t bin3addr                         _AT_ 0xC78B;
EXTERN xdata volatile uint8_t bin4addr                         _AT_ 0xC78C;
EXTERN xdata volatile uint8_t bin5addr                         _AT_ 0xC78D;
EXTERN xdata volatile uint8_t isoerr                           _AT_ 0xC7A0;
EXTERN xdata volatile uint8_t zbcout                           _AT_ 0xC7A2;
EXTERN xdata volatile uint8_t ivec                             _AT_ 0xC7A8;
EXTERN xdata volatile uint8_t in_irq                           _AT_ 0xC7A9;
EXTERN xdata volatile uint8_t out_irq                          _AT_ 0xC7AA;
EXTERN xdata volatile uint8_t usbirq                           _AT_ 0xC7AB;
EXTERN xdata volatile uint8_t in_ien                           _AT_ 0xC7AC;
EXTERN xdata volatile uint8_t out_ien                          _AT_ 0xC7AD;
EXTERN xdata volatile uint8_t usbien                           _AT_ 0xC7AE;
EXTERN xdata volatile uint8_t usbbav                           _AT_ 0xC7AF;
EXTERN xdata volatile uint8_t ep0cs                            _AT_ 0xC7B4;
EXTERN xdata volatile uint8_t in0bc                            _AT_ 0xC7B5;
EXTERN xdata volatile uint8_t in1cs                            _AT_ 0xC7B6;
EXTERN xdata volatile uint8_t in1bc                            _AT_ 0xC7B7;
EXTERN xdata volatile uint8_t in2cs                            _AT_ 0xC7B8;
EXTERN xdata volatile uint8_t in2bc                            _AT_ 0xC7B9;
EXTERN xdata volatile uint8_t in3cs                            _AT_ 0xC7BA;
EXTERN xdata volatile uint8_t in3bc                            _AT_ 0xC7BB;
EXTERN xdata volatile uint8_t in4cs                            _AT_ 0xC7BC;
EXTERN xdata volatile uint8_t in4bc                            _AT_ 0xC7BD;
EXTERN xdata volatile uint8_t in5cs                            _AT_ 0xC7BE;
EXTERN xdata volatile uint8_t in5bc                            _AT_ 0xC7BF;
EXTERN xdata volatile uint8_t out0bc                           _AT_ 0xC7C5;
EXTERN xdata volatile uint8_t out1cs                           _AT_ 0xC7C6;
EXTERN xdata volatile uint8_t out1bc                           _AT_ 0xC7C7;
EXTERN xdata volatile uint8_t out2cs                           _AT_ 0xC7C8;
EXTERN xdata volatile uint8_t out2bc                           _AT_ 0xC7C9;
EXTERN xdata volatile uint8_t out3cs                           _AT_ 0xC7CA;
EXTERN xdata volatile uint8_t out3bc                           _AT_ 0xC7CB;
EXTERN xdata volatile uint8_t out4cs                           _AT_ 0xC7CC;
EXTERN xdata volatile uint8_t out4bc                           _AT_ 0xC7CD;
EXTERN xdata volatile uint8_t out5cs                           _AT_ 0xC7CE;
EXTERN xdata volatile uint8_t out5bc                           _AT_ 0xC7CF;
EXTERN xdata volatile uint8_t usbcs                            _AT_ 0xC7D6;
EXTERN xdata volatile uint8_t togctl                           _AT_ 0xC7D7;
EXTERN xdata volatile uint8_t usbfrml                          _AT_ 0xC7D8;
EXTERN xdata volatile uint8_t usbfrmh                          _AT_ 0xC7D9;
EXTERN xdata volatile uint8_t fnaddr                           _AT_ 0xC7DB;
EXTERN xdata volatile uint8_t usbpair                          _AT_ 0xC7DD;
EXTERN xdata volatile uint8_t inbulkval                        _AT_ 0xC7DE;
EXTERN xdata volatile uint8_t outbulkval                       _AT_ 0xC7DF;
EXTERN xdata volatile uint8_t inisoval                         _AT_ 0xC7E0;
EXTERN xdata volatile uint8_t outisoval                        _AT_ 0xC7E1;
EXTERN xdata volatile uint8_t isostaddr                        _AT_ 0xC7E2;
EXTERN xdata volatile uint8_t isosize                          _AT_ 0xC7E3;
EXTERN xdata volatile uint8_t setupbuf[8]                      _AT_ 0xC7E8;
EXTERN xdata volatile uint8_t out8addr                         _AT_ 0xC7F0;
EXTERN xdata volatile uint8_t in8addr                          _AT_ 0xC7F8;

#endif
