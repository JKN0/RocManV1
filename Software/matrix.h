/***************************************************************************

 matrix.h
 
 RocMan - PacMan for junk led matrix

 Controller: STC8A8K64D4 @ breakout
 
 12.12.2024  - First version

 ***************************************************************************/

#ifndef INC_MATRIX_H_
#define INC_MATRIX_H_

extern BIT frame_sync;
extern BIT test_flag;


void init_matrix_task( void );
void matrix_task( void );
void serial_rx_task( void );

void dma_spi_isr(void) __interrupt(DMA_SPI_VECTOR) __using (2);

#endif /* INC_MATRIX_H_ */
