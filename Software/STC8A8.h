/*-------------------------------------------------------------------------
 STC8A8.h
 
 H-file for STC8A8K64D4 etc.
-------------------------------------------------------------------------*/

#ifndef _STC8A8_H
#define _STC8A8_H

#include <8051.h>
#include <compiler.h>

#ifdef REG8051_H
#undef REG8051_H
#endif

// --- AUXR -------------------------------------------------------------------------------------

SFR(AUXR,0x8E);
#define T0x12     0x80    // AUXR.7
#define T1x12     0x40    // AUXR.6
#define UART_M0x6 0x20    // AUXR.5
#define T2R       0x10    // AUXR.4
#define T2_CT     0x08    // AUXR.3
#define T2x12     0x04    // AUXR.2
#define EXTRAM    0x02    // AUXR.1
#define S1ST2     0x01    // AUXR.0

// --- Pin switch -------------------------------------------------------------------------------------

SFR(P_SW1,0xA2);
#define S1_S1     0x80    // P_SW1.7
#define S1_S0     0x40    // P_SW1.6
#define CCP_S1    0x20    // P_SW1.5
#define CCP_S0    0x10    // P_SW1.4
#define SPI_S1    0x08    // P_SW1.3
#define SPI_S0    0x04    // P_SW1.2

SFR(P_SW2,0xBA);
#define EAXFR     0x80    // P_SW2.7
#define I2C_S1    0x20    // P_SW2.5
#define I2C_S0    0x10    // P_SW2.4
#define CMPO_S    0x08    // P_SW2.3
#define S4_S      0x04    // P_SW2.2
#define S3_S      0x02    // P_SW2.1
#define S2_S      0x01    // P_SW2.0

// --- Interrupts -------------------------------------------------------------------------------------

// IE deviations
SBIT(EADC, 0xA8, 5);    // normally ET2
SBIT(ELVD, 0xA8, 6);    // normally EC

// IE2
SFR(IE2,0xAF);
#define ET4     0x40    // IE2.6
#define ET3     0x20    // IE2.5
#define ES4     0x10    // IE2.4
#define ES3     0x08    // IE2.3
#define ET2     0x04    // IE2.2
#define ESPI    0x02    // IE2.1
#define ES2     0x01    // IE2.0

// INTCLKO
SFR(INTCLKO,0x8F);
#define EX4     0x40    // INTCLKO.6
#define EX3     0x20    // INTCLKO.5
#define EX2     0x10    // INTCLKO.4
#define T2CLKO  0x04    // INTCLKO.2
#define T1CLKO  0x02    // INTCLKO.1
#define T0CLKO  0x01    // INTCLKO.0

// IP deviations
SBIT(PADC, 0xB8, 5);    // normally PT2
SBIT(PLVD, 0xB8, 6);    // normally PPC
SBIT(PPCA, 0xB8, 7);

SFR(AUXINTIF,0xEF); 
#define INT4IF  0x40    // AUXINTIF.6
#define INT3IF  0x20    // AUXINTIF.5
#define INT2IF  0x10    // AUXINTIF.4
#define T4IF    0x04    // AUXINTIF.2
#define T3IF    0x02    // AUXINTIF.1
#define T2IF    0x01    // AUXINTIF.0


// IPH
SFR(IPH,0xB7);
#define PPCAH   0x80    // IPH.7
#define PLVDH   0x40    // IPH.6
#define PADCH   0x20    // IPH.5
#define PSH     0x10    // IPH.4
#define PT1H    0x08    // IPH.3
#define PX1H    0x04    // IPH.2
#define PT0H    0x02    // IPH.1
#define PX0H    0x01    // IPH.0

// IP2
SFR(IP2,0xB5);
#define PI2C    0x40    // IP2.6
#define PCMP    0x20    // IP2.5
#define PX4     0x10    // IP2.4
#define PPWMFD  0x08    // IP2.3
#define PPWM    0x04    // IP2.2
#define PSPI    0x02    // IP2.1
#define PS2     0x01    // IP2.0

// IP2H
SFR(IP2H ,0xB6);
#define PI2CH   0x40    // IP2H.6
#define PCMPH   0x20    // IP2H.5
#define PX4H    0x10    // IP2H.4
#define PPWMFDH 0x08    // IP2H.3
#define PPWMH   0x04    // IP2H.2
#define PSPIH   0x02    // IP2H.1
#define PS2H    0x01    // IP2H.0

// IP3
SFR(IP3,0xDF); 
#define PS4     0x02    // IP3.1
#define PS3     0x01    // IP3.0

// IP3H
SFR(IP3H,0xEE);
#define PS4H    0x02    // IP3H.1
#define PS3H    0x01    // IP3H.0

#define P0INTE         (*(unsigned char volatile __xdata *)0xFD00)
#define P1INTE         (*(unsigned char volatile __xdata *)0xFD01)
#define P2INTE         (*(unsigned char volatile __xdata *)0xFD02)
#define P3INTE         (*(unsigned char volatile __xdata *)0xFD03)
#define P4INTE         (*(unsigned char volatile __xdata *)0xFD04)
#define P5INTE         (*(unsigned char volatile __xdata *)0xFD05)
#define P6INTE         (*(unsigned char volatile __xdata *)0xFD06)
#define P7INTE         (*(unsigned char volatile __xdata *)0xFD07)

#define P0INTF         (*(unsigned char volatile __xdata *)0xFD10)
#define P1INTF         (*(unsigned char volatile __xdata *)0xFD11)
#define P2INTF         (*(unsigned char volatile __xdata *)0xFD12)
#define P3INTF         (*(unsigned char volatile __xdata *)0xFD13)
#define P4INTF         (*(unsigned char volatile __xdata *)0xFD14)
#define P5INTF         (*(unsigned char volatile __xdata *)0xFD15)
#define P6INTF         (*(unsigned char volatile __xdata *)0xFD16)
#define P7INTF         (*(unsigned char volatile __xdata *)0xFD17)

// TODO: similar macros than for I/O modes
// IM1,IM0:
//   0,0 = falling edge
//   0,1 = rising edge
//   1,0 = low level
//   1,1 = high level

#define P0IM0          (*(unsigned char volatile __xdata *)0xFD20)
#define P1IM0          (*(unsigned char volatile __xdata *)0xFD21)
#define P2IM0          (*(unsigned char volatile __xdata *)0xFD22)
#define P3IM0          (*(unsigned char volatile __xdata *)0xFD23)
#define P4IM0          (*(unsigned char volatile __xdata *)0xFD24)
#define P5IM0          (*(unsigned char volatile __xdata *)0xFD25)
#define P6IM0          (*(unsigned char volatile __xdata *)0xFD26)
#define P7IM0          (*(unsigned char volatile __xdata *)0xFD27)

#define P0IM1          (*(unsigned char volatile __xdata *)0xFD30)
#define P1IM1          (*(unsigned char volatile __xdata *)0xFD31)
#define P2IM1          (*(unsigned char volatile __xdata *)0xFD32)
#define P3IM1          (*(unsigned char volatile __xdata *)0xFD33)
#define P4IM1          (*(unsigned char volatile __xdata *)0xFD34)
#define P5IM1          (*(unsigned char volatile __xdata *)0xFD35)
#define P6IM1          (*(unsigned char volatile __xdata *)0xFD36)
#define P7IM1          (*(unsigned char volatile __xdata *)0xFD37)

#define PINIPL         (*(unsigned char volatile __xdata *)0xFD60)
#define PINIPH         (*(unsigned char volatile __xdata *)0xFD61)

#define P0WKUE         (*(unsigned char volatile __xdata *)0xFD40)
#define P1WKUE         (*(unsigned char volatile __xdata *)0xFD41)
#define P2WKUE         (*(unsigned char volatile __xdata *)0xFD42)
#define P3WKUE         (*(unsigned char volatile __xdata *)0xFD43)
#define P4WKUE         (*(unsigned char volatile __xdata *)0xFD44)
#define P5WKUE         (*(unsigned char volatile __xdata *)0xFD45)
#define P6WKUE         (*(unsigned char volatile __xdata *)0xFD46)
#define P7WKUE         (*(unsigned char volatile __xdata *)0xFD47)

// --- I/O ports --------------------------------------------------------------------------------------

// Macros for port I/O modes

// PxM1,PxM0: 
//   0,0 -> quasi bd
//   0,1 -> push-pull
//   1,0 -> input (default)
//   1,1 -> open drain

#define B0   0x01
#define B1   0x02
#define B2   0x04
#define B3   0x08
#define B4   0x10
#define B5   0x20
#define B6   0x40
#define B7   0x80
#define ALL  0xFF

#define QUASIBD   0
#define PUSHPULL  1
#define INPUT     2
#define OPENDRAIN 3

#define P0_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P0M1 &= ~(pins); else P0M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P0M0 &= ~(pins); else P0M0 |= (pins); }
#define P1_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P1M1 &= ~(pins); else P1M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P1M0 &= ~(pins); else P1M0 |= (pins); }
#define P2_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P2M1 &= ~(pins); else P2M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P2M0 &= ~(pins); else P2M0 |= (pins); }
#define P3_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P3M1 &= ~(pins); else P3M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P3M0 &= ~(pins); else P3M0 |= (pins); }
#define P4_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P4M1 &= ~(pins); else P4M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P4M0 &= ~(pins); else P4M0 |= (pins); }
#define P5_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P5M1 &= ~(pins); else P5M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P5M0 &= ~(pins); else P5M0 |= (pins); }
#define P6_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P6M1 &= ~(pins); else P6M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P6M0 &= ~(pins); else P6M0 |= (pins); }
#define P7_pinmode(mode,pins)  { if (mode==QUASIBD || mode==PUSHPULL) P7M1 &= ~(pins); else P7M1 |= (pins);\
                                 if (mode==QUASIBD || mode==INPUT)    P7M0 &= ~(pins); else P7M0 |= (pins); }

SFR(P4,0xC0);
SFR(P5,0xC8);
SFR(P6,0xE8);
SFR(P7,0xF8);

