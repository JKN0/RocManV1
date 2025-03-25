/***************************************************************************
 
 main.c
 
 RocMan - PacMan for junk led matrix

 Controller: STC8A8K64D4 @ breakout
 
 12.12.2024  - First version

 ***************************************************************************/

/* --------------------------------------------------------------------------
 
 CPU I/O-devices used:
 
   Timers:
     T0 = RTC, 5 ms
     T1 = UART1 baud rate 115,2 kbps
     T2 = -
     T3 = sound ch1
     T4 = sound ch2
   
   UARTs:
     UART1 = serial UI + debug
     UART2...4 = -
     
   PCA:
     CH0,CH1 = IR-code receiving
     CH2,CH3 = -
     
   SPI:
     SCK = matrix GCLK pulses at 2.8 MHz (MISO/MOSI not connected)
     
   DMA:
     transfer 32 random bytes via SPI to get 256 SCK pulses
     
 Software timer usage:
 
   Timer 1 = heartbeat led, 1 Hz
   Timer 2 = frame sending timings in matrix task
   Timer 3 = ghost state management timings in game task
   Timer 4 = game mode management timings in game task

   Auto-repeat timer 1 = fast rate tick generation for game task (pacman)
   Auto-repeat timer 2 = medium rate tick generation for game task (normal ghosts)
   Auto-repeat timer 3 = slow rate tick generation for game task (slow ghosts)
   
---------------------------------------------------------------------------- */

#define __STC8__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <datatypes.h>
#include "stc8a8.h"
#include "main.h"
#include "matrix.h"
#include "game.h"
#include "sound.h"
#include "ir_rec.h"

/* =====================================================================
------------------------ Constants & macros ------------------------- */

// sysclock 45 MHz, cycle 22 ns
// T0x12=0
// Timer 0 counts sysclock/12 = 3.75 MHz, reload for 5 ms = 18750
#define TMR0_RELOAD         (WORD)(-18750)

// UART baudrate = 65536 - FOSC/rate/4,  T1x12=1
// 45MHz/115200/4 = 98
#define TMR1_RELOAD         (WORD)(-98)    // 115.2 kbit/s @ 45 MHz

#define TESTCLEAR(s,d)      if (s) {s=0; d=1;} else d=0;
#define CYCLIC_INC(p)       { p++; if (p >= (outbuf+OUTBUF_LEN)) p = outbuf; }

/* =======================================================================
------------------------ I/O ------------------------------------------ */

#define run_led              P3_3       // heartbeat-led

/* =====================================================================
------------------------ Structures --------------------------------- */

/* =====================================================================
------------------------  Global variables  ------------------------- */

BYTE timeout_ctr1;
BYTE timeout_ctr2;
WORD timeout_ctr3;
WORD timeout_ctr4;

BYTE rep_timeout_ctr1;
BYTE rep_timeout_value1;
BYTE rep_timeout_ctr2;
BYTE rep_timeout_value2;
BYTE rep_timeout_ctr3;
BYTE rep_timeout_value3;

BIT volatile timeout1;
BIT volatile timeout2;
BIT volatile timeout3;
BIT volatile timeout4;

BIT volatile rep_timeout1;
BIT volatile rep_timeout2;
BIT volatile rep_timeout3;

BIT volatile rtc_flag;
BIT tick = FALSE;

BIT cr_received = FALSE;
BIT tx_on = FALSE;

XDATA BYTE inbuf[INBUF_LEN];

XDATA BYTE outbuf[OUTBUF_LEN];
XDATA BYTE *outbuf_top = outbuf;
XDATA BYTE *outbuf_bot = outbuf;
BIT outbuf_full = FALSE;

/* =====================================================================
------------------------ Function prototypes ------------------------ */

void init_hw( void );

/* =====================================================================
Main
--------------------------------------------------------------------- */

void main( void )
{
    init_hw();
    init_matrix_task();
    init_game_task();

    ENABLE(EA);          // Enable interrupts
    
    printf_fast("\r\n*** RocMan ***\r\n");
    
    set_timeout1(TIME_100MS);

    TP = 0;
    
    /* --- Main loop --- */
    while (TRUE) 
    {
        // Copy the rtc_flag (set by RTC interrupt) to tick-flag in one atomic operation.
        // This makes sure the tick won't change during the main loop.
        TESTCLEAR(rtc_flag,tick);

        // Run tasks
        matrix_task();
        game_task();
        sound_task();
        ir_task();
        serial_rx_task();
        
        // Hearbeat led
        if (timeout1)
        {
            run_led = !run_led;
            set_timeout1(TIME_500MS);
        }

        // tick-flag valid only for one main loop iteration 
        tick = FALSE;
    }
}

