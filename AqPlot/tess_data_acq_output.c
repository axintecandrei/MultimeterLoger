/**
  ******************************************************************************
  * File Name          : tess_data_acq_output.c
  * Author			       : Axinte Andrei
  * Description        : IO pin mode setting source file
  ******************************************************************************
**/

/*INCLUDES*/
#include "tess_data_acq_output.h"

#if CFG_ACQ_ON

static void TESS_FILL_DASOUTPUT_BUFFER(r_buff_data_t* out_buffer);
static void TESS_DAS_ADD_SIGNAL(float signal, dtype_t dtype, float resolution);

void TESS_DAS_INIT()
{

   TESS_RING_BUFFER_INIT(&TESS_RING_BUFFER);
   TESS_LOOP_CNT = 0;

   /*init das buffer*/
   TESS_DASOUTPUT_BUFF.p_byte_data = &TESS_DASOUTPUT_BUFF.buff.byte0;
   for (TESS_DASOUTPUT_BUFF.filled = 0; TESS_DASOUTPUT_BUFF.filled < ACQ_BUFFER_SIZE; TESS_DASOUTPUT_BUFF.filled++)
   {
      *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = 0;
   }
   TESS_DASOUTPUT_BUFF.filled = 0;
   Set_TessDasStates(DAS_StandBy);
}

static void TESS_FILL_DASOUTPUT_BUFFER(r_buff_data_t* out_buffer)
{
   /* fill buffer with data*/
   TESS_DAS_ADD_SIGNAL(0x55,uint8,1);     /*0*/
   TESS_DAS_ADD_SIGNAL(-0x55,int8,1);     /*1*/
   TESS_DAS_ADD_SIGNAL(TESS_RING_BUFFER.count,uint16,1);     /*2*/
   TESS_DAS_ADD_SIGNAL(TESS_RING_BUFFER.head,uint16,1);     /*4*/
   TESS_DAS_ADD_SIGNAL(TESS_RING_BUFFER.tail,uint16,1);     /*6*/
   TESS_DAS_ADD_SIGNAL(Get_TessDasStates(),uint8,1);     /*8*/
   TESS_DAS_ADD_SIGNAL(-1256,int16,1);     /*9*/
   TESS_DAS_ADD_SIGNAL(-897856,int32,1);     /*11*/
   TESS_DAS_ADD_SIGNAL(TESS_LOOP_CNT,uint32,1); /*15*/
   TESS_DAS_ADD_SIGNAL(1.36F,int32,100); /*19*/
   /*22*/
   /*Reset */
   TESS_DASOUTPUT_BUFF.filled = 0;

}

void TESS_DAS_MAIN()
{
   /*DAS State Machine*/
   switch(Get_TessDasStates())
   {
   case DAS_StartMeas:
      /*Fill DAS buffer with variables*/
      TESS_FILL_DASOUTPUT_BUFFER(&TESS_DATA_ACQ_BUFFER);
      /*Add package to ring buffer*/
      TESS_RING_BUFFER_PUT(&TESS_RING_BUFFER, TESS_DASOUTPUT_BUFF.buff);
      /*Get from rinng buffer and copy package to dma buff*/
      TESS_DMA_BUFFER = TESS_RING_BUFFER_GET(&TESS_RING_BUFFER);
      /*Trigger first transmission, rest will be done by the dma*/
      CDC_Transmit_FS((uint8_t *)&TESS_DMA_BUFFER,ACQ_BUFFER_SIZE);

      TESS_LOOP_CNT++;


      Set_TessDasStates(DAS_KeepPacking);
      break;

   case DAS_KeepPacking:
      /*Fill DAS buffer with variables*/
      TESS_FILL_DASOUTPUT_BUFFER(&TESS_DATA_ACQ_BUFFER);
      /*Add package to ring buffer*/
      TESS_RING_BUFFER_PUT(&TESS_RING_BUFFER, TESS_DASOUTPUT_BUFF.buff);
      TESS_LOOP_CNT++;
      break;
   case DAS_StopOfMeasurement:
      TESS_DAS_INIT();
      Set_TessDasStates(DAS_StandBy);
      break;
   case DAS_StandBy:

   default:
      break;
   }

}

void TESS_DAS_GET_COMMANDS(uint8_t command)
{
   if ((command == DAS_START_MEAS) )
   {
      if (Get_TessDasStates() == DAS_StandBy)
      {
         Set_TessDasStates(DAS_StartMeas);
      }
      else
      {
         /*transition from other states forbidden*/
      }
   }
   else if(command == DAS_STOP_MEAS)
   {
      if ((Get_TessDasStates() == DAS_KeepPacking) || (Get_TessDasStates() == DAS_StartMeas))
      {
         Set_TessDasStates(DAS_StopOfMeasurement);
      }
      else
      {
         /*transition from other states forbidden*/
      }
   }
}

uint8_t TESS_DAS_UPDATE_UPON_TC()
{


   uint8_t  usb_tx_result = USBD_OK;


   if (Get_TessDasStates() == DAS_KeepPacking)
   {
      if (!TESS_RINGBUFF_IS_EMPTY(&TESS_RING_BUFFER) )
      {
         TESS_DMA_BUFFER = TESS_RING_BUFFER_GET(&TESS_RING_BUFFER);

         usb_tx_result =  CDC_Transmit_FS((uint8_t *)&TESS_DMA_BUFFER,ACQ_BUFFER_SIZE);

      }
      else
      {

         /*trigger another transmittion*/
         Set_TessDasStates(DAS_StartMeas);

      }
   }
   return usb_tx_result;
}


void DAS_Receive_Clbk_USB (uint8_t* Buf, uint32_t Len)
{
   uint8_t  rx_char_usb = 0;


   rx_char_usb = Buf[0];
   TESS_DAS_GET_COMMANDS(rx_char_usb);
}

int8_t  DAS_Transmit_Clbk_USB (void)
{

   return TESS_DAS_UPDATE_UPON_TC();
}


static void TESS_DAS_ADD_SIGNAL(float signal, dtype_t dtype, float resolution)
{
   uint32_t  temp32;


   switch (dtype)
   {
   case uint8:
      {
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (uint8_t)signal;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
      }
      break;
   case int8:
      {
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = TESS_SINT8_TO_UINT8((int8_t)signal);
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
      }
      break;
   case uint16:
      {
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = ((uint16_t)(signal)&0x00FF);
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = ((uint16_t)(signal)&0xFF00)>>8;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
      }
      break;
   case int16:
      {
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = ((TESS_SINT16_TO_UINT16(signal))&0x00FF);
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = ((TESS_SINT16_TO_UINT16(signal))&0xFF00)>>8;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
      }
      break;
   case uint32:
      {
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (((uint32_t)(signal))&0x000000FF);
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (((uint32_t)(signal))&0x0000FF00)>>8;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (((uint32_t)(signal))&0x00FF0000)>>16;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (((uint32_t)(signal))&0xFF000000)>>24;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
      }
      break;
   case int32:      /*or float 32*/
      {
         temp32 = (int32_t)(signal*resolution);
         temp32 = TESS_SINT32_TO_UINT32(temp32);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (temp32&0x000000FF);
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (temp32&0x0000FF00)>>8;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (temp32&0x00FF0000)>>16;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
         *(TESS_DASOUTPUT_BUFF.p_byte_data + TESS_DASOUTPUT_BUFF.filled) = (temp32&0xFF000000)>>24;
         TESS_DASOUTPUT_BUFF.filled = TESS_BUFF_IDX_INC(TESS_DASOUTPUT_BUFF.filled, ACQ_BUFFER_SIZE);
      }
      break;
   }


}


#endif

