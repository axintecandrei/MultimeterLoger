/**
  ******************************************************************************
  * File Name          : tess_data_acq_output.h
  * Author			   : Axinte Andrei
  * Description        : IO pin mode setting header file
  ******************************************************************************
**/
#ifndef __tess_data_acq_output_h__
#define __tess_data_acq_output_h__


#include "config.h"
#include "tess_ring_buffer.h"
#include "usbd_cdc_if.h"
/*Include the needed files so the DAS
 * can have access to the variable
 * to be send during measurement*/



/*End Include*/

#if CFG_ACQ_ON

/*DEFINES*/
#define ACQ_BUFFER_SIZE 6

#define ACQ_HEADER      0x55
#define ACQ_FOOTER      0x77

#define DAS_START_MEAS  0x31
#define DAS_STOP_MEAS   0x30
#define DAS_ACK         0x35

#define TESS_SINT8_TO_UINT8(x)   ((uint8_t)( (x) + (128)))
#define TESS_SINT16_TO_UINT16(x) ((uint16_t)( (x) + (32768)))
#define TESS_SINT32_TO_UINT32(x) ((uint32_t)( (x) + 2147483648))

typedef enum tess_das_states_e
{
   DAS_StandBy,
   DAS_SendPackage,
   DAS_StartMeas,
   DAS_KeepPacking,
   DAS_StopOfMeasurement,
}tess_das_states_t;

typedef enum dtype_e
{
   int8 = 0,
   uint8 = 1,
   uint16 = 2,
   int16 = 3,
   uint32 = 4,
   int32 = 5,
}dtype_t;

typedef struct tess_das_output_buff_s
{
   uint8_t filled;
   uint8_t* p_byte_data;
   r_buff_data_t buff;
}tess_das_output_buff_t;


/*VARIABLES*/
tess_das_states_t  PORT__TessDasStates;
#define Set_TessDasStates(v) (PORT__TessDasStates = (v))
#define Get_TessDasStates()   (*((const tess_das_states_t*) &PORT__TessDasStates))

uint16_t  PORT__TessDasADC_CH_1;
#define Set_TessDasADC_CH_1(v) (PORT__TessDasADC_CH_1 = (v))
#define Get_TessDasADC_CH_1()   (*((const uint16_t*) &PORT__TessDasADC_CH_1))

uint16_t  PORT__TessDasInputCapture;
#define Set_TessDasInputCapture(v)   (PORT__TessDasInputCapture = (v))
#define Get_TessDasInputCapture()   (*((const uint16_t*) &PORT__TessDasInputCapture))

int16_t  PORT__TessDasSpeed;
#define Set_TessDasSpeed(v) (PORT__TessDasSpeed = (v))
#define Get_TessDasSpeed()   (*((const int16_t*) &PORT__TessDasSpeed))


r_buff_data_t           TESS_DATA_ACQ_BUFFER;
r_buff_data_t           TESS_DMA_BUFFER;
tess_das_output_buff_t  TESS_DASOUTPUT_BUFF;
uint32_t                TESS_LOOP_CNT;
extern uint32_t         dma_isr_cnt, ring_buff_cnt;
/*FUNCTIONS*/
void TESS_DAS_INIT();
void TESS_DAS_MAIN();
uint8_t TESS_DAS_UPDATE_UPON_TC();
void TESS_DAS_GET_COMMANDS(uint8_t command);

#endif /*CFG_ACQ_ON*/
#endif /*__tess_data_acq_output_h__*/

