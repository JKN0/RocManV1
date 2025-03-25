/***************************************************************************

 eeprom.c

 EEPROM functions for Pacman project
 
 Controller: STC8A8K64D4 @ breakout
 
 2.2.2025  - First version

 ***************************************************************************/

#define __STC8__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <datatypes.h>
#include "stc8a8.h"
#include "main.h"
#include "eeprom.h"

/* =====================================================================
------------------------ Constants & macros ------------------------- */

#define TRIGGER_IAP     { IAP_TRIG = 0x5A; IAP_TRIG = 0xA5; }

/* =======================================================================
------------------------ I/O ------------------------------------------ */

/* =====================================================================
------------------------ Structures --------------------------------- */

/* =====================================================================
------------------------  Global variables  ------------------------- */

// Don't forget to set "User EEPROM size = 4K" in STC-ISP !
PARAMS_T CODE __at (0xF000) params;

PARAMS_T PDATA params_buf;

/* =====================================================================
------------------------ Function prototypes ------------------------ */

void erase_params( void );
void write_params( void );
void read_params( void );

/* =====================================================================
Set default params to EEPROM
--------------------------------------------------------------------- */

void set_default_params( void )
{
    params_buf.hi_score = 0;
    params_buf.gain = 5;
    
    erase_params();
    write_params();
}

/* =====================================================================
Save hi score to EEPROM
--------------------------------------------------------------------- */

void save_hi_score( WORD newscore )
{
    read_params();
    params_buf.hi_score = newscore;
    
    erase_params();
    write_params();
}

/* =====================================================================
Save gain to EEPROM
--------------------------------------------------------------------- */

void save_gain( BYTE newgain )
{
    read_params();
    params_buf.gain = newgain;
    
    erase_params();
    write_params();
}

/* =====================================================================
Erase params from EEPROM
--------------------------------------------------------------------- */

void erase_params( void )
{
    IAP_CONTR = IAPEN;
    IAP_ADDR = 0;           // EEPROM address 0
    IAP_CMD = CMD_ERASE;
    TRIGGER_IAP;
    NOP();
    IAP_CONTR = 0;
    IAP_CMD = CMD_NONE;
}

/* =====================================================================
Write params from params_buf to EEPROM
Calculate checksum and write it to EEPROM
--------------------------------------------------------------------- */

void write_params( void )
{
    BYTE PDATA *p = (BYTE PDATA *)params_buf;
    BYTE cs = 0;
    BYTE addr;
    
    IAP_CONTR = IAPEN;
    IAP_CMD = CMD_WRITE;
    
    // EEPROM addresses 0...0FFF
    for (addr = 0; addr < sizeof(params) - 1; addr++)
    {
        BYTE dat = *p++;
        cs += dat;
        
        IAP_ADDR = addr;
        IAP_DATA = dat;    
        TRIGGER_IAP;
        NOP();
    }

    // Write chekcsum
    IAP_ADDR = addr;
    IAP_DATA = cs;
    TRIGGER_IAP;
    NOP();

    IAP_CONTR = 0;
    IAP_CMD = CMD_NONE;
}

/* =====================================================================
Read params from EEPROM to params_buf
--------------------------------------------------------------------- */

void read_params( void )
{
    memcpy(&params_buf,&params,sizeof(params));
}

/* =====================================================================
Check params in EEPROM
--------------------------------------------------------------------- */

BIT params_valid( void )
{
    BYTE CODE *p = (BYTE CODE *)params;
    BYTE cs = 0;
    
    for (BYTE i = 0; i < sizeof(params) - 1; i++)
    {
        cs += *p++;
    }
    
    return cs == *p;
}

/* ============================ EOF ====================================== */