/* =======================================================================
Start timeout 1
----------------------------------------------------------------------- */

void set_timeout1( BYTE delay )
{
    DISABLE(ET0);
    timeout1 = FALSE;
    timeout_ctr1 = delay;
    ENABLE(ET0);
}

/* =======================================================================
Start timeout 2
----------------------------------------------------------------------- */

void set_timeout2( BYTE delay )
{
    DISABLE(ET0);
    timeout2 = FALSE;
    timeout_ctr2 = delay;
    ENABLE(ET0);
}

/* =======================================================================
Start timeout 3
----------------------------------------------------------------------- */

void set_timeout3( WORD delay )
{
    DISABLE(ET0);
    timeout3 = FALSE;
    timeout_ctr3 = delay;
    ENABLE(ET0);
}

/* =======================================================================
Start timeout 4
----------------------------------------------------------------------- */

void set_timeout4( WORD delay )
{
    DISABLE(ET0);
    timeout4 = FALSE;
    timeout_ctr4 = delay;
    ENABLE(ET0);
}

/* =======================================================================
Start auto-repeating timeout 1
----------------------------------------------------------------------- */

void set_rep_timeout1( BYTE delay )
{
    DISABLE(ET0);
    rep_timeout1 = FALSE;
    rep_timeout_ctr1 = delay;
    rep_timeout_value1 = delay;
    ENABLE(ET0);
}

/* =======================================================================
Start auto-repeating timeout 2
----------------------------------------------------------------------- */

void set_rep_timeout2( BYTE delay )
{
    DISABLE(ET0);
    rep_timeout2 = FALSE;
    rep_timeout_ctr2 = delay;
    rep_timeout_value2 = delay;
    ENABLE(ET0);
}

/* =======================================================================
Start auto-repeating timeout 3
----------------------------------------------------------------------- */

void set_rep_timeout3( BYTE delay )
{
    DISABLE(ET0);
    rep_timeout3 = FALSE;
    rep_timeout_ctr3 = delay;
    rep_timeout_value3 = delay;
    ENABLE(ET0);
}

/* =======================================================================
putchar. printf_fast() calls this.
----------------------------------------------------------------------- */

int putchar( int ch )
{
    DISABLE(ES);
    
    if (!outbuf_full)
    {
        *outbuf_top = ch;

        CYCLIC_INC(outbuf_top);
        if (outbuf_bot == outbuf_top)
            outbuf_full = TRUE;

        if (!tx_on)
        {
            tx_on = TRUE;
            TI = 1;
        }
    }
    
    ENABLE(ES);

    return ch;
}

/* =======================================================================
putch_intr. putchar() to be called from the serial interrupt.
----------------------------------------------------------------------- */

void putch_intr( BYTE ch ) __using (1) 
{
    if (!outbuf_full)
    {
        *outbuf_top = ch;

        CYCLIC_INC(outbuf_top);
        if (outbuf_bot == outbuf_top)
            outbuf_full = TRUE;

        if (!tx_on)
        {
            tx_on = TRUE;
            TI = 1;
        }
    }
}

/* =======================================================================
Init hardware
----------------------------------------------------------------------- */

void init_hw( void )
{
    EN_XSFR();          // Enable extended SFRs
    
    // Clock: 45 MHz
    
    // -- Ports
    // Pinmode-macros cause warnings 'unreachable code': ignore them
    #pragma save
    #pragma disable_warning 126
    P3_pinmode(QUASIBD,B3);                 // led
    P3_pinmode(PUSHPULL,B6|B5);             // TP,TP2
    P1_pinmode(PUSHPULL,B5|B4|B3|B1|B0);    // SPI pins
    P0_pinmode(PUSHPULL,ALL);               // addr,T3CLKO,T4CLKO
    P2_pinmode(PUSHPULL,ALL);               // RGB
    #pragma restore
    
    // -- Timers
    AUXR = T1x12;       // T0x12=0, T1x12=1
    AUXR &= ~S1ST2;     // S1ST2=0 (select T1 as baud rate generator)
    
    TMOD = 0x00;        // Tmr 0 = mode 0, timer; Tmr 1 = mode 0, timer
                        // Note: mode 0 = 16-bit auto-reload in STC8
    TCON = 0x00;        // IEx = 0, TRx = 0, TFx = 0
    
    // Timer 0: RTC
    TMR0 = TMR0_RELOAD;
    TR0 = 1;

    // Timer 1: baud rate generator
    TMR1 = TMR1_RELOAD;
    TR1 = 1;
    
    // Timer 3 & 4: sound generators
    T4T3M = T3CLKO | T4CLKO;    // enable freq output

    // -- UART
    SM1 = 1;            // Mode 1
    REN = 1;            // Receive enable
    
    // -- PCA   
    CMOD = 0x00;        // internal clock, Fosc/12 = 3.75 MHz
    CCON = 0x00;        // all interrupts off
    CCAPM0 = 0x11;      // channel 0: capture neg edge, interrupt enabled
    CCAPM1 = 0x48;      // channel 1: software timer, interrupt disabled
    CR = 1;             // PCA run
 
    // -- SPI
    SPDAT = 0;                             // initial SPI data
    SPSTAT = SPIF | WCOL;                  // clear SPIF and WCOL
    SPCTL = SPEN | MSTR | SSIG | SPR1;     // master mode, ignore SS, clk 2.8 MHz
    
    // -- DMA: SPI send
    DMA_SPI_CFG = ACT_TX;                  // DMA transmits via SPI
    DMA_SPI_STA = 0;
    
    // -- EEPROM
    IAP_TPS = 45;   // clk = 45 MHz
    
    // -- Interrupts
    // Enable: Tmr0, UART1, DMA SPI
    // Note: in STC8 there is no separate EC (enable PCA) bit, just ECF and ECCFx directly
    ENABLE(ET0);    
    ENABLE(ES);
    DMA_SPI_CFG |= SPIIE | SPIIP1;         // DMA int prio 1

}

