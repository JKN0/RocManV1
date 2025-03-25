/***************************************************************************

 sound.h

 RocMan - PacMan for junk led matrix
 
 Controller: STC8A8K64D4 @ breakout
 
 8.1.2025  - First version

 ***************************************************************************/

#ifndef INC_SOUND_H_
#define INC_SOUND_H_

#define SND_MASK_CH1        0x10
#define SND_MASK_CH2        0x20
#define SND_MASK_STOP1      0x40
#define SND_MASK_STOP2      0x80
#define SND_MASK_SEQNR      0x0F

// Sound commands
#define SND_NONE            0
#define SND_READY_JINGLE    ( 1 | SND_MASK_CH1 | SND_MASK_CH2)
#define SND_RUN_A           ( 2 | SND_MASK_CH2)
#define SND_RUN_B           ( 3 | SND_MASK_CH2)
#define SND_RUN_C           ( 4 | SND_MASK_CH2)
#define SND_FRIGHTEN        ( 5 | SND_MASK_CH2)
#define SND_CHOMP           ( 6 | SND_MASK_CH1)
#define SND_DIE             ( 7 | SND_MASK_CH1)
#define SND_CAPTURE         ( 8 | SND_MASK_CH1)
#define SND_EYES            ( 9 | SND_MASK_CH2)
#define SND_FRUIT           (10 | SND_MASK_CH1)
#define SND_LEVEL           (11 | SND_MASK_CH2)

#define SND_STOP            (SND_MASK_STOP1 | SND_MASK_STOP2)
#define SND_STOP1           SND_MASK_STOP1
#define SND_STOP2           SND_MASK_STOP2

extern BYTE sound_cmd;

void sound_task( void );

#endif /* INC_SOUND_H_ */
