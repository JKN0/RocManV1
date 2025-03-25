/***************************************************************************

 game.h
 
 RocMan - PacMan for junk led matrix

 Controller: STC8A8K64D4 @ breakout
 
 12.12.2024  - First version

 ***************************************************************************/

#ifndef INC_GAME_H_
#define INC_GAME_H_

void init_game_task( void );
void game_task( void );

BYTE get_pixel( BYTE x, BYTE y );

#endif /* INC_GAME_H_ */
