/***************************************************************************

 matrix.c
 
 RocMan - PacMan for junk led matrix

 Controller: STC8A8K64D4 @ breakout
 
 12.12.2024  - First version

 ***************************************************************************/

#define __STC8__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <datatypes.h>
#include "stc8a8.h"
#include "main.h"
#include "matrix.h"
#include "game.h"
#include "eeprom.h"

/* =====================================================================
------------------------ Constants & macros ------------------------- */

#define DELAY_60NS      { NOP(); NOP(); NOP(); }
#define DELAY_300NS     { BYTE d=4; do NOP(); while (--d); }
#define DELAY_1200NS    { BYTE d=14; do NOP(); while (--d); }

#define PULSE_DCLK      { DCLK = 1; DELAY_60NS; DCLK = 0; }

#define frame_reset()   { addr = 0; ADDR_PORT = 0; }

// Configuration register 1
// - ghost elimination: disable
// - scan lines: 11
// - 14-bit PWM
// - GCLK multiplier: enable
// - current gain: HC off, value = 11
//#define CONFIG_REG_1    0x0A4B
#define CONFIG_REG_1    0x0A40      // no gain value, OR this with EEPROM param

#define CMD_PRE         14
#define CMD_RESET       10

#define FRAME_TIME      (80/RTC_TIME)

/* =======================================================================
------------------------ I/O ------------------------------------------ */

// P1.5 = SCK               // GCLK, marked OE in matrix
// P1.4 = MISO              // not used
// P1.3 = MOSI              // not used
// P1.2 = SS                // not used

#define LE          P1_1    // LE
#define DCLK        P1_0    // DCLK

// Note silkscreen errors in matrix PCB:
// - address bit order (MSB->LSB): CDBA
// - data channels: 1 = lower, 2 = upper
#define ADDR_PORT   P0      // P0.0 ... P0.3 = addr A,B,D,C
#define DATA_PORT   P2      // P2.0 ... P2.3 = DR1,DG1,DB1 = data ch 1: lower
                            // P2.4 ... P2.6 = DR2,DG2,DB2 = data ch 2: upper

// P1.7 = PCA CCP0   IR receiver

// P3.0 = UART1 RXD
// P3.1 = UART1 TXD

// P0.5 = T3CLKO  sound ch1
// P0.7 = T4CLKO  sound ch2

/* =====================================================================
------------------------ Structures --------------------------------- */

/* =====================================================================
------------------------  Global variables  ------------------------- */

BYTE addr = 0;

volatile BIT vsync_req = FALSE;
volatile BIT gclk_run = FALSE;
volatile BIT gclk_stopped = TRUE;

BYTE line_nr = 0;

BIT frame_sync = FALSE;

BIT test_flag = FALSE;

BYTE XDATA scanline_data[44];

BYTE CODE palette[][16] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },    // 0 = blank
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },    // 1 = non-transparent black
    { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 },    // 2 = white
    { 1,0,0,1,0,0,1,1,1,1,0,1,1,0,1,1 },    // 3 = red, ghost 0
    { 2,0,2,2,0,2,0,2,1,7,4,6,0,7,7,6 },    // 4 = green (fruits)
    { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4 },    // 5 = blue
    { 3,3,0,3,0,3,0,0,3,0,3,0,3,3,0,0 },    // 6 = yellow, Pacman
    { 5,5,7,7,5,5,5,5,5,5,5,7,5,7,5,5 },    // 7 = pink, ghost 1
    { 6,0,0,6,0,0,6,6,6,6,0,6,6,0,6,6 },    // 8 = cyan, ghost 2
    { 1,3,3,7,3,5,3,5,3,1,3,7,1,3,1,1 },    // 9 = brown, ghost 3
    { 0,4,6,6,0,1,0,4,6,7,6,6,5,1,2,1 },    // 10 = maze
    { 1,7,0,6,2,2,5,0,5,2,7,7,6,2,2,4 },    // 11 = dots
    { 1,3,3,7,3,5,3,5,3,1,3,7,1,3,1,1 },    // 12 = ghost door
    { 1,1,3,1,1,3,1,1,3,3,3,1,1,3,1,3 },    // 13 = orange (logo)
    { 1,7,1,1,1,7,1,7,1,1,7,7,7,7,7,7 },    // 14 = pink (fruits)
    { 3,1,3,1,3,3,3,3,3,3,1,1,3,1,1,3 },    // 15 = yellow (fruits)
};

/* =====================================================================
------------------------ Function prototypes ------------------------ */

void get_chain_data( BYTE line_nr, BYTE led, BYTE XDATA *dat );
void write_chain( BYTE XDATA *dat );
void write_config1( WORD config );
void send_control_command( BYTE pulses );
void gclk_start( void );
void gclk_stop( void );