SFR(P0M1,0x93);
SFR(P0M0,0x94);
SFR(P1M1,0x91);
SFR(P1M0,0x92);
SFR(P2M1,0x95);
SFR(P2M0,0x96);
SFR(P3M1,0xB1);
SFR(P3M0,0xB2);
SFR(P4M1,0xB3);
SFR(P4M0,0xB4);
SFR(P5M1,0xC9);
SFR(P5M0,0xCA);
SFR(P6M1,0xCB);
SFR(P6M0,0xCC);
SFR(P7M1,0xE1);
SFR(P7M0,0xE2);

SBIT(P4_0,0xC0,0);
SBIT(P4_1,0xC0,1);
SBIT(P4_2,0xC0,2);
SBIT(P4_3,0xC0,3);
SBIT(P4_4,0xC0,4);
SBIT(P4_5,0xC0,5);
SBIT(P4_6,0xC0,6);
SBIT(P4_7,0xC0,7);

SBIT(P5_0,0xC8,0);
SBIT(P5_1,0xC8,1);
SBIT(P5_2,0xC8,2);
SBIT(P5_3,0xC8,3);
SBIT(P5_4,0xC8,4);
SBIT(P5_5,0xC8,5);
SBIT(P5_6,0xC8,6);
SBIT(P5_7,0xC8,7);

SBIT(P6_0,0xE8,0);
SBIT(P6_1,0xE8,1);
SBIT(P6_2,0xE8,2);
SBIT(P6_3,0xE8,3);
SBIT(P6_4,0xE8,4);
SBIT(P6_5,0xE8,5);
SBIT(P6_6,0xE8,6);
SBIT(P6_7,0xE8,7);

SBIT(P7_0,0xF8,0);
SBIT(P7_1,0xF8,1);
SBIT(P7_2,0xF8,2);
SBIT(P7_3,0xF8,3);
SBIT(P7_4,0xF8,4);
SBIT(P7_5,0xF8,5);
SBIT(P7_6,0xF8,6);
SBIT(P7_7,0xF8,7);

#define P0PU           (*(unsigned char volatile __xdata *)0xFE10)
#define P1PU           (*(unsigned char volatile __xdata *)0xFE11)
#define P2PU           (*(unsigned char volatile __xdata *)0xFE12)
#define P3PU           (*(unsigned char volatile __xdata *)0xFE13)
#define P4PU           (*(unsigned char volatile __xdata *)0xFE14)
#define P5PU           (*(unsigned char volatile __xdata *)0xFE15)
#define P6PU           (*(unsigned char volatile __xdata *)0xFE16)
#define P7PU           (*(unsigned char volatile __xdata *)0xFE17)
                   
#define P0NCS          (*(unsigned char volatile __xdata *)0xFE18)
#define P1NCS          (*(unsigned char volatile __xdata *)0xFE19)
#define P2NCS          (*(unsigned char volatile __xdata *)0xFE1A)
#define P3NCS          (*(unsigned char volatile __xdata *)0xFE1B)
#define P4NCS          (*(unsigned char volatile __xdata *)0xFE1C)
#define P5NCS          (*(unsigned char volatile __xdata *)0xFE1D)
#define P6NCS          (*(unsigned char volatile __xdata *)0xFE1E)
#define P7NCS          (*(unsigned char volatile __xdata *)0xFE1F)
                   
#define P0SR           (*(unsigned char volatile __xdata *)0xFE20)
#define P1SR           (*(unsigned char volatile __xdata *)0xFE21)
#define P2SR           (*(unsigned char volatile __xdata *)0xFE22)
#define P3SR           (*(unsigned char volatile __xdata *)0xFE23)
#define P4SR           (*(unsigned char volatile __xdata *)0xFE24)
#define P5SR           (*(unsigned char volatile __xdata *)0xFE25)
#define P6SR           (*(unsigned char volatile __xdata *)0xFE26)
#define P7SR           (*(unsigned char volatile __xdata *)0xFE27)
                     
#define P0DR           (*(unsigned char volatile __xdata *)0xFE28)
#define P1DR           (*(unsigned char volatile __xdata *)0xFE29)
#define P2DR           (*(unsigned char volatile __xdata *)0xFE2A)
#define P3DR           (*(unsigned char volatile __xdata *)0xFE2B)
#define P4DR           (*(unsigned char volatile __xdata *)0xFE2C)
#define P5DR           (*(unsigned char volatile __xdata *)0xFE2D)
#define P6DR           (*(unsigned char volatile __xdata *)0xFE2E)
#define P7DR           (*(unsigned char volatile __xdata *)0xFE2F)

// Set to 0, when used as analog input (default = 1)
#define P0IE           (*(unsigned char volatile __xdata *)0xFE30)
#define P1IE           (*(unsigned char volatile __xdata *)0xFE31)
#define P2IE           (*(unsigned char volatile __xdata *)0xFE32)
#define P3IE           (*(unsigned char volatile __xdata *)0xFE33)
#define P4IE           (*(unsigned char volatile __xdata *)0xFE34)
#define P5IE           (*(unsigned char volatile __xdata *)0xFE35)
#define P6IE           (*(unsigned char volatile __xdata *)0xFE36)
#define P7IE           (*(unsigned char volatile __xdata *)0xFE37)

// --- Timers ------------------------------------------------------------------------------------

SFR(T4T3M,0xD1);
#define T4R     0x80    // T4T3M.7
#define T4_CT   0x40    // T4T3M.6
#define T4x12   0x20    // T4T3M.5
#define T4CLKO  0x10    // T4T3M.4
#define T3R     0x08    // T4T3M.3
#define T3_CT   0x04    // T4T3M.2
#define T3x12   0x02    // T4T3M.1
#define T3CLKO  0x01    // T4T3M.0

SFR(T4H,0xD2);
SFR(T4L,0xD3);
SFR(T3H,0xD4);
SFR(T3L,0xD5);
SFR(T2H,0xD6);
SFR(T2L,0xD7);

SFR16E(TMR2,0xD6D7);
SFR16E(TMR3,0xD4D5);
SFR16E(TMR4,0xD2D3);

SFR(WKTCL,0xAA);
SFR(WKTCH,0xAB);
#define WKTEN   0x80    // WKTCH.7

#define TM2PS          (*(unsigned char volatile __xdata *)0xFEA2)
#define TM3PS          (*(unsigned char volatile __xdata *)0xFEA3)
#define TM4PS          (*(unsigned char volatile __xdata *)0xFEA4)
#define ADCTIM         (*(unsigned char volatile __xdata *)0xFEA8)


// --- UARTS ---------------------------------------------------------------------------------------

SFR(S2BUF,0x9B);
SFR(S2CON,0x9A);
#define S2SM0   0x80    // S2CON.7
#define S2SM2   0x20    // S2CON.5
#define S2REN   0x10    // S2CON.4
#define S2TB8   0x08    // S2CON.3
#define S2RB8   0x04    // S2CON.2
#define S2TI    0x02    // S2CON.1
#define S2RI    0x01    // S2CON.0

SFR(S3BUF,0xAD);
SFR(S3CON,0xAC);
#define S3SM0   0x80    // S3CON.7
#define S3ST3   0x40    // S3CON.6
#define S3SM2   0x20    // S3CON.5
#define S3REN   0x10    // S3CON.4
#define S3TB8   0x08    // S3CON.3
#define S3RB8   0x04    // S3CON.2
#define S3TI    0x02    // S3CON.1
#define S3RI    0x01    // S3CON.0

SFR(S4BUF,0x85);
SFR(S4CON,0x84);
#define S4SM0   0x80    // S4CON.7
#define S4ST3   0x40    // S4CON.6
#define S4SM2   0x20    // S4CON.5
#define S4REN   0x10    // S4CON.4
#define S4TB8   0x08    // S4CON.3
#define S4RB8   0x04    // S4CON.2
#define S4TI    0x02    // S4CON.1
#define S4RI    0x01    // S4CON.0

SFR(SADDR,0xA9);
SFR(SADEN,0xB9);

// --- SPI ---------------------------------------------------------------------------------------

SFR(SPSTAT,0xCD);

#define SPIF    0x80    // SPSTAT.7
#define WCOL    0x40    // SPSTAT.6

SFR(SPCTL,0xCE);

#define SSIG    0x80    // SPCTL.7
#define SPEN    0x40    // SPCTL.6
#define DORD    0x20    // SPCTL.5
#define MSTR    0x10    // SPCTL.4
#define CPOL    0x08    // SPCTL.3
#define CPHA    0x04    // SPCTL.2
#define SPR1    0x02    // SPCTL.1
#define SPR0    0x01    // SPCTL.0

SFR(SPDAT,0xCF);

// --- PCA ---------------------------------------------------------------------------------------

SFR(CCON,0xD8);
SBIT(CF,0xD8,7);
SBIT(CR,0xD8,6);
SBIT(CCF3,0xD8,3);
SBIT(CCF2,0xD8,2);
SBIT(CCF1,0xD8,1);
SBIT(CCF0,0xD8,0);

SFR(CMOD,0xD9);
#define CIDL    0x80    // CMOD.7
#define CPS2    0x08    // CMOD.3
#define CPS1    0x04    // CMOD.2
#define CPS0    0x02    // CMOD.1
#define EXF     0x01    // CMOD.0

SFR(CCAPM0,0xDA);
SFR(CCAPM1,0xDB);
SFR(CCAPM2,0xDC);
SFR(CL    ,0xE9);  
SFR(CH    ,0xF9);

SFR(CCAP0L,0xEA);  
SFR(CCAP1L,0xEB);  
SFR(CCAP2L,0xEC);  
SFR(CCAP0H,0xFA);
SFR(CCAP1H,0xFB);
SFR(CCAP2H,0xFC);

SFR(PCA_PWM0,0xF2); 
SFR(PCA_PWM1,0xF3);
SFR(PCA_PWM2,0xF4);

#define CCAPM3   (*(unsigned char volatile __xdata *)0xFD54)
#define CCAP3L   (*(unsigned char volatile __xdata *)0xFD55)
#define CCAP3H   (*(unsigned char volatile __xdata *)0xFD56)

SFR16E(CCAP0,0xFAEA);
SFR16E(CCAP1,0xFBEB);
SFR16E(CCAP2,0xFCEC);


