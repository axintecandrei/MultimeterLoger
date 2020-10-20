/**
  ******************************************************************************
  * File Name          : tess_ring_buffer.h
  * Author			   : Axinte Andrei
  * Description        : Ring buffer module header file
  ******************************************************************************
**/
#ifndef __tess_ring_buffer_h__
#define __tess_ring_buffer_h__

#include "config.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"


#if CFG_ACQ_ON

#define RBUF_SIZE                               1536
#define Benchalign_buffer_overwrite             0x0001
#define Benchaling_buffer_concurent_write_read  0x0002
#define Buff_lock_none                          ((uint8_t)0x00)
#define Buff_lock_read                          ((uint8_t)0x01)
#define Buff_lock_write                         ((uint8_t)0x02)

typedef struct r_buff_data_s
{
   uint8_t byte0;
   uint8_t byte1;
   uint8_t byte2;
   uint8_t byte3;
   uint8_t byte4;
   uint8_t byte5;
   uint8_t byte6;
   uint8_t byte7;
   uint8_t byte8;
   uint8_t byte9;
   uint8_t byte10;
   uint8_t byte11;
   uint8_t byte12;
   uint8_t byte13;
   uint8_t byte14;
   uint8_t byte15;
   uint8_t byte16;
   uint8_t byte17;
   uint8_t byte18;
   uint8_t byte19;
   uint8_t byte20;
   uint8_t byte21;
   uint8_t byte22;
   uint8_t byte23;
   uint8_t byte24;
   uint8_t byte25;
   uint8_t byte26;
   uint8_t byte27;
   uint8_t byte28;
   uint8_t byte29;

}r_buff_data_t;


typedef struct
{
   uint16_t head;
   uint16_t tail;
   uint16_t count;
   uint16_t inner_count;
   uint8_t buffer_acces;
   uint8_t buffer_flags;
   r_buff_data_t buff_data[RBUF_SIZE];

}ring_buffer_t;

r_buff_data_t  TESS_RING_BUFFER_DATA;
r_buff_data_t  TESS_TO_RING_BUFFER;
ring_buffer_t  TESS_RING_BUFFER;
uint32_t       dma_isr;


extern void TESS_RING_BUFFER_INIT(ring_buffer_t * buff);
extern r_buff_data_t TESS_RING_BUFFER_GET(ring_buffer_t * buff);
extern void TESS_RING_BUFFER_PUT(ring_buffer_t * buff, r_buff_data_t data);
extern uint8_t TESS_RINGBUFF_IS_EMPTY( const ring_buffer_t * buff);
extern uint8_t TESS_RINGBUFF_IS_FULL( const ring_buffer_t * buff);
uint16_t TESS_BUFF_IDX_INC (const uint16_t currentVal,const uint16_t buff_size);
#endif /*CFG_ACQ_ON*/
#endif /* __tess_ring_buffer_h__ */

