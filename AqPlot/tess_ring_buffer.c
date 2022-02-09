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

}

static void FULL_BUFF_DATA(r_buff_data_t *dest)
{
   dest->byte0 = 0;
   dest->byte1 = 0;
   dest->byte2 = 0;
   dest->byte3 = 0;
   dest->byte4 = 0;
   dest->byte5 = 0;
}
#endif /*CFG_ACQ_ON*/

