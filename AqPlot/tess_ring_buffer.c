/**
  ******************************************************************************
  * File Name          : tess_ring_buffer.c
  * Author			   : Axinte Andrei
  * Description        : Ring buffer module interface source file
  ******************************************************************************
**/
#include "tess_ring_buffer.h"

#if CFG_ACQ_ON

static void DATA_COPY ( const r_buff_data_t *source, r_buff_data_t *dest);
static void FULL_BUFF_DATA(r_buff_data_t *dest);
void TESS_RING_BUFFER_INIT(ring_buffer_t * buff)
{
   uint16_t  idx;


   buff->count         = 0;
   buff->inner_count   = 0;
   buff->head          = 0;
   buff->tail          = 0;

   for (idx = 0; idx < RBUF_SIZE; idx++)
   {
      buff->buff_data[idx].byte0 = 0;
      buff->buff_data[idx].byte1 = 0;
      buff->buff_data[idx].byte2 = 0;
      buff->buff_data[idx].byte3 = 0;
      buff->buff_data[idx].byte4 = 0;
      buff->buff_data[idx].byte5 = 0;
      buff->buff_data[idx].byte6 = 0;
      buff->buff_data[idx].byte7 = 0;
      buff->buff_data[idx].byte8 = 0;
      buff->buff_data[idx].byte9 = 0;
      buff->buff_data[idx].byte10 = 0;
      buff->buff_data[idx].byte11 = 0;
      buff->buff_data[idx].byte12 = 0;
      buff->buff_data[idx].byte13 = 0;
      buff->buff_data[idx].byte14 = 0;
      buff->buff_data[idx].byte15 = 0;
      buff->buff_data[idx].byte16 = 0;
      buff->buff_data[idx].byte17 = 0;
      buff->buff_data[idx].byte18 = 0;
      buff->buff_data[idx].byte19 = 0;
      buff->buff_data[idx].byte20 = 0;
      buff->buff_data[idx].byte21 = 0;
      buff->buff_data[idx].byte22 = 0;
      buff->buff_data[idx].byte23 = 0;
      buff->buff_data[idx].byte24 = 0;
      buff->buff_data[idx].byte25 = 0;
      buff->buff_data[idx].byte26 = 0;
      buff->buff_data[idx].byte27 = 0;
      buff->buff_data[idx].byte28 = 0;
      buff->buff_data[idx].byte29 = 0;
   }
}

r_buff_data_t TESS_RING_BUFFER_GET(ring_buffer_t * buff)
{
   r_buff_data_t  loc_buff_data;


   if (buff->count > 0u)
   {
      /*copy data from the buffer to local structure*/
      DATA_COPY(&(buff->buff_data[buff->tail]), &loc_buff_data);

      buff->tail  = TESS_BUFF_IDX_INC (buff->tail, RBUF_SIZE);
      buff->count--;

   }
   else
   {
      /* fill buffer with dummy data*/
      loc_buff_data.byte0 = 1;
      loc_buff_data.byte1 = 1;
      loc_buff_data.byte2 = 1;
      loc_buff_data.byte3 = 1;
      loc_buff_data.byte4 = 1;
      loc_buff_data.byte5 = 1;
      loc_buff_data.byte6 = 1;
      loc_buff_data.byte7 = 1;
      loc_buff_data.byte8 = 1;
      loc_buff_data.byte9 = 1;
      loc_buff_data.byte10 = 1;
      loc_buff_data.byte11 = 1;
      loc_buff_data.byte12 = 1;
      loc_buff_data.byte13 = 1;
      loc_buff_data.byte14 = 1;
      loc_buff_data.byte15 = 1;
      loc_buff_data.byte16 = 1;
      loc_buff_data.byte17 = 1;
      loc_buff_data.byte18 = 1;
      loc_buff_data.byte19 = 1;
      loc_buff_data.byte20 = 1;
      loc_buff_data.byte21 = 1;
      loc_buff_data.byte22 = 1;
      loc_buff_data.byte23 = 1;
      loc_buff_data.byte24 = 1;
      loc_buff_data.byte25 = 1;
      loc_buff_data.byte26 = 1;
      loc_buff_data.byte27 = 1;
      loc_buff_data.byte28 = 1;
      loc_buff_data.byte29 = 1;
   }

   /*return the entire structure at once*/
   return (loc_buff_data);
}