// --- IAP/EEPROM ---------------------------------------------------------------------------------

SFR(IAP_DATA ,0xC2);
SFR(IAP_ADDRH,0xC3);
SFR(IAP_ADDRL,0xC4);
SFR(IAP_CMD  ,0xC5);
#define CMD_NONE    0x00    // IAP_CMD[1:0]
#define CMD_READ    0x01    // IAP_CMD[1:0]
#define CMD_WRITE   0x02    // IAP_CMD[1:0]
#define CMD_ERASE   0x03    // IAP_CMD[1:0]

SFR(IAP_TRIG ,0xC6);
SFR(IAP_CONTR,0xC7);
#define IAPEN       0x80    // IAP_CONTR.7
#define SWBS        0x40    // IAP_CONTR.6
#define SWRST       0x20    // IAP_CONTR.5
#define CMD_FAIL    0x10    // IAP_CONTR.4

SFR(IAP_TPS,  0xF5);

SFR16E(IAP_ADDR,0xC3C4);

// Same with another names
SFR(ISP_DATA ,0xC2);
SFR(ISP_ADDRH,0xC3);
SFR(ISP_ADDRL,0xC4);
SFR(ISP_CMD  ,0xC5);
SFR(ISP_TRIG ,0xC6);
SFR(ISP_CONTR,0xC7);
SFR(ISP_TPS,  0xF5);
SFR16E(ISP_ADDR,0xC3C4);

// --- TODO ---------------------------------------------------------------------------------------

SFR16E(TMR0,0x8C8A);
__sfr16 __at (0x8D8B) TMR1;
__sfr16 __at (0xD6D7) TMR2;

SFR(WAKE_CLKO,0x8F);
SFR(BUS_SPEED,0xA1);
SFR(TA   ,0xAE);
SFR(VOCTRL   ,0xBB);

SFR(ADC_CONTR,0xBC);
SFR(ADC_RES  ,0xBD);
SFR(ADC_RESL ,0xBE);

SFR(WDT_CONTR,0xC1);

SFR(ADCCFG,0xDE); 

SFR(DPS   ,0xE3);
SFR(DPL1  ,0xE4);
SFR(DPH1  ,0xE5);
SFR(CMPCR1,0xE6);
SFR(CMPCR2,0xE7);

SFR(PWMSET  ,0xF1); 

SFR(PWMCFG01,0xF6);
SFR(PWMCFG23,0xF7);
SFR(USBCON ,0xF4);


SFR(PWMCFG45,0xFE);
SFR(USBADR,0xFC);
SFR(RSTCFG,0xFF);

#define EN_XSFR()        P_SW2 |=  0x80
#define DIS_XSFR()       P_SW2 &= ~0x80

// --- DMA ---------------------------------------------------------------------------------------

// - DMA: M2M
#define DMA_M2M_CR     (*(unsigned char volatile __xdata *)0xFA01)
#define DMA_M2M_STA    (*(unsigned char volatile __xdata *)0xFA02)
#define DMA_M2M_AMT    (*(unsigned char volatile __xdata *)0xFA03)
#define DMA_M2M_DONE   (*(unsigned char volatile __xdata *)0xFA04)
#define DMA_M2M_TXAH   (*(unsigned char volatile __xdata *)0xFA05)
#define DMA_M2M_TXAL   (*(unsigned char volatile __xdata *)0xFA06)
#define DMA_M2M_RXAH   (*(unsigned char volatile __xdata *)0xFA07)
#define DMA_M2M_RXAL   (*(unsigned char volatile __xdata *)0xFA08)

// - DMA: ADC
#define DMA_ADC_CFG    (*(unsigned char volatile __xdata *)0xFA10)
#define DMA_ADC_CR     (*(unsigned char volatile __xdata *)0xFA11)
#define DMA_ADC_STA    (*(unsigned char volatile __xdata *)0xFA12)
#define DMA_ADC_RXAH   (*(unsigned char volatile __xdata *)0xFA17)
#define DMA_ADC_RXAL   (*(unsigned char volatile __xdata *)0xFA18)
#define DMA_ADC_CFG2   (*(unsigned char volatile __xdata *)0xFA19)
#define DMA_ADC_CHSW0  (*(unsigned char volatile __xdata *)0xFA1A)
#define DMA_ADC_CHSW1  (*(unsigned char volatile __xdata *)0xFA1B)

// - DMA: SPI
#define DMA_SPI_CFG    (*(unsigned char volatile __xdata *)0xFA20)
#define SPIIE   0x80
#define ACT_TX  0x40
#define ACT_RX  0x20
#define SPIIP1  0x08
#define SPIIP0  0x04
#define SPIPTY1 0x02
#define SPIPTY0 0x01

#define DMA_SPI_CR     (*(unsigned char volatile __xdata *)0xFA21)
#define ENSPI   0x80
#define TRIG_M  0x40
#define TRIG_S  0x20
#define CLRFIFO 0x01

#define DMA_SPI_STA    (*(unsigned char volatile __xdata *)0xFA22)
#define TXOVW   0x04
#define RXLOSS  0x02
#define SPIIF   0x01

#define DMA_SPI_AMT    (*(unsigned char volatile __xdata *)0xFA23)
#define DMA_SPI_DONE   (*(unsigned char volatile __xdata *)0xFA24)
#define DMA_SPI_TXAH   (*(unsigned char volatile __xdata *)0xFA25)
#define DMA_SPI_TXAL   (*(unsigned char volatile __xdata *)0xFA26)
#define DMA_SPI_RXAH   (*(unsigned char volatile __xdata *)0xFA27)
#define DMA_SPI_RXAL   (*(unsigned char volatile __xdata *)0xFA28)

#define DMA_SPI_CFG2   (*(unsigned char volatile __xdata *)0xFA29)
#define WRPSS   0x04
#define SSS1    0x02
#define SSS0    0x01

// - DMA: UARTs
#define DMA_UR1T_CFG   (*(unsigned char volatile __xdata *)0xFA30)
#define DMA_UR1T_CR    (*(unsigned char volatile __xdata *)0xFA31)
#define DMA_UR1T_STA   (*(unsigned char volatile __xdata *)0xFA32)
#define DMA_UR1T_AMT   (*(unsigned char volatile __xdata *)0xFA33)
#define DMA_UR1T_DONE  (*(unsigned char volatile __xdata *)0xFA34)
#define DMA_UR1T_TXAH  (*(unsigned char volatile __xdata *)0xFA35)
#define DMA_UR1T_TXAL  (*(unsigned char volatile __xdata *)0xFA36)
#define DMA_UR1R_CFG   (*(unsigned char volatile __xdata *)0xFA38)
#define DMA_UR1R_CR    (*(unsigned char volatile __xdata *)0xFA39)
#define DMA_UR1R_STA   (*(unsigned char volatile __xdata *)0xFA3A)
#define DMA_UR1R_AMT   (*(unsigned char volatile __xdata *)0xFA3B)
#define DMA_UR1R_DONE  (*(unsigned char volatile __xdata *)0xFA3C)
#define DMA_UR1R_TXAH  (*(unsigned char volatile __xdata *)0xFA3D)
#define DMA_UR1R_TXAL  (*(unsigned char volatile __xdata *)0xFA3E)
#define DMA_UR2T_CFG   (*(unsigned char volatile __xdata *)0xFA40)
#define DMA_UR2T_CR    (*(unsigned char volatile __xdata *)0xFA41)
#define DMA_UR2T_STA   (*(unsigned char volatile __xdata *)0xFA42)
#define DMA_UR2T_AMT   (*(unsigned char volatile __xdata *)0xFA43)
#define DMA_UR2T_DONE  (*(unsigned char volatile __xdata *)0xFA44)
#define DMA_UR2T_TXAH  (*(unsigned char volatile __xdata *)0xFA45)
#define DMA_UR2T_TXAL  (*(unsigned char volatile __xdata *)0xFA46)
#define DMA_UR2R_CFG   (*(unsigned char volatile __xdata *)0xFA48)
#define DMA_UR2R_CR    (*(unsigned char volatile __xdata *)0xFA49)
#define DMA_UR2R_STA   (*(unsigned char volatile __xdata *)0xFA4A)
#define DMA_UR2R_AMT   (*(unsigned char volatile __xdata *)0xFA4B)
#define DMA_UR2R_DONE  (*(unsigned char volatile __xdata *)0xFA4C)
#define DMA_UR2R_TXAH  (*(unsigned char volatile __xdata *)0xFA4D)
#define DMA_UR2R_TXAL  (*(unsigned char volatile __xdata *)0xFA4E)
#define DMA_UR3T_CFG   (*(unsigned char volatile __xdata *)0xFA50)
#define DMA_UR3T_CR    (*(unsigned char volatile __xdata *)0xFA51)
#define DMA_UR3T_STA   (*(unsigned char volatile __xdata *)0xFA52)
#define DMA_UR3T_AMT   (*(unsigned char volatile __xdata *)0xFA53)
#define DMA_UR3T_DONE  (*(unsigned char volatile __xdata *)0xFA54)
#define DMA_UR3T_TXAH  (*(unsigned char volatile __xdata *)0xFA55)
#define DMA_UR3T_TXAL  (*(unsigned char volatile __xdata *)0xFA56)
#define DMA_UR3R_CFG   (*(unsigned char volatile __xdata *)0xFA58)
#define DMA_UR3R_CR    (*(unsigned char volatile __xdata *)0xFA59)
#define DMA_UR3R_STA   (*(unsigned char volatile __xdata *)0xFA5A)
#define DMA_UR3R_AMT   (*(unsigned char volatile __xdata *)0xFA5B)
#define DMA_UR3R_DONE  (*(unsigned char volatile __xdata *)0xFA5C)
#define DMA_UR3R_TXAH  (*(unsigned char volatile __xdata *)0xFA5D)
#define DMA_UR3R_TXAL  (*(unsigned char volatile __xdata *)0xFA5E)
#define DMA_UR4T_CFG   (*(unsigned char volatile __xdata *)0xFA60)
#define DMA_UR4T_CR    (*(unsigned char volatile __xdata *)0xFA61)
#define DMA_UR4T_STA   (*(unsigned char volatile __xdata *)0xFA62)
#define DMA_UR4T_AMT   (*(unsigned char volatile __xdata *)0xFA63)
#define DMA_UR4T_DONE  (*(unsigned char volatile __xdata *)0xFA64)
#define DMA_UR4T_TXAH  (*(unsigned char volatile __xdata *)0xFA65)
#define DMA_UR4T_TXAL  (*(unsigned char volatile __xdata *)0xFA66)
#define DMA_UR4R_CFG   (*(unsigned char volatile __xdata *)0xFA68)
#define DMA_UR4R_CR    (*(unsigned char volatile __xdata *)0xFA69)
#define DMA_UR4R_STA   (*(unsigned char volatile __xdata *)0xFA6A)
#define DMA_UR4R_AMT   (*(unsigned char volatile __xdata *)0xFA6B)
#define DMA_UR4R_DONE  (*(unsigned char volatile __xdata *)0xFA6C)
#define DMA_UR4R_TXAH  (*(unsigned char volatile __xdata *)0xFA6D)
#define DMA_UR4R_TXAL  (*(unsigned char volatile __xdata *)0xFA6E)

