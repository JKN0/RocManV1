/***************************************************************************

 ir_rec.h

 NEC IR protocol receiver 
 
 Controller: STC8A8K64D4 @ breakout
 
 15.12.2024  - First version

 ***************************************************************************/

#ifndef INC_IR_REC_H_
#define INC_IR_REC_H_

#define IRCMD_UP        0x00
#define IRCMD_DOWN      0x01
#define IRCMD_RIGHT     0x02
#define IRCMD_LEFT      0x03
#define IRCMD_OK        0x1F
#define IRCMD_0         0x10
#define IRCMD_NONE      0xFF

extern BYTE ir_cmd;

void pca_isr(void) __interrupt(PCA_VECTOR) __using (1);

void ir_task( void );

#endif /* INC_IR_REC_H_ */