void TESS_RING_BUFFER_PUT(ring_buffer_t * buff, r_buff_data_t data)
{

   if (buff->count < RBUF_SIZE)
   {
      DATA_COPY(&data, &(buff->buff_data[buff->head]));    /* copy received data to the buffer */

      buff->head = TESS_BUFF_IDX_INC (buff->head, RBUF_SIZE);
      buff->count++;
   }
   else
   {
      /* discard data at tail */
      buff->tail = TESS_BUFF_IDX_INC (buff->tail, RBUF_SIZE);
      FULL_BUFF_DATA(&data);
      /* store new data */
      DATA_COPY(&data, &(buff->buff_data[buff->head]));    /* copy received data to the buffer */
      buff->head = TESS_BUFF_IDX_INC (buff->head, RBUF_SIZE);

      /*count will remain constant*/
   }
}

uint8_t TESS_RINGBUFF_IS_EMPTY( const ring_buffer_t * buff)
{
   return (buff->count == 0);
}

uint8_t TESS_RINGBUFF_IS_FULL( const ring_buffer_t * buff)
{
   return (RBUF_SIZE == buff->count);
}

uint16_t TESS_BUFF_IDX_INC (const uint16_t currentVal,const uint16_t buff_size)
{
   uint16_t  newVal;


   newVal = currentVal + 1;
   newVal %= buff_size;

   return newVal;
}

static void DATA_COPY ( const r_buff_data_t *source, r_buff_data_t *dest)
{
   /* copy byte by byte*/
   dest->byte0 = source->byte0;
   dest->byte1 = source->byte1;
   dest->byte2 = source->byte2;
   dest->byte3 = source->byte3;
   dest->byte4 = source->byte4;
   dest->byte5 = source->byte5;
   dest->byte6 = source->byte6;
   dest->byte7 = source->byte7;
   dest->byte8 = source->byte8;
   dest->byte9 = source->byte9;
   dest->byte10 = source->byte10;
   dest->byte11 = source->byte11;
   dest->byte12 = source->byte12;
   dest->byte13 = source->byte13;
   dest->byte14 = source->byte14;
   dest->byte15 = source->byte15;
   dest->byte16 = source->byte16;
   dest->byte17 = source->byte17;
   dest->byte18 = source->byte18;
   dest->byte19 = source->byte19;
   dest->byte20 = source->byte20;
   dest->byte21 = source->byte21;
   dest->byte22 = source->byte22;
   dest->byte23 = source->byte23;
   dest->byte24 = source->byte24;
   dest->byte25 = source->byte25;
   dest->byte26 = source->byte26;
   dest->byte27 = source->byte27;
   dest->byte28 = source->byte28;
   dest->byte29 = source->byte29;

}

static void FULL_BUFF_DATA(r_buff_data_t *dest)
{
   dest->byte0 = 0;
   dest->byte1 = 0;
   dest->byte2 = 0;
   dest->byte3 = 0;
   dest->byte4 = 0;
   dest->byte5 = 0;
   dest->byte6 = 0;
   dest->byte7 = 0;
   dest->byte8 = 0;
   dest->byte9 = 0;
   dest->byte10 = 0;
   dest->byte11 = 0;
   dest->byte12 = 0;
   dest->byte13 = 0;
   dest->byte14 = 0;
   dest->byte15 = 0;
   dest->byte16 = 0;
   dest->byte17 = 0;
   dest->byte18 = 0;
   dest->byte19 = 0;
   dest->byte20 = 0;
   dest->byte21 = 0;
   dest->byte22 = 0;
   dest->byte23 = 0;
   dest->byte24 = 0;
   dest->byte25 = 0;
   dest->byte26 = 0;
   dest->byte27 = 0;
   dest->byte28 = 0;
   dest->byte29 = 0;
}
#endif /*CFG_ACQ_ON*/

