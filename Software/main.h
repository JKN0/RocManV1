/***************************************************************************

 main.h
 
 RocMan - PacMan for junk led matrix

 Controller: STC8A8K64D4 @ breakout
 
 12.12.2024  - First version

 ***************************************************************************/

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#define RTC_TIME            5

#define TIME_10MS           (10/RTC_TIME)
#define TIME_20MS           (20/RTC_TIME)
#define TIME_50MS           (50/RTC_TIME)
#define TIME_100MS          (100/RTC_TIME)
#define TIME_150MS          (150/RTC_TIME)
#define TIME_200MS          (200/RTC_TIME)
#define TIME_300MS          (300/RTC_TIME)
#define TIME_500MS          (500/RTC_TIME)
#define TIME_1S             (1000/RTC_TIME)
#define TIME_3S             (3000/RTC_TIME)

#define INBUF_LEN            20
#define OUTBUF_LEN           2000

#define OK                   0
#define FAIL                 1

#define LED_OFF              1
#define LED_ON               0

#define printf               printf_fast

#define TP                   P3_6
#define TP2                  P3_5

extern BIT volatile timeout1;
extern BIT volatile timeout2;
extern BIT volatile timeout3;
extern BIT volatile timeout4;

extern BIT volatile rep_timeout1;
extern BIT volatile rep_timeout2;
extern BIT volatile rep_timeout3;

extern BIT cr_received;
extern BIT tx_on;

extern BYTE XDATA outbuf[];
extern BYTE XDATA *outbuf_bot;
extern BYTE XDATA *outbuf_top;
extern BIT outbuf_full;

extern BYTE XDATA inbuf[];

extern BIT tick;

void set_timeout1( BYTE delay );
void set_timeout2( BYTE delay );
void set_timeout3( WORD delay );
void set_timeout4( WORD delay );

void set_rep_timeout1( BYTE delay );
void set_rep_timeout2( BYTE delay );
void set_rep_timeout3( BYTE delay );

#endif /* INC_MAIN_H_ */
