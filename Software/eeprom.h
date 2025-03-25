/***************************************************************************

 eeprom.h

 EEPROM functions for Pacman project
 
 Controller: STC8A8K64D4 @ breakout
 
 2.2.2025  - First version

 ***************************************************************************/

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

typedef struct params_t {
    WORD hi_score;
    BYTE gain;
    BYTE cksum;
} PARAMS_T;

extern PARAMS_T CODE __at (0xF000) params;

void set_default_params( void );
void save_hi_score( WORD newscore );
void save_gain( BYTE newgain );
BIT params_valid( void );

#endif /* INC_EEPROM_H_ */
