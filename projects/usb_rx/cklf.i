
#line 1 "..\..\hal\nrf24lu1p\cklf.c" /0











 
 


 
 
  
#line 1 "C:\KEIL\C51\INC\NORDIC\REG24LU1.H" /0







 
 
 
 
 
 
 
 
 sfr   P0           = 0x80;
 sfr   SP           = 0x81;
 sfr   DPL          = 0x82;
 sfr   DPH          = 0x83;
 sfr   DPL1         = 0x84;
 sfr   DPH1         = 0x85;
 sfr   PCON         = 0x87;
 sfr   TCON         = 0x88;
 sfr   TMOD         = 0x89;
 sfr   TL0          = 0x8A;
 sfr   TL1          = 0x8B;
 sfr   TH0          = 0x8C;
 sfr   TH1          = 0x8D;
 sfr   CKCON        = 0x8E;
 sfr   RFCON        = 0x90;
 sfr   DPS          = 0x92;
 sfr   P0DIR        = 0x94;
 sfr   P0ALT        = 0x95;
 sfr   S0CON        = 0x98;
 sfr   S0BUF        = 0x99;
 sfr   IEN2         = 0x9A;
 sfr   USBCON       = 0xA0;
 sfr   CLKCTL       = 0xA3;
 sfr   PWRDWN       = 0xA4;
 sfr   WUCONF       = 0xA5;
 sfr   INTEXP       = 0xA6;
 sfr   IEN0         = 0xA8;
 sfr   IP0          = 0xA9;
 sfr   S0RELL       = 0xAA;
 sfr   REGXH        = 0xAB;
 sfr   REGXL        = 0xAC;
 sfr   REGXC        = 0xAD;
 sfr   RSTRES       = 0xB1;
 sfr   SMDAT        = 0xB2;
 sfr   SMCTL        = 0xB3;
 sfr   TICKDV       = 0xB5;
 sfr   IEN1         = 0xB8;
 sfr   IP1          = 0xB9;
 sfr   S0RELH       = 0xBA;
 sfr   SSCONF       = 0xBC;
 sfr   SSDATA       = 0xBD;
 sfr   SSSTAT       = 0xBE;
 sfr   IRCON        = 0xC0;
 sfr   CCEN         = 0xC1;
 sfr   CCL1         = 0xC2;
 sfr   CCH1         = 0xC3;
 sfr   CCL2         = 0xC4;
 sfr   CCH2         = 0xC5;
 sfr   CCL3         = 0xC6;
 sfr   CCH3         = 0xC7;
 sfr   T2CON        = 0xC8;
 sfr   P0EXP        = 0xC9;
 sfr   CRCL         = 0xCA;
 sfr   CRCH         = 0xCB;
 sfr   TL2          = 0xCC;
 sfr   TH2          = 0xCD;
 sfr   PSW          = 0xD0;
 sfr   WDCON        = 0xD8;
 sfr   USBSLP       = 0xD9;
 sfr   ACC          = 0xE0;
 sfr   RFDAT        = 0xE5;
 sfr   RFCTL        = 0xE6;
 sfr   AESCS        = 0xE8;
 sfr   MD0          = 0xE9;
 sfr   MD1          = 0xEA;
 sfr   MD2          = 0xEB;
 sfr   MD3          = 0xEC;
 sfr   MD4          = 0xED;
 sfr   MD5          = 0xEE;
 sfr   ARCON        = 0xEF;
 sfr   B            = 0xF0;
 sfr   AESKIN       = 0xF1;
 sfr   AESIV        = 0xF2;
 sfr   AESD         = 0xF3;
 sfr   AESIA1       = 0xF5;
 sfr   AESIA2       = 0xF6;
 sfr   FSR          = 0xF8;
 sfr   FPCR         = 0xF9;
 sfr   FCR          = 0xFA;
 
 
 
 
 
 sfr16 CC1          = 0xC2;
 sfr16 CC2          = 0xC4;
 sfr16 CC3          = 0xC6;
 sfr16 CRC          = 0xCA;
 sfr16 T2           = 0xCC;
 
 




 
 
 
 
 
 
 
 
 
 
 sbit  MCDIS        = FSR^7;
 sbit  STP          = FSR^6;
 sbit  WEN          = FSR^5;
 sbit  RDYN         = FSR^4;
 sbit  INFEN        = FSR^3;
 sbit  RDIS         = FSR^2;
 sbit  RDEND        = FSR^1;
 sbit  WPEN         = FSR^0;
 
 
 sbit  CY           = PSW^7;
 sbit  AC           = PSW^6;
 sbit  F0           = PSW^5;
 sbit  RS1          = PSW^4;
 sbit  RS0          = PSW^3;
 sbit  OV           = PSW^2;
 sbit  F1           = PSW^1;
 sbit  P            = PSW^0;
 
 
 sbit  TF1          = TCON^7;
 sbit  TR1          = TCON^6;
 sbit  TF0          = TCON^5;
 sbit  TR0          = TCON^4;
 sbit  IE1          = TCON^3;
 sbit  IT1          = TCON^2;
 sbit  IE0          = TCON^1;
 sbit  IT0          = TCON^0;
 
 
 sbit  SM0          = S0CON^7;
 sbit  SM1          = S0CON^6;
 sbit  SM20         = S0CON^5;
 sbit  REN0         = S0CON^4;
 sbit  TB80         = S0CON^3;
 sbit  RB80         = S0CON^2;
 sbit  TI0          = S0CON^1;
 sbit  RI0          = S0CON^0;
 
 
 sbit  T2PS         = T2CON^7;
 sbit  I3FR         = T2CON^6;
 sbit  I2FR         = T2CON^5;
 sbit  T2R1         = T2CON^4;
 sbit  T2R0         = T2CON^3;
 sbit  T2CM         = T2CON^2;
 sbit  T2I1         = T2CON^1;
 sbit  T2I0         = T2CON^0;
 
 
 sbit  EA           = IEN0^7;
 
 sbit  ET2          = IEN0^5;
 sbit  ES0          = IEN0^4;
 sbit  ET1          = IEN0^3;
 sbit  EX1          = IEN0^2;
 sbit  ET0          = IEN0^1;
 sbit  EX0          = IEN0^0;
 
 
 sbit  EXEN2        = IEN1^7;
 
 sbit  WUIRQ        = IEN1^5;
 sbit  USB          = IEN1^4;
 sbit  USBWU        = IEN1^3;
 sbit  SPI          = IEN1^2;
 sbit  RF           = IEN1^1;
 sbit  RFSPI        = IEN1^0;
 
 
 sbit  EXF2         = IRCON^7;
 sbit  TF2          = IRCON^6;
 sbit  WUF          = IRCON^5;
 sbit  USBF         = IRCON^4;
 sbit  USBWUF       = IRCON^3;
 sbit  SPIF         = IRCON^2;
 sbit  RFF          = IRCON^1;
 sbit  RFSPIF       = IRCON^0;
 
 
 sbit  SWRST        = USBCON^7;
 sbit  WU           = USBCON^6;
 sbit  SUSPEND      = USBCON^5;
 sbit  IV4          = USBCON^4;
 sbit  IV3          = USBCON^3;
 sbit  IV2          = USBCON^2;
 sbit  IV1          = USBCON^1;
 sbit  IV0          = USBCON^0;
 
 
 sbit  P00          = P0^0;
 sbit  P01          = P0^1;
 sbit  P02          = P0^2;
 sbit  P03          = P0^3;
 sbit  MCSN         = P0^3;
 sbit  SCSN         = P0^3;
 sbit  P04          = P0^4;
 sbit  P05          = P0^5;
 
 
 sbit  RFCE         = RFCON^0;
 sbit  RFCSN        = RFCON^1;
 sbit  RFCKEN       = RFCON^2;
 
 
 sbit  BD           = WDCON^7;
 
 
 sbit  GO           = AESCS^0;
 sbit  DECR         = AESCS^1;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 18 "..\..\hal\nrf24lu1p\cklf.c" /0
 
  