// - DMA: LCM
#define DMA_LCM_CFG    (*(unsigned char volatile __xdata *)0xFA70)
#define DMA_LCM_CR     (*(unsigned char volatile __xdata *)0xFA71)
#define DMA_LCM_STA    (*(unsigned char volatile __xdata *)0xFA72)
#define DMA_LCM_AMT    (*(unsigned char volatile __xdata *)0xFA73)
#define DMA_LCM_DONE   (*(unsigned char volatile __xdata *)0xFA74)
#define DMA_LCM_TXAH   (*(unsigned char volatile __xdata *)0xFA75)
#define DMA_LCM_TXAL   (*(unsigned char volatile __xdata *)0xFA76)
#define DMA_LCM_RXAH   (*(unsigned char volatile __xdata *)0xFA77)
#define DMA_LCM_RXAL   (*(unsigned char volatile __xdata *)0xFA78)



#define I2C_USE_P14P15()    P_SW2 &= ~0x30                  
#define I2C_USE_P24P25()    P_SW2  = (P_SW2 & ~0x30) | 0x10 
#define I2C_USE_P76P77()    P_SW2  = (P_SW2 & ~0x30) | 0x20 
#define I2C_USE_P33P32()    P_SW2 |= 0x30                   

#define CLKSEL      (*(unsigned char volatile __xdata *)0xfe00)
#define CKSEL       (*(unsigned char volatile __xdata *)0xfe00)
#define CLKDIV      (*(unsigned char volatile __xdata *)0xfe01)
#define IRC24MCR    (*(unsigned char volatile __xdata *)0xfe02)
#define XOSCCR      (*(unsigned char volatile __xdata *)0xfe03)
#define IRC32KCR    (*(unsigned char volatile __xdata *)0xfe04)
#define MCLKOCR     (*(unsigned char volatile __xdata *)0xfe05)
#define IRCDB       (*(unsigned char volatile __xdata *)0xfe06)
#define X32KCR      (*(unsigned char volatile __xdata *)0xfe08)

                  
#define I2CCFG      (*(unsigned char volatile __xdata *)0xfe80)
#define I2CMSCR     (*(unsigned char volatile __xdata *)0xfe81)
#define I2CMSST     (*(unsigned char volatile __xdata *)0xfe82)
#define I2CSLCR     (*(unsigned char volatile __xdata *)0xfe83)
#define I2CSLST     (*(unsigned char volatile __xdata *)0xfe84)
#define I2CSLADR    (*(unsigned char volatile __xdata *)0xfe85)
#define I2CTXD      (*(unsigned char volatile __xdata *)0xfe86)
#define I2CRXD      (*(unsigned char volatile __xdata *)0xfe87)
#define I2CMSAUX    (*(unsigned char volatile __xdata *)0xfe88)


#define PWMA_ETRPS  (*(unsigned char volatile __xdata *)0xfeb0)
#define PWMA_ENO    (*(unsigned char volatile __xdata *)0xfeb1)
#define PWMA_PS     (*(unsigned char volatile __xdata *)0xfeb2)
#define PWMA_IOAUX  (*(unsigned char volatile __xdata *)0xfeb3)
#define PWMB_ETRPS  (*(unsigned char volatile __xdata *)0xfeb4)
#define PWMB_ENO    (*(unsigned char volatile __xdata *)0xfeb5)
#define PWMB_PS     (*(unsigned char volatile __xdata *)0xfeb6)
#define PWMB_IOAUX  (*(unsigned char volatile __xdata *)0xfeb7)
#define PWMA_CR1    (*(unsigned char volatile __xdata *)0xfec0)
#define PWMA_CR2    (*(unsigned char volatile __xdata *)0xfec1)
#define PWMA_SMCR   (*(unsigned char volatile __xdata *)0xfec2)
#define PWMA_ETR    (*(unsigned char volatile __xdata *)0xfec3)
#define PWMA_IER    (*(unsigned char volatile __xdata *)0xfec4)
#define PWMA_SR1    (*(unsigned char volatile __xdata *)0xfec5)
#define PWMA_SR2    (*(unsigned char volatile __xdata *)0xfec6)
#define PWMA_EGR    (*(unsigned char volatile __xdata *)0xfec7)
#define PWMA_CCMR1  (*(unsigned char volatile __xdata *)0xfec8)
#define PWMA_CCMR2  (*(unsigned char volatile __xdata *)0xfec9)
#define PWMA_CCMR3  (*(unsigned char volatile __xdata *)0xfeca)
#define PWMA_CCMR4  (*(unsigned char volatile __xdata *)0xfecb)
#define PWMA_CCER1  (*(unsigned char volatile __xdata *)0xfecc)
#define PWMA_CCER2  (*(unsigned char volatile __xdata *)0xfecd)
#define PWMA_CNTR   (*(unsigned int  volatile __xdata *)0xfece)
#define PWMA_CNTRH  (*(unsigned char volatile __xdata *)0xfece)
#define PWMA_CNTRL  (*(unsigned char volatile __xdata *)0xfecf)
#define PWMA_PSCR   (*(unsigned int  volatile __xdata *)0xfed0)
#define PWMA_PSCRH  (*(unsigned char volatile __xdata *)0xfed0)
#define PWMA_PSCRL  (*(unsigned char volatile __xdata *)0xfed1)
#define PWMA_ARR    (*(unsigned int  volatile __xdata *)0xfed2)
#define PWMA_ARRH   (*(unsigned char volatile __xdata *)0xfed2)
#define PWMA_ARRL   (*(unsigned char volatile __xdata *)0xfed3)
#define PWMA_RCR    (*(unsigned char volatile __xdata *)0xfed4)
#define PWMA_CCR1   (*(unsigned int  volatile __xdata *)0xfed5)
#define PWMA_CCR1H  (*(unsigned char volatile __xdata *)0xfed5)
#define PWMA_CCR1L  (*(unsigned char volatile __xdata *)0xfed6)
#define PWMA_CCR2   (*(unsigned int  volatile __xdata *)0xfed7)
#define PWMA_CCR2H  (*(unsigned char volatile __xdata *)0xfed7)
#define PWMA_CCR2L  (*(unsigned char volatile __xdata *)0xfed8)
#define PWMA_CCR3   (*(unsigned int  volatile __xdata *)0xfed9)
#define PWMA_CCR3H  (*(unsigned char volatile __xdata *)0xfed9)
#define PWMA_CCR3L  (*(unsigned char volatile __xdata *)0xfeda)
#define PWMA_CCR4   (*(unsigned int  volatile __xdata *)0xfedb)
#define PWMA_CCR4H  (*(unsigned char volatile __xdata *)0xfedb)
#define PWMA_CCR4L  (*(unsigned char volatile __xdata *)0xfedc)
#define PWMA_BRK    (*(unsigned char volatile __xdata *)0xfedd)
#define PWMA_DTR    (*(unsigned char volatile __xdata *)0xfede)
#define PWMA_OISR   (*(unsigned char volatile __xdata *)0xfedf)
                    
#define PWMB_CR1    (*(unsigned char volatile __xdata *)0xfee0)
#define PWMB_CR2    (*(unsigned char volatile __xdata *)0xfee1)
#define PWMB_SMCR   (*(unsigned char volatile __xdata *)0xfee2)
#define PWMB_ETR    (*(unsigned char volatile __xdata *)0xfee3)
#define PWMB_IER    (*(unsigned char volatile __xdata *)0xfee4)
#define PWMB_SR1    (*(unsigned char volatile __xdata *)0xfee5)
#define PWMB_SR2    (*(unsigned char volatile __xdata *)0xfee6)
#define PWMB_EGR    (*(unsigned char volatile __xdata *)0xfee7)
#define PWMB_CCMR1  (*(unsigned char volatile __xdata *)0xfee8)
#define PWMB_CCMR2  (*(unsigned char volatile __xdata *)0xfee9)
#define PWMB_CCMR3  (*(unsigned char volatile __xdata *)0xfeea)
#define PWMB_CCMR4  (*(unsigned char volatile __xdata *)0xfeeb)
#define PWMB_CCER1  (*(unsigned char volatile __xdata *)0xfeec)
#define PWMB_CCER2  (*(unsigned char volatile __xdata *)0xfeed)
#define PWMB_CNTR   (*(unsigned int  volatile __xdata *)0xfeee)
#define PWMB_CNTRH  (*(unsigned char volatile __xdata *)0xfeee)
#define PWMB_CNTRL  (*(unsigned char volatile __xdata *)0xfeef)
#define PWMB_PSCR   (*(unsigned int  volatile __xdata *)0xfef0)
#define PWMB_PSCRH  (*(unsigned char volatile __xdata *)0xfef0)
#define PWMB_PSCRL  (*(unsigned char volatile __xdata *)0xfef1)
#define PWMB_ARR    (*(unsigned int  volatile __xdata *)0xfef2)
#define PWMB_ARRH   (*(unsigned char volatile __xdata *)0xfef2)
#define PWMB_ARRL   (*(unsigned char volatile __xdata *)0xfef3)
#define PWMB_RCR    (*(unsigned char volatile __xdata *)0xfef4)
#define PWMB_CCR5   (*(unsigned int  volatile __xdata *)0xfef5)
#define PWMB_CCR5H  (*(unsigned char volatile __xdata *)0xfef5)
#define PWMB_CCR5L  (*(unsigned char volatile __xdata *)0xfef6)
#define PWMB_CCR6   (*(unsigned int  volatile __xdata *)0xfef7)
#define PWMB_CCR6H  (*(unsigned char volatile __xdata *)0xfef7)
#define PWMB_CCR6L  (*(unsigned char volatile __xdata *)0xfef8)
#define PWMB_CCR7   (*(unsigned int  volatile __xdata *)0xfef9)
#define PWMB_CCR7H  (*(unsigned char volatile __xdata *)0xfef9)
#define PWMB_CCR7L  (*(unsigned char volatile __xdata *)0xfefa)
#define PWMB_CCR8   (*(unsigned int  volatile __xdata *)0xfefb)
#define PWMB_CCR8H  (*(unsigned char volatile __xdata *)0xfefb)
#define PWMB_CCR8L  (*(unsigned char volatile __xdata *)0xfefc)
#define PWMB_BRK    (*(unsigned char volatile __xdata *)0xfefd)
#define PWMB_DTR    (*(unsigned char volatile __xdata *)0xfefe)
#define PWMB_OISR   (*(unsigned char volatile __xdata *)0xfeff)