/* =====================================================================
Serial task
Simple UI for EEPROM params management
--------------------------------------------------------------------- */

void serial_rx_task( void )
{
    if (cr_received)
    {
        cr_received = FALSE;

        switch (inbuf[0])
        {
        case 'l':
            printf("hi score: %d\r\n",params.hi_score);
            printf("gain: %d\r\n",params.gain);
            printf("csum: 0x%x\r\n",params.cksum);
            break;

        case 'r':
            save_hi_score(0);
            printf("Hi score reset\r\n");
            break;

        case 'g':
            BYTE g = atoi(inbuf+1);
            save_gain(g);
            printf("New gain = %d\r\n",g);
            break;

        case 'h':
            printf("Commands:\r\n");
            printf("  l = list params\r\n");
            printf("  r = reset hi score\r\n");
            printf("  g<gain> = set gain\r\n");
            break;
        }
    }
}

/* =====================================================================
--------------------------------------------------------------------- */

void init_matrix_task( void )
{
    LE = 0;
    DCLK = 0;
    DATA_PORT = 0;

    frame_reset();
}

/* =====================================================================
Main state machine for sending data to display
Frame time is 80 ms = 12.5 FPS
Actual data sending takes about 72 ms, rest of the frame time is used for
drawing next picture to frame buffer.
--------------------------------------------------------------------- */

#define S_INIT          0
#define S_WAIT_CONFIG   1
#define S_WAIT_RESET    2
#define S_SEND_DATA     3
#define S_WAIT_FRAME    4

void matrix_task( void )
{
    static BYTE state = S_INIT;
    static BYTE line_nr = 0;

    switch (state)
    {
    case S_INIT:
        // Write config word and wait 100 ms
        write_config1(CONFIG_REG_1 | params.gain);
        //write_config1(CONFIG_REG_1 | 5);    // hard-coded gain = 5
        set_timeout2(TIME_100MS);
        state = S_WAIT_CONFIG;
        break;

    case S_WAIT_CONFIG:
        if (timeout2)
        {
            // Reset display and wait 100 ms
            send_control_command(CMD_RESET);
            frame_reset();
            set_timeout2(TIME_100MS);
            state = S_WAIT_RESET;
        }
        break;

    case S_WAIT_RESET:
        if (timeout2)
        {
            // Start GCLK and start sending display data
            gclk_start();
            set_timeout2(FRAME_TIME);
            state = S_SEND_DATA;
        }
        break;

    case S_SEND_DATA:
        // Send all 16 leds for one line
        TP = 1;
        for (BYTE led = 0; led < 16; led++) 
        {
            get_chain_data(line_nr, led, scanline_data);
            write_chain(scanline_data);
        }
        
        line_nr++;
        if (line_nr == 11)
        {
            // All lines sent
            vsync_req = TRUE;       // request display VSYNC --> DMA ISR
            frame_sync = TRUE;      // permission to draw to frame buffer --> game_task
            state = S_WAIT_FRAME;
        }
        TP = 0;
        break;
        
    case S_WAIT_FRAME:
        // Wait rest of the frame time (80 ms) and restart data sending
        if (timeout2)
        {
            LE = 0;
            DCLK = 0;
            DATA_PORT = 0;
            
            line_nr = 0;
            set_timeout2(FRAME_TIME);
            state = S_SEND_DATA;
        }
        break;
    }
}

/* =====================================================================
Get 44 pixels of data to dat-buffer
get_pixel(x,y) returns the actual pixel color value (0...15)
--------------------------------------------------------------------- */

void get_chain_data( BYTE line_nr, BYTE led, BYTE XDATA *dat ) 
{
    BYTE led_column = led & 0x07;
    BYTE led_row;

    if (led & 0x08)
        led_row = 0;
    else
    {
        led_row = 11;
        led_column = 7 - led_column;
    }
    
    BYTE row = line_nr + led_row;

    // Outer loop (for j=0...3) unrolled to reduce push/pops
    
    for (BYTE i = 0; i < 11; i++)
    {
        // This looks unnecessary complicated, but SDCC produces 
        // significantly less machine code than with the original one-liner:
        //   dat[start+i] = get_pixel((8 * i) + led_column, row);
        
        BYTE col = (i << 3) + led_column;
        BYTE pixel = get_pixel(col,row);
        BYTE idx = 33 + i;
        dat[idx] = pixel;
    }
    row += 22;

    for (BYTE i = 0; i < 11; i++)
    {
        BYTE col = (i << 3) + led_column;
        BYTE pixel = get_pixel(col,row);
        BYTE idx = 22 + i;
        dat[idx] = pixel;
    }
    row += 22;

    for (BYTE i = 0; i < 11; i++)
    {
        BYTE col = (i << 3) + led_column;
        BYTE pixel = get_pixel(col,row);
        BYTE idx = 11 + i;
        dat[idx] = pixel;
    }
    row += 22;

    for (BYTE i = 0; i < 11; i++)
    {
        BYTE col = (i << 3) + led_column;
        BYTE pixel = get_pixel(col,row);
        dat[i] = pixel;
    }

/*    
    // Original unoptimized code:
    
    BYTE start = 33;
    for (j = 0; j < 4; j++)
    {
        for (BYTE i = 0; i < 11; i++)
            dat[start+i] = get_pixel((8 * i) + led_column, row);
        
        row += 22;
        start -= 11;
    } 
*/
}