/* ======================================================================= */
/* =====================       INTERRUPT SERVICES     ==================== */
/* ======================================================================= */

/* =======================================================================
Real time interrupt: Timer 0, 5 ms interval 

Run the timeouts.
----------------------------------------------------------------------- */

void rtc_isr(void) __interrupt(TF0_VECTOR) __using (1) 
{
    static BIT rtc_toggle = FALSE;
    
    // Auto reload, no need to reload TMR0

    /*
     * --- Timers ---
     */
     
    // Decrement timeout counters and set the flag, if counter == 0
    timeout_ctr1--;
    if (timeout_ctr1 == 0)
        timeout1 = TRUE;
        
    timeout_ctr2--;
    if (timeout_ctr2 == 0)
        timeout2 = TRUE;
        
    timeout_ctr3--;
    if (timeout_ctr3 == 0)
        timeout3 = TRUE;
    
    timeout_ctr4--;
    if (timeout_ctr4 == 0)
        timeout4 = TRUE;
    
    rep_timeout_ctr1--;
    if (rep_timeout_ctr1 == 0)
    {
        rep_timeout1 = TRUE;
        rep_timeout_ctr1 = rep_timeout_value1;
    }

    rep_timeout_ctr2--;
    if (rep_timeout_ctr2 == 0)
    {
        rep_timeout2 = TRUE;
        rep_timeout_ctr2 = rep_timeout_value2;
    }

    rep_timeout_ctr3--;
    if (rep_timeout_ctr3 == 0)
    {
        rep_timeout3 = TRUE;
        rep_timeout_ctr3 = rep_timeout_value3;
    }

    // Set the rtc_flag every 10 ms 
    rtc_toggle = !rtc_toggle;
    if (rtc_toggle)
        rtc_flag = TRUE;
}

/* =======================================================================
Serial interrupt
----------------------------------------------------------------------- */

void serial_isr(void) __interrupt(UART1_VECTOR) __using (1) 
{
    static BYTE idx = 0;
    BYTE ch;

    // --- UART receive
    if (RI)
    {
        RI = 0;
        
        ch = SBUF & 0x7F; 

        if (ch == '\r')                 // if CR -> line ready
        {
            inbuf[idx] = '\0';
            idx = 0;

            putch_intr('\r');              // echo CR
            putch_intr('\n');

            cr_received = TRUE;         // full line received
        }
        else if (ch == '\b' && idx > 0) // if BS -> remove char from buf
        {
            idx--;
            putch_intr('\b');              // echo BS
            putch_intr(' ');
            putch_intr('\b');
        }
        else if (ch >= ' ')             // normal char
        {
            ch |= 0x20;                 // to lowercase

            if (idx < INBUF_LEN-1)      // to buf, if fits
            {
                inbuf[idx] = ch;
                idx++;

                putch_intr(ch);          // echo
           }
        }
    }

    // --- UART send
    if (TI)
    {
        TI = 0;
        
        // Anything to send?
        if (outbuf_bot != outbuf_top || outbuf_full)
        {
            SBUF = *outbuf_bot;          // Transmit the character
            CYCLIC_INC(outbuf_bot);
            outbuf_full = FALSE;
        }
        else
            tx_on = FALSE;
    }
}

/* =======================================================================
----------------------------------------------------------------------- */

unsigned char __sdcc_external_startup(void)
{
    return 0;
}

/* ============================ EOF ====================================== */