/////////////////////////////////////////////////
//FD00H-FDFFH
/////////////////////////////////////////////////

#define     PWM0C       (*(unsigned int volatile __xdata *)0xff00)
#define     PWM0CH      (*(unsigned char volatile __xdata *)0xff00)
#define     PWM0CL      (*(unsigned char volatile __xdata *)0xff01)
#define     PWM0CKS     (*(unsigned char volatile __xdata *)0xff02)
#define     PWM0TADC    (*(unsigned int volatile __xdata *)0xff03)
#define     PWM0TADCH   (*(unsigned char volatile __xdata *)0xff03)
#define     PWM0TADCL   (*(unsigned char volatile __xdata *)0xff04)
#define     PWM0IF      (*(unsigned char volatile __xdata *)0xff05)
#define     PWM0FDCR    (*(unsigned char volatile __xdata *)0xff06)
#define     PWM00T1     (*(unsigned int volatile __xdata *)0xff10)
#define     PWM00T1H    (*(unsigned char volatile __xdata *)0xff10)
#define     PWM00T1L    (*(unsigned char volatile __xdata *)0xff11)
#define     PWM00T2     (*(unsigned int volatile __xdata *)0xff12)
#define     PWM00T2H    (*(unsigned char volatile __xdata *)0xff12)
#define     PWM00T2L    (*(unsigned char volatile __xdata *)0xff13)
#define     PWM00CR     (*(unsigned char volatile __xdata *)0xff14)
#define     PWM00HLD    (*(unsigned char volatile __xdata *)0xff15)
#define     PWM01T1     (*(unsigned int volatile __xdata *)0xff18)
#define     PWM01T1H    (*(unsigned char volatile __xdata *)0xff18)
#define     PWM01T1L    (*(unsigned char volatile __xdata *)0xff19)
#define     PWM01T2     (*(unsigned int volatile __xdata *)0xff1a)
#define     PWM01T2H    (*(unsigned char volatile __xdata *)0xff1a)
#define     PWM01T2L    (*(unsigned char volatile __xdata *)0xff1b)
#define     PWM01CR     (*(unsigned char volatile __xdata *)0xff1c)
#define     PWM01HLD    (*(unsigned char volatile __xdata *)0xff1d)
#define     PWM02T1     (*(unsigned int volatile __xdata *)0xff20)
#define     PWM02T1H    (*(unsigned char volatile __xdata *)0xff20)
#define     PWM02T1L    (*(unsigned char volatile __xdata *)0xff21)
#define     PWM02T2     (*(unsigned int volatile __xdata *)0xff22)
#define     PWM02T2H    (*(unsigned char volatile __xdata *)0xff22)
#define     PWM02T2L    (*(unsigned char volatile __xdata *)0xff23)
#define     PWM02CR     (*(unsigned char volatile __xdata *)0xff24)
#define     PWM02HLD    (*(unsigned char volatile __xdata *)0xff25)
#define     PWM03T1     (*(unsigned int volatile __xdata *)0xff28)
#define     PWM03T1H    (*(unsigned char volatile __xdata *)0xff28)
#define     PWM03T1L    (*(unsigned char volatile __xdata *)0xff29)
#define     PWM03T2     (*(unsigned int volatile __xdata *)0xff2a)
#define     PWM03T2H    (*(unsigned char volatile __xdata *)0xff2a)
#define     PWM03T2L    (*(unsigned char volatile __xdata *)0xff2b)
#define     PWM03CR     (*(unsigned char volatile __xdata *)0xff2c)
#define     PWM03HLD    (*(unsigned char volatile __xdata *)0xff2d)
#define     PWM04T1     (*(unsigned int volatile __xdata *)0xff30)
#define     PWM04T1H    (*(unsigned char volatile __xdata *)0xff30)
#define     PWM04T1L    (*(unsigned char volatile __xdata *)0xff31)
#define     PWM04T2     (*(unsigned int volatile __xdata *)0xff32)
#define     PWM04T2H    (*(unsigned char volatile __xdata *)0xff32)
#define     PWM04T2L    (*(unsigned char volatile __xdata *)0xff33)
#define     PWM04CR     (*(unsigned char volatile __xdata *)0xff34)
#define     PWM04HLD    (*(unsigned char volatile __xdata *)0xff35)
#define     PWM05T1     (*(unsigned int volatile __xdata *)0xff38)
#define     PWM05T1H    (*(unsigned char volatile __xdata *)0xff38)
#define     PWM05T1L    (*(unsigned char volatile __xdata *)0xff39)
#define     PWM05T2     (*(unsigned int volatile __xdata *)0xff3a)
#define     PWM05T2H    (*(unsigned char volatile __xdata *)0xff3a)
#define     PWM05T2L    (*(unsigned char volatile __xdata *)0xff3b)
#define     PWM05CR     (*(unsigned char volatile __xdata *)0xff3c)
#define     PWM05HLD    (*(unsigned char volatile __xdata *)0xff3d)
#define     PWM06T1     (*(unsigned int volatile __xdata *)0xff40)
#define     PWM06T1H    (*(unsigned char volatile __xdata *)0xff40)
#define     PWM06T1L    (*(unsigned char volatile __xdata *)0xff41)
#define     PWM06T2     (*(unsigned int volatile __xdata *)0xff42)
#define     PWM06T2H    (*(unsigned char volatile __xdata *)0xff42)
#define     PWM06T2L    (*(unsigned char volatile __xdata *)0xff43)
#define     PWM06CR     (*(unsigned char volatile __xdata *)0xff44)
#define     PWM06HLD    (*(unsigned char volatile __xdata *)0xff45)
#define     PWM07T1     (*(unsigned int volatile __xdata *)0xff48)
#define     PWM07T1H    (*(unsigned char volatile __xdata *)0xff48)
#define     PWM07T1L    (*(unsigned char volatile __xdata *)0xff49)
#define     PWM07T2     (*(unsigned int volatile __xdata *)0xff4a)
#define     PWM07T2H    (*(unsigned char volatile __xdata *)0xff4a)
#define     PWM07T2L    (*(unsigned char volatile __xdata *)0xff4b)
#define     PWM07CR     (*(unsigned char volatile __xdata *)0xff4c)
#define     PWM07HLD    (*(unsigned char volatile __xdata *)0xff4d)
#define     PWM1C       (*(unsigned int volatile __xdata *)0xff50)
#define     PWM1CH      (*(unsigned char volatile __xdata *)0xff50)
#define     PWM1CL      (*(unsigned char volatile __xdata *)0xff51)
#define     PWM1CKS     (*(unsigned char volatile __xdata *)0xff52)
#define     PWM1IF      (*(unsigned char volatile __xdata *)0xff55)
#define     PWM1FDCR    (*(unsigned char volatile __xdata *)0xff56)
#define     PWM10T1     (*(unsigned int volatile __xdata *)0xff60)
#define     PWM10T1H    (*(unsigned char volatile __xdata *)0xff60)
#define     PWM10T1L    (*(unsigned char volatile __xdata *)0xff61)
#define     PWM10T2     (*(unsigned int volatile __xdata *)0xff62)
#define     PWM10T2H    (*(unsigned char volatile __xdata *)0xff62)
#define     PWM10T2L    (*(unsigned char volatile __xdata *)0xff63)
#define     PWM10CR     (*(unsigned char volatile __xdata *)0xff64)
#define     PWM10HLD    (*(unsigned char volatile __xdata *)0xff65)
#define     PWM11T1     (*(unsigned int volatile __xdata *)0xff68)
#define     PWM11T1H    (*(unsigned char volatile __xdata *)0xff68)
#define     PWM11T1L    (*(unsigned char volatile __xdata *)0xff69)
#define     PWM11T2     (*(unsigned int volatile __xdata *)0xff6a)
#define     PWM11T2H    (*(unsigned char volatile __xdata *)0xff6a)
#define     PWM11T2L    (*(unsigned char volatile __xdata *)0xff6b)
#define     PWM11CR     (*(unsigned char volatile __xdata *)0xff6c)
#define     PWM11HLD    (*(unsigned char volatile __xdata *)0xff6d)
#define     PWM12T1     (*(unsigned int volatile __xdata *)0xff70)
#define     PWM12T1H    (*(unsigned char volatile __xdata *)0xff70)
#define     PWM12T1L    (*(unsigned char volatile __xdata *)0xff71)
#define     PWM12T2     (*(unsigned int volatile __xdata *)0xff72)
#define     PWM12T2H    (*(unsigned char volatile __xdata *)0xff72)
#define     PWM12T2L    (*(unsigned char volatile __xdata *)0xff73)
#define     PWM12CR     (*(unsigned char volatile __xdata *)0xff74)
#define     PWM12HLD    (*(unsigned char volatile __xdata *)0xff75)
#define     PWM13T1     (*(unsigned int volatile __xdata *)0xff78)
#define     PWM13T1H    (*(unsigned char volatile __xdata *)0xff78)
#define     PWM13T1L    (*(unsigned char volatile __xdata *)0xff79)
#define     PWM13T2     (*(unsigned int volatile __xdata *)0xff7a)
#define     PWM13T2H    (*(unsigned char volatile __xdata *)0xff7a)
#define     PWM13T2L    (*(unsigned char volatile __xdata *)0xff7b)
#define     PWM13CR     (*(unsigned char volatile __xdata *)0xff7c)
#define     PWM13HLD    (*(unsigned char volatile __xdata *)0xff7d)
#define     PWM14T1     (*(unsigned int volatile __xdata *)0xff80)
#define     PWM14T1H    (*(unsigned char volatile __xdata *)0xff80)
#define     PWM14T1L    (*(unsigned char volatile __xdata *)0xff81)
#define     PWM14T2     (*(unsigned int volatile __xdata *)0xff82)
#define     PWM14T2H    (*(unsigned char volatile __xdata *)0xff82)
#define     PWM14T2L    (*(unsigned char volatile __xdata *)0xff83)
#define     PWM14CR     (*(unsigned char volatile __xdata *)0xff84)
#define     PWM14HLD    (*(unsigned char volatile __xdata *)0xff85)
#define     PWM15T1     (*(unsigned int volatile __xdata *)0xff88)
#define     PWM15T1H    (*(unsigned char volatile __xdata *)0xff88)
#define     PWM15T1L    (*(unsigned char volatile __xdata *)0xff89)
#define     PWM15T2     (*(unsigned int volatile __xdata *)0xff8a)
#define     PWM15T2H    (*(unsigned char volatile __xdata *)0xff8a)
#define     PWM15T2L    (*(unsigned char volatile __xdata *)0xff8b)
#define     PWM15CR     (*(unsigned char volatile __xdata *)0xff8c)
#define     PWM15HLD    (*(unsigned char volatile __xdata *)0xff8d)
#define     PWM16T1     (*(unsigned int volatile __xdata *)0xff90)
#define     PWM16T1H    (*(unsigned char volatile __xdata *)0xff90)
#define     PWM16T1L    (*(unsigned char volatile __xdata *)0xff91)
#define     PWM16T2     (*(unsigned int volatile __xdata *)0xff92)
#define     PWM16T2H    (*(unsigned char volatile __xdata *)0xff92)
#define     PWM16T2L    (*(unsigned char volatile __xdata *)0xff93)
#define     PWM16CR     (*(unsigned char volatile __xdata *)0xff94)
#define     PWM16HLD    (*(unsigned char volatile __xdata *)0xff95)
#define     PWM17T1     (*(unsigned int volatile __xdata *)0xff98)
#define     PWM17T1H    (*(unsigned char volatile __xdata *)0xff98)
#define     PWM17T1L    (*(unsigned char volatile __xdata *)0xff99)
#define     PWM17T2     (*(unsigned int volatile __xdata *)0xff9a)
#define     PWM17T2H    (*(unsigned char volatile __xdata *)0xff9a)
#define     PWM17T2L    (*(unsigned char volatile __xdata *)0xff9b)
#define     PWM17CR     (*(unsigned char volatile __xdata *)0xff9c)
#define     PWM17HLD    (*(unsigned char volatile __xdata *)0xff9d)
#define     PWM2C       (*(unsigned int volatile __xdata *)0xffa0)
#define     PWM2CH      (*(unsigned char volatile __xdata *)0xffa0)
#define     PWM2CL      (*(unsigned char volatile __xdata *)0xffa1)
#define     PWM2CKS     (*(unsigned char volatile __xdata *)0xffa2)
#define     PWM2TADC    (*(unsigned int volatile __xdata *)0xffa3)
#define     PWM2TADCH   (*(unsigned char volatile __xdata *)0xffa3)
#define     PWM2TADCL   (*(unsigned char volatile __xdata *)0xffa4)
#define     PWM2IF      (*(unsigned char volatile __xdata *)0xffa5)
#define     PWM2FDCR    (*(unsigned char volatile __xdata *)0xffa6)
#define     PWM20T1     (*(unsigned int volatile __xdata *)0xffb0)
#define     PWM20T1H    (*(unsigned char volatile __xdata *)0xffb0)
#define     PWM20T1L    (*(unsigned char volatile __xdata *)0xffb1)
#define     PWM20T2     (*(unsigned int volatile __xdata *)0xffb2)
#define     PWM20T2H    (*(unsigned char volatile __xdata *)0xffb2)
#define     PWM20T2L    (*(unsigned char volatile __xdata *)0xffb3)
#define     PWM20CR     (*(unsigned char volatile __xdata *)0xffb4)
#define     PWM20HLD    (*(unsigned char volatile __xdata *)0xffb5)
#define     PWM21T1     (*(unsigned int volatile __xdata *)0xffb8)
#define     PWM21T1H    (*(unsigned char volatile __xdata *)0xffb8)
#define     PWM21T1L    (*(unsigned char volatile __xdata *)0xffb9)
#define     PWM21T2     (*(unsigned int volatile __xdata *)0xffba)
#define     PWM21T2H    (*(unsigned char volatile __xdata *)0xffba)
#define     PWM21T2L    (*(unsigned char volatile __xdata *)0xffbb)
#define     PWM21CR     (*(unsigned char volatile __xdata *)0xffbc)
#define     PWM21HLD    (*(unsigned char volatile __xdata *)0xffbd)
#define     PWM22T1     (*(unsigned int volatile __xdata *)0xffc0)
#define     PWM22T1H    (*(unsigned char volatile __xdata *)0xffc0)
#define     PWM22T1L    (*(unsigned char volatile __xdata *)0xffc1)
#define     PWM22T2     (*(unsigned int volatile __xdata *)0xffc2)
#define     PWM22T2H    (*(unsigned char volatile __xdata *)0xffc2)
#define     PWM22T2L    (*(unsigned char volatile __xdata *)0xffc3)
#define     PWM22CR     (*(unsigned char volatile __xdata *)0xffc4)
#define     PWM22HLD    (*(unsigned char volatile __xdata *)0xffc5)
#define     PWM23T1     (*(unsigned int volatile __xdata *)0xffc8)
#define     PWM23T1H    (*(unsigned char volatile __xdata *)0xffc8)
#define     PWM23T1L    (*(unsigned char volatile __xdata *)0xffc9)
#define     PWM23T2     (*(unsigned int volatile __xdata *)0xffca)
#define     PWM23T2H    (*(unsigned char volatile __xdata *)0xffca)
#define     PWM23T2L    (*(unsigned char volatile __xdata *)0xffcb)
#define     PWM23CR     (*(unsigned char volatile __xdata *)0xffcc)
#define     PWM23HLD    (*(unsigned char volatile __xdata *)0xffcd)
#define     PWM24T1     (*(unsigned int volatile __xdata *)0xffd0)
#define     PWM24T1H    (*(unsigned char volatile __xdata *)0xffd0)
#define     PWM24T1L    (*(unsigned char volatile __xdata *)0xffd1)
#define     PWM24T2     (*(unsigned int volatile __xdata *)0xffd2)
#define     PWM24T2H    (*(unsigned char volatile __xdata *)0xffd2)
#define     PWM24T2L    (*(unsigned char volatile __xdata *)0xffd3)
#define     PWM24CR     (*(unsigned char volatile __xdata *)0xffd4)
#define     PWM24HLD    (*(unsigned char volatile __xdata *)0xffd5)
#define     PWM25T1     (*(unsigned int volatile __xdata *)0xffd8)
#define     PWM25T1H    (*(unsigned char volatile __xdata *)0xffd8)
#define     PWM25T1L    (*(unsigned char volatile __xdata *)0xffd9)
#define     PWM25T2     (*(unsigned int volatile __xdata *)0xffda)
#define     PWM25T2H    (*(unsigned char volatile __xdata *)0xffda)
#define     PWM25T2L    (*(unsigned char volatile __xdata *)0xffdb)
#define     PWM25CR     (*(unsigned char volatile __xdata *)0xffdc)
#define     PWM25HLD    (*(unsigned char volatile __xdata *)0xffdd)
#define     PWM26T1     (*(unsigned int volatile __xdata *)0xffe0)
#define     PWM26T1H    (*(unsigned char volatile __xdata *)0xffe0)
#define     PWM26T1L    (*(unsigned char volatile __xdata *)0xffe1)
#define     PWM26T2     (*(unsigned int volatile __xdata *)0xffe2)
#define     PWM26T2H    (*(unsigned char volatile __xdata *)0xffe2)
#define     PWM26T2L    (*(unsigned char volatile __xdata *)0xffe3)
#define     PWM26CR     (*(unsigned char volatile __xdata *)0xffe4)
#define     PWM26HLD    (*(unsigned char volatile __xdata *)0xffe5)
#define     PWM27T1     (*(unsigned int volatile __xdata *)0xffe8)
#define     PWM27T1H    (*(unsigned char volatile __xdata *)0xffe8)
#define     PWM27T1L    (*(unsigned char volatile __xdata *)0xffe9)
#define     PWM27T2     (*(unsigned int volatile __xdata *)0xffea)
#define     PWM27T2H    (*(unsigned char volatile __xdata *)0xffea)
#define     PWM27T2L    (*(unsigned char volatile __xdata *)0xffeb)
#define     PWM27CR     (*(unsigned char volatile __xdata *)0xffec)
#define     PWM27HLD    (*(unsigned char volatile __xdata *)0xffed)