/* =====================================================================
Write chain data

  Write 44 bytes of data to matrix from dat-buffer. Buffer contains 
  color values (0...15) which are used as palette indexes.

  Optimized using assembly
  
  Function execution time 218 us 
  Executed 176 times per frame = 38.4 ms
    ==> theoretical frame rate 26 fps, if everything else would take no time
  DCLK frequency is 1.9 MHz during 16 pulse burst
  
  Because STC's DPTR1 implementation is stupid, we have to disable 
  all interrupts while using DPTR1. They are disabled about 9 us at a time.
  Because of this, there may be several interrupt requests in queue when 
  re-enabling the interrupts. We want to make sure that DMA interrupt is 
  served first, that's why it is on higher priority (prio 1) than others.

--------------------------------------------------------------------- */

void write_chain( BYTE XDATA *dat )
{
    dat;

/* Original code:
    
    BYTE CODE *p1,*p2;

    for (BYTE ic = 0; ic < 22; ic++)
    {
        p1 = palette[dat[ic]];
        p2 = palette[dat[ic+22]];
        
        for (BYTE bitnr = 0; bitnr < 16; bitnr++)
        {
            if (ic == 21 && bitnr == 15)
                LE = 1;
            
            DATA_PORT = (*p1 << 4) | *p2;
            p1++;
            p2++;
            NOP();
            PULSE_DCLK;
        }
    }
 */ 

    // R7,R6 = dat
    // R5,R4 = dat+22
    // DPH,DPL = p1
    // DPH1,DPL1 = p2
    // R3 = bitnr
    // R2 = ic
    // R1 = temporary
    
    __asm

;   &dat[0] --> R7,R6
    MOV     R6,DPL
    MOV     R7,DPH

;   &dat[22] --> R5,R4
    MOV     A,R6
    ADD     A,#22
    MOV     R4,A
    MOV     A,R7
    ADDC    A,#0
    MOV     R5,A
    
;   for (ic = 22; ic != 0; ic--) // begin
    MOV     R2,#22

10000$:
;   p2 = palette[dat[ic+22]];
    MOV     DPL,R4          ; dat+22 ptr L
    MOV     DPH,R5          ; dat+22 ptr H
    MOVX    A,@DPTR
    INC     DPTR            ; ptr++
    MOV     R4,DPL
    MOV     R5,DPH
    SWAP    A               ; *16  (upper half always 0)
    ADD     A,#_palette     ; p2 --> DPH1,DPL1
    MOV     _DPL1,A
    MOV     A,#(_palette >> 8)
    ADDC    A,#0
    MOV     _DPH1,A

;   p1 = palette[dat[ic]];
    MOV     DPL,R6         ; dat ptr L
    MOV     DPH,R7         ; dat ptr H
    MOVX    A,@DPTR         
    INC     DPTR            ; ptr++
    MOV     R6,DPL
    MOV     R7,DPH
    SWAP    A               ; *16
    ADD     A,#_palette     ; p1 --> DPH,DPL
    MOV     DPL,A
    MOV     A,#(_palette >> 8)
    ADDC    A,#0
    MOV     DPH,A
    
;   for (bitnr = 0; bitnr != 16; bitnr++) // begin
    MOV     R3,#0
    CLR     EA              ; disable ints while messing with DPTR1
                            ; ints are disabled about 9 us
10001$:
;   if (ic == 1 && bitnr == 15)
    CJNE    R2,#1,10002$
    CJNE    R3,#15,10002$
    SETB    _P1_1           ; LE = 1
10002$:

;   DATA_PORT = (*p1 << 4) | *p2;
    MOV     A,R3
    MOVC    A,@A+DPTR       ; *(p1+bitnr) --> A
    SWAP    A               ; << 4  (upper half always 0)
    MOV     R1,A

    INC     _DPS            ; select DPTR1
    MOV     A,R3
    MOVC    A,@A+DPTR       ; *(p2+bitnr) --> A
    ORL     A,R1
    MOV     _P2,A           ; data to chain
    INC     _DPS            ; select DPTR0    acts as tSU0 

;   PULSE DCLK
    SETB    _P1_0
    INC     R3              ; bitnr++         acts as tW(CLK)
    CLR     _P1_0

;   for (bitnr = 0; bitnr != 16; bitnr++) // end
    CJNE    R3,#16,10001$

    SETB    EA              ; re-enable ints

;   for (ic = 22; ic != 0; ic--) // end
    DJNZ    R2,10000$

    CLR     _P1_1           ; LE = 0

    __endasm;
}