#line 1 "..\..\HAL\NRF24LU1P\CKLF.H" /0











 
 






 
 
 
 
  
#line 1 "..\..\COMPILER\C51\STDINT.H" /0











 
 








 
 
 
 
 
 
 
 typedef unsigned char uint8_t;         
 
 typedef signed char int8_t;           
 
 typedef unsigned int uint16_t;         
 
 typedef signed int int16_t;           
 
 typedef unsigned long uint32_t;        
 
 typedef signed long int32_t;          
 
 
 
 
 
 
 
 
#line 24 "..\..\HAL\NRF24LU1P\CKLF.H" /0
 
 



 
 void cklf_regxc_write(uint8_t addr, uint16_t val);
 



 
 uint16_t cklf_regxc_read(uint8_t addr);
 


 
 void cklf_rtc_disable(void);
 



 
 void cklf_rtc_init(uint8_t cnt_h, uint16_t cnt_l);
 




 
 uint16_t cklf_rtc_read_lsw(void);
 




 
 uint8_t cklf_rtc_read_msb(void);
 




 
 void cklf_rtc_wait(void);
 


 
 void cklf_wdog_init(uint16_t cnt);
 



 
 void cklf_wdog_feed(void);
 



 
 void cklf_gpio_wakeup(uint16_t wcon1, uint16_t wcon0);
 
 
 
#line 19 "..\..\hal\nrf24lu1p\cklf.c" /0
 
 
 static uint16_t wd_cnt;
 
 void cklf_regxc_write(uint8_t addr, uint16_t val)
 {
 REGXH = val >> 8;
 REGXL = val & 0xff;
 REGXC = addr;
 }
 
 uint16_t cklf_regxc_read(uint8_t addr)
 {
 uint16_t val;
 
 REGXC = addr;
 val = (uint16_t)REGXH;
 val <<= 8;
 val |= (uint16_t)REGXL;
 return val;
 }
 
 void cklf_rtc_disable(void)
 {
 cklf_regxc_write(0x0B, 0);
 }
 
 void cklf_rtc_init(uint8_t cnt_h, uint16_t cnt_l)
 {
 cklf_regxc_write(0x0B, 0);                 
 WUF = 0;                                      
 cklf_regxc_write(0x09, (uint16_t)cnt_h);   
 cklf_regxc_write(0x0A, cnt_l);             
 }
 
 uint16_t cklf_rtc_read_lsw(void)
 {
 return cklf_regxc_read(0x03);
 }
 
 uint8_t cklf_rtc_read_msb(void)
 {
 return (uint8_t)(cklf_regxc_read(0x01) >> 8);
 }
 
 void cklf_rtc_wait(void)
 {
 while(WUF == 0)                          
 ;
 WUF = 0;
 }
 
 void cklf_wdog_init(uint16_t cnt)
 {
 wd_cnt = cnt;
 cklf_regxc_write(0x08, wd_cnt);
 }
 
 void cklf_wdog_feed(void)
 {
 cklf_regxc_write(0x08, wd_cnt);
 }
 
 void cklf_gpio_wakeup(uint16_t wcon1, uint16_t wcon0)
 {
 cklf_regxc_write(0x0D, wcon1);
 cklf_regxc_write(0x0C, wcon0);
 }