/////////////////////////////////////////////////
//FC00H-FCFFH
/////////////////////////////////////////////////

#define     PWM3C       (*(unsigned int volatile __xdata *)0xfc00)
#define     PWM3CH      (*(unsigned char volatile __xdata *)0xfc00)
#define     PWM3CL      (*(unsigned char volatile __xdata *)0xfc01)
#define     PWM3CKS     (*(unsigned char volatile __xdata *)0xfc02)
#define     PWM3IF      (*(unsigned char volatile __xdata *)0xfc05)
#define     PWM3FDCR    (*(unsigned char volatile __xdata *)0xfc06)
#define     PWM30T1     (*(unsigned int volatile __xdata *)0xfc10)
#define     PWM30T1H    (*(unsigned char volatile __xdata *)0xfc10)
#define     PWM30T1L    (*(unsigned char volatile __xdata *)0xfc11)
#define     PWM30T2     (*(unsigned int volatile __xdata *)0xfc12)
#define     PWM30T2H    (*(unsigned char volatile __xdata *)0xfc12)
#define     PWM30T2L    (*(unsigned char volatile __xdata *)0xfc13)
#define     PWM30CR     (*(unsigned char volatile __xdata *)0xfc14)
#define     PWM30HLD    (*(unsigned char volatile __xdata *)0xfc15)
#define     PWM31T1     (*(unsigned int volatile __xdata *)0xfc18)
#define     PWM31T1H    (*(unsigned char volatile __xdata *)0xfc18)
#define     PWM31T1L    (*(unsigned char volatile __xdata *)0xfc19)
#define     PWM31T2     (*(unsigned int volatile __xdata *)0xfc1a)
#define     PWM31T2H    (*(unsigned char volatile __xdata *)0xfc1a)
#define     PWM31T2L    (*(unsigned char volatile __xdata *)0xfc1b)
#define     PWM31CR     (*(unsigned char volatile __xdata *)0xfc1c)
#define     PWM31HLD    (*(unsigned char volatile __xdata *)0xfc1d)
#define     PWM32T1     (*(unsigned int volatile __xdata *)0xfc20)
#define     PWM32T1H    (*(unsigned char volatile __xdata *)0xfc20)
#define     PWM32T1L    (*(unsigned char volatile __xdata *)0xfc21)
#define     PWM32T2     (*(unsigned int volatile __xdata *)0xfc22)
#define     PWM32T2H    (*(unsigned char volatile __xdata *)0xfc22)
#define     PWM32T2L    (*(unsigned char volatile __xdata *)0xfc23)
#define     PWM32CR     (*(unsigned char volatile __xdata *)0xfc24)
#define     PWM32HLD    (*(unsigned char volatile __xdata *)0xfc25)
#define     PWM33T1     (*(unsigned int volatile __xdata *)0xfc28)
#define     PWM33T1H    (*(unsigned char volatile __xdata *)0xfc28)
#define     PWM33T1L    (*(unsigned char volatile __xdata *)0xfc29)
#define     PWM33T2     (*(unsigned int volatile __xdata *)0xfc2a)
#define     PWM33T2H    (*(unsigned char volatile __xdata *)0xfc2a)
#define     PWM33T2L    (*(unsigned char volatile __xdata *)0xfc2b)
#define     PWM33CR     (*(unsigned char volatile __xdata *)0xfc2c)
#define     PWM33HLD    (*(unsigned char volatile __xdata *)0xfc2d)
#define     PWM34T1     (*(unsigned int volatile __xdata *)0xfc30)
#define     PWM34T1H    (*(unsigned char volatile __xdata *)0xfc30)
#define     PWM34T1L    (*(unsigned char volatile __xdata *)0xfc31)
#define     PWM34T2     (*(unsigned int volatile __xdata *)0xfc32)
#define     PWM34T2H    (*(unsigned char volatile __xdata *)0xfc32)
#define     PWM34T2L    (*(unsigned char volatile __xdata *)0xfc33)
#define     PWM34CR     (*(unsigned char volatile __xdata *)0xfc34)
#define     PWM34HLD    (*(unsigned char volatile __xdata *)0xfc35)
#define     PWM35T1     (*(unsigned int volatile __xdata *)0xfc38)
#define     PWM35T1H    (*(unsigned char volatile __xdata *)0xfc38)
#define     PWM35T1L    (*(unsigned char volatile __xdata *)0xfc39)
#define     PWM35T2     (*(unsigned int volatile __xdata *)0xfc3a)
#define     PWM35T2H    (*(unsigned char volatile __xdata *)0xfc3a)
#define     PWM35T2L    (*(unsigned char volatile __xdata *)0xfc3b)
#define     PWM35CR     (*(unsigned char volatile __xdata *)0xfc3c)
#define     PWM35HLD    (*(unsigned char volatile __xdata *)0xfc3d)
#define     PWM36T1     (*(unsigned int volatile __xdata *)0xfc40)
#define     PWM36T1H    (*(unsigned char volatile __xdata *)0xfc40)
#define     PWM36T1L    (*(unsigned char volatile __xdata *)0xfc41)
#define     PWM36T2     (*(unsigned int volatile __xdata *)0xfc42)
#define     PWM36T2H    (*(unsigned char volatile __xdata *)0xfc42)
#define     PWM36T2L    (*(unsigned char volatile __xdata *)0xfc43)
#define     PWM36CR     (*(unsigned char volatile __xdata *)0xfc44)
#define     PWM36HLD    (*(unsigned char volatile __xdata *)0xfc45)
#define     PWM37T1     (*(unsigned int volatile __xdata *)0xfc48)
#define     PWM37T1H    (*(unsigned char volatile __xdata *)0xfc48)
#define     PWM37T1L    (*(unsigned char volatile __xdata *)0xfc49)
#define     PWM37T2     (*(unsigned int volatile __xdata *)0xfc4a)
#define     PWM37T2H    (*(unsigned char volatile __xdata *)0xfc4a)
#define     PWM37T2L    (*(unsigned char volatile __xdata *)0xfc4b)
#define     PWM37CR     (*(unsigned char volatile __xdata *)0xfc4c)
#define     PWM37HLD    (*(unsigned char volatile __xdata *)0xfc4d)
#define     PWM4C       (*(unsigned int volatile __xdata *)0xfc50)
#define     PWM4CH      (*(unsigned char volatile __xdata *)0xfc50)
#define     PWM4CL      (*(unsigned char volatile __xdata *)0xfc51)
#define     PWM4CKS     (*(unsigned char volatile __xdata *)0xfc52)
#define     PWM4TADC    (*(unsigned int volatile __xdata *)0xfc53)
#define     PWM4TADCH   (*(unsigned char volatile __xdata *)0xfc53)
#define     PWM4TADCL   (*(unsigned char volatile __xdata *)0xfc54)
#define     PWM4IF      (*(unsigned char volatile __xdata *)0xfc55)
#define     PWM4FDCR    (*(unsigned char volatile __xdata *)0xfc56)
#define     PWM40T1     (*(unsigned int volatile __xdata *)0xfc60)
#define     PWM40T1H    (*(unsigned char volatile __xdata *)0xfc60)
#define     PWM40T1L    (*(unsigned char volatile __xdata *)0xfc61)
#define     PWM40T2     (*(unsigned int volatile __xdata *)0xfc62)
#define     PWM40T2H    (*(unsigned char volatile __xdata *)0xfc62)
#define     PWM40T2L    (*(unsigned char volatile __xdata *)0xfc63)
#define     PWM40CR     (*(unsigned char volatile __xdata *)0xfc64)
#define     PWM40HLD    (*(unsigned char volatile __xdata *)0xfc65)
#define     PWM41T1     (*(unsigned int volatile __xdata *)0xfc68)
#define     PWM41T1H    (*(unsigned char volatile __xdata *)0xfc68)
#define     PWM41T1L    (*(unsigned char volatile __xdata *)0xfc69)
#define     PWM41T2     (*(unsigned int volatile __xdata *)0xfc6a)
#define     PWM41T2H    (*(unsigned char volatile __xdata *)0xfc6a)
#define     PWM41T2L    (*(unsigned char volatile __xdata *)0xfc6b)
#define     PWM41CR     (*(unsigned char volatile __xdata *)0xfc6c)
#define     PWM41HLD    (*(unsigned char volatile __xdata *)0xfc6d)
#define     PWM42T1     (*(unsigned int volatile __xdata *)0xfc70)
#define     PWM42T1H    (*(unsigned char volatile __xdata *)0xfc70)
#define     PWM42T1L    (*(unsigned char volatile __xdata *)0xfc71)
#define     PWM42T2     (*(unsigned int volatile __xdata *)0xfc72)
#define     PWM42T2H    (*(unsigned char volatile __xdata *)0xfc72)
#define     PWM42T2L    (*(unsigned char volatile __xdata *)0xfc73)
#define     PWM42CR     (*(unsigned char volatile __xdata *)0xfc74)
#define     PWM42HLD    (*(unsigned char volatile __xdata *)0xfc75)
#define     PWM43T1     (*(unsigned int volatile __xdata *)0xfc78)
#define     PWM43T1H    (*(unsigned char volatile __xdata *)0xfc78)
#define     PWM43T1L    (*(unsigned char volatile __xdata *)0xfc79)
#define     PWM43T2     (*(unsigned int volatile __xdata *)0xfc7a)
#define     PWM43T2H    (*(unsigned char volatile __xdata *)0xfc7a)
#define     PWM43T2L    (*(unsigned char volatile __xdata *)0xfc7b)
#define     PWM43CR     (*(unsigned char volatile __xdata *)0xfc7c)
#define     PWM43HLD    (*(unsigned char volatile __xdata *)0xfc7d)
#define     PWM44T1     (*(unsigned int volatile __xdata *)0xfc80)
#define     PWM44T1H    (*(unsigned char volatile __xdata *)0xfc80)
#define     PWM44T1L    (*(unsigned char volatile __xdata *)0xfc81)
#define     PWM44T2     (*(unsigned int volatile __xdata *)0xfc82)
#define     PWM44T2H    (*(unsigned char volatile __xdata *)0xfc82)
#define     PWM44T2L    (*(unsigned char volatile __xdata *)0xfc83)
#define     PWM44CR     (*(unsigned char volatile __xdata *)0xfc84)
#define     PWM44HLD    (*(unsigned char volatile __xdata *)0xfc85)
#define     PWM45T1     (*(unsigned int volatile __xdata *)0xfc88)
#define     PWM45T1H    (*(unsigned char volatile __xdata *)0xfc88)
#define     PWM45T1L    (*(unsigned char volatile __xdata *)0xfc89)
#define     PWM45T2     (*(unsigned int volatile __xdata *)0xfc8a)
#define     PWM45T2H    (*(unsigned char volatile __xdata *)0xfc8a)
#define     PWM45T2L    (*(unsigned char volatile __xdata *)0xfc8b)
#define     PWM45CR     (*(unsigned char volatile __xdata *)0xfc8c)
#define     PWM45HLD    (*(unsigned char volatile __xdata *)0xfc8d)
#define     PWM46T1     (*(unsigned int volatile __xdata *)0xfc90)
#define     PWM46T1H    (*(unsigned char volatile __xdata *)0xfc90)
#define     PWM46T1L    (*(unsigned char volatile __xdata *)0xfc91)
#define     PWM46T2     (*(unsigned int volatile __xdata *)0xfc92)
#define     PWM46T2H    (*(unsigned char volatile __xdata *)0xfc92)
#define     PWM46T2L    (*(unsigned char volatile __xdata *)0xfc93)
#define     PWM46CR     (*(unsigned char volatile __xdata *)0xfc94)
#define     PWM46HLD    (*(unsigned char volatile __xdata *)0xfc95)
#define     PWM47T1     (*(unsigned int volatile __xdata *)0xfc98)
#define     PWM47T1H    (*(unsigned char volatile __xdata *)0xfc98)
#define     PWM47T1L    (*(unsigned char volatile __xdata *)0xfc99)
#define     PWM47T2     (*(unsigned int volatile __xdata *)0xfc9a)
#define     PWM47T2H    (*(unsigned char volatile __xdata *)0xfc9a)
#define     PWM47T2L    (*(unsigned char volatile __xdata *)0xfc9b)
#define     PWM47CR     (*(unsigned char volatile __xdata *)0xfc9c)
#define     PWM47HLD    (*(unsigned char volatile __xdata *)0xfc9d)
#define     PWM5C       (*(unsigned int volatile __xdata *)0xfca0)
#define     PWM5CH      (*(unsigned char volatile __xdata *)0xfca0)
#define     PWM5CL      (*(unsigned char volatile __xdata *)0xfca1)
#define     PWM5CKS     (*(unsigned char volatile __xdata *)0xfca2)
#define     PWM5IF      (*(unsigned char volatile __xdata *)0xfca5)
#define     PWM5FDCR    (*(unsigned char volatile __xdata *)0xfca6)
#define     PWM50T1     (*(unsigned int volatile __xdata *)0xfcb0)
#define     PWM50T1H    (*(unsigned char volatile __xdata *)0xfcb0)
#define     PWM50T1L    (*(unsigned char volatile __xdata *)0xfcb1)
#define     PWM50T2     (*(unsigned int volatile __xdata *)0xfcb2)
#define     PWM50T2H    (*(unsigned char volatile __xdata *)0xfcb2)
#define     PWM50T2L    (*(unsigned char volatile __xdata *)0xfcb3)
#define     PWM50CR     (*(unsigned char volatile __xdata *)0xfcb4)
#define     PWM50HLD    (*(unsigned char volatile __xdata *)0xfcb5)
#define     PWM51T1     (*(unsigned int volatile __xdata *)0xfcb8)
#define     PWM51T1H    (*(unsigned char volatile __xdata *)0xfcb8)
#define     PWM51T1L    (*(unsigned char volatile __xdata *)0xfcb9)
#define     PWM51T2     (*(unsigned int volatile __xdata *)0xfcba)
#define     PWM51T2H    (*(unsigned char volatile __xdata *)0xfcba)
#define     PWM51T2L    (*(unsigned char volatile __xdata *)0xfcbb)
#define     PWM51CR     (*(unsigned char volatile __xdata *)0xfcbc)
#define     PWM51HLD    (*(unsigned char volatile __xdata *)0xfcbd)
#define     PWM52T1     (*(unsigned int volatile __xdata *)0xfcc0)
#define     PWM52T1H    (*(unsigned char volatile __xdata *)0xfcc0)
#define     PWM52T1L    (*(unsigned char volatile __xdata *)0xfcc1)
#define     PWM52T2     (*(unsigned int volatile __xdata *)0xfcc2)
#define     PWM52T2H    (*(unsigned char volatile __xdata *)0xfcc2)
#define     PWM52T2L    (*(unsigned char volatile __xdata *)0xfcc3)
#define     PWM52CR     (*(unsigned char volatile __xdata *)0xfcc4)
#define     PWM52HLD    (*(unsigned char volatile __xdata *)0xfcc5)
#define     PWM53T1     (*(unsigned int volatile __xdata *)0xfcc8)
#define     PWM53T1H    (*(unsigned char volatile __xdata *)0xfcc8)
#define     PWM53T1L    (*(unsigned char volatile __xdata *)0xfcc9)
#define     PWM53T2     (*(unsigned int volatile __xdata *)0xfcca)
#define     PWM53T2H    (*(unsigned char volatile __xdata *)0xfcca)
#define     PWM53T2L    (*(unsigned char volatile __xdata *)0xfccb)
#define     PWM53CR     (*(unsigned char volatile __xdata *)0xfccc)
#define     PWM53HLD    (*(unsigned char volatile __xdata *)0xfccd)
#define     PWM54T1     (*(unsigned int volatile __xdata *)0xfcd0)
#define     PWM54T1H    (*(unsigned char volatile __xdata *)0xfcd0)
#define     PWM54T1L    (*(unsigned char volatile __xdata *)0xfcd1)
#define     PWM54T2     (*(unsigned int volatile __xdata *)0xfcd2)
#define     PWM54T2H    (*(unsigned char volatile __xdata *)0xfcd2)
#define     PWM54T2L    (*(unsigned char volatile __xdata *)0xfcd3)
#define     PWM54CR     (*(unsigned char volatile __xdata *)0xfcd4)
#define     PWM54HLD    (*(unsigned char volatile __xdata *)0xfcd5)
#define     PWM55T1     (*(unsigned int volatile __xdata *)0xfcd8)
#define     PWM55T1H    (*(unsigned char volatile __xdata *)0xfcd8)
#define     PWM55T1L    (*(unsigned char volatile __xdata *)0xfcd9)
#define     PWM55T2     (*(unsigned int volatile __xdata *)0xfcda)
#define     PWM55T2H    (*(unsigned char volatile __xdata *)0xfcda)
#define     PWM55T2L    (*(unsigned char volatile __xdata *)0xfcdb)
#define     PWM55CR     (*(unsigned char volatile __xdata *)0xfcdc)
#define     PWM55HLD    (*(unsigned char volatile __xdata *)0xfcdd)
#define     PWM56T1     (*(unsigned int volatile __xdata *)0xfce0)
#define     PWM56T1H    (*(unsigned char volatile __xdata *)0xfce0)
#define     PWM56T1L    (*(unsigned char volatile __xdata *)0xfce1)
#define     PWM56T2     (*(unsigned int volatile __xdata *)0xfce2)
#define     PWM56T2H    (*(unsigned char volatile __xdata *)0xfce2)
#define     PWM56T2L    (*(unsigned char volatile __xdata *)0xfce3)
#define     PWM56CR     (*(unsigned char volatile __xdata *)0xfce4)
#define     PWM56HLD    (*(unsigned char volatile __xdata *)0xfce5)
#define     PWM57T1     (*(unsigned int volatile __xdata *)0xfce8)
#define     PWM57T1H    (*(unsigned char volatile __xdata *)0xfce8)
#define     PWM57T1L    (*(unsigned char volatile __xdata *)0xfce9)
#define     PWM57T2     (*(unsigned int volatile __xdata *)0xfcea)
#define     PWM57T2H    (*(unsigned char volatile __xdata *)0xfcea)
#define     PWM57T2L    (*(unsigned char volatile __xdata *)0xfceb)
#define     PWM57CR     (*(unsigned char volatile __xdata *)0xfcec)
#define     PWM57HLD    (*(unsigned char volatile __xdata *)0xfced)