/* =====================================================================
Write to Configuration register 1 (16 bit)
--------------------------------------------------------------------- */

void write_config1( WORD config )
{
    WORD mask = 0x8000;
    
    DATA_PORT = 0;
    
    send_control_command(CMD_PRE);
    
    for (WORD i = 0; i < 16*22; i++)
    {
        // LE high for last 4 DCLKs
        if (i == 16*22 - 4)
            LE = 1;
        
        DATA_PORT = (config & mask) ? 0x77 : 0x00;
        PULSE_DCLK;
        
        mask >>= 1;
        if (mask == 0) mask = 0x8000;
    }
    
    LE = 0;
}

/* =====================================================================
Send control command to shift registers: N DCLK pulses while LE is high
--------------------------------------------------------------------- */

void send_control_command( BYTE pulses )
{
    LE = 0;
    DCLK = 0;
    DELAY_60NS;
    LE = 1;
    DELAY_60NS;
    
    do {
        PULSE_DCLK;
        DELAY_60NS;
    } 
    while (--pulses);
    
    LE = 0;
}

/* =====================================================================
Start GCLK
--------------------------------------------------------------------- */

void gclk_start( void )
{
    gclk_run = TRUE;
    DMA_SPI_STA = SPIIF;     // set SPIIF to get DMA going
}

/* =====================================================================
Stop GCLK and wait until stopped (lasts max 100 us)
--------------------------------------------------------------------- */

void gclk_stop( void )
{
    gclk_stopped = FALSE;
    gclk_run = FALSE;
    
    while (!gclk_stopped)
        ;
}

/* =====================================================================
After DMA has sent 32 bytes via SPI, causing 256 pulses in SPI SCK line,
DMA activates SPIIF (SPI Interrupt Flag). This is the interrupt service
for it.
This service steps the address and sends the 257th pulse. It also 
retriggers DMA to send another 32 SPI bytes. If vsync_req is on, first 
send two DCLK pulses while LE is on to signal VSYNC to shift registers.

This is on priority 1, and uses reg bank 2, see comment in write_chain().
--------------------------------------------------------------------- */

void dma_spi_isr(void) __interrupt(DMA_SPI_VECTOR) __using (2) 
{
    // Check that we came here because of SPIIF
    if (DMA_SPI_STA & SPIIF)
    {
        // If VSYNC requested and we are at last addr, execute VSYNC
        if (vsync_req && addr == 10)
        {
            LE = 0;
            DCLK = 0;
            DELAY_60NS;

            // VSYNC command = two DLCK pulses while LE is high
            LE = 1;
            DELAY_60NS;
            PULSE_DCLK;
            DELAY_60NS;
            PULSE_DCLK;
            DELAY_60NS;
            LE = 0;
            
            vsync_req = FALSE;
        }

        DELAY_300NS;

       // Increment address 0...10 cyclic and write
        if (++addr > 10) addr = 0;
        ADDR_PORT = addr;

        DELAY_300NS;
        
        // --- Generate the 257th GCLK pulse
        
        // Disable SPI: this turns the SCK line to normal port pin
        // and the 1 in the port output latch appears in the output
        // --> GCLK = 1
        SPCTL &= ~SPEN;
        
        // Prepare DMA to send 32 bytes = 256 SCK pulses
        DMA_SPI_TXAH = 0x1F;                        // random start address in XRAM (1F00H)
        DMA_SPI_TXAL = 0x00;
        DMA_SPI_AMT = 31;                           // amount: 32 bytes

        DELAY_60NS;                                 // make tdth long enough
        SPCTL |= SPEN;                              // re-enable SPI --> GCLK = 0
        
        DELAY_1200NS;                               // tdtl
        
        if (gclk_run)
            DMA_SPI_CR = ENSPI | TRIG_M | CLRFIFO;  // start DMA
        else
            gclk_stopped = TRUE;
    }
    
    DMA_SPI_STA = 0;    // clear SPIIF,TXOVW,RXLOSS
}

/* ============================ EOF ====================================== */