/*   interrupt vectors */
#define     INT0_VECTOR     0
#define     TIMER0_VECTOR   1
#define     INT1_VECTOR     2
#define     TIMER1_VECTOR   3
#define     UART1_VECTOR    4
#define     ADC_VECTOR      5
#define     LVD_VECTOR      6
#define     PCA_VECTOR      7
#define     UART2_VECTOR    8
#define     SPI_VECTOR      9
#define     INT2_VECTOR     10
#define     INT3_VECTOR     11
#define     TIMER2_VECTOR   12
#define     INT4_VECTOR     16
#define     UART3_VECTOR    17
#define     UART4_VECTOR    18
#define     TIMER3_VECTOR   19
#define     TIMER4_VECTOR   20
#define     CMP_VECTOR      21
#define     PWM0_VECTOR     22
#define     PWMFD_VECTOR    23
#define     I2C_VECTOR      24
#define     USB_VECTOR      25
#define     PWMA_VECTOR     26
#define     PWMB_VECTOR     27
#define     PWM1_VECTOR     28
#define     PWM2_VECTOR     29
#define     PWM3_VECTOR     30
#define     PWM4_VECTOR     31
#define     PWM5_VECTOR     32
#define     P0_VECTOR       37
#define     P1_VECTOR       38
#define     P2_VECTOR       39
#define     P3_VECTOR       40
#define     P4_VECTOR       41
#define     P5_VECTOR       42
#define     P6_VECTOR       43
#define     P7_VECTOR       44
#define     DMA_M2M_VECTOR  47
#define     DMA_ADC_VECTOR  48
#define     DMA_SPI_VECTOR  49
#define     DMA_UR1T_VECTOR 50
#define     DMA_UR1R_VECTOR 51
#define     DMA_UR2T_VECTOR 52
#define     DMA_UR2R_VECTOR 53
#define     DMA_UR3T_VECTOR 54
#define     DMA_UR3R_VECTOR 55
#define     DMA_UR4T_VECTOR 56
#define     DMA_UR4R_VECTOR 57
#define     DMA_LCM_VECTOR  58
#define     LCM_VECTOR      59

#endif  // _STC8A8_H
