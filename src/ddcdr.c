#include "H.h"

/*Режим 22/10 - увелечение DXM до 22 бит, и  уменьшение DXC до 10 бит.
Увеличивает смещение адреса >32768 */

#define N 32768         //КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВЕ ПРИЁМНИКА
#define DMA_CH_OUTPUT 8 //НОМЕР DMA ПРИЁМНИКА ( УСТРОЙСТВО -> ПАМЯТЬ )
#define DMA_CH_INPUT  7 //НОМЕР DMA ПЕРЕДАТЧИКА ( ПАМЯТЬ -> УСТРОЙСТВО )

/*ОПРЕДЕЛЕНИЯ ТИПА ПАМЯТИ И ТИПА ПЕРЕДАЧИ*/
static int32_t HAL_DMA_INIT_MEM_TYPE( const void* addr );

__attribute__((aligned(4))) uint32_t otcb[4] ;//DMA_RX
__attribute__((aligned(4))) uint32_t itcb[4] ;//DMA_TX
__attribute__((aligned(4))) uint16_t samples[N];

int main(void)
{
  FILE *ifid ; 
  FILE *ofid ;
  int DMA_CNT,iqcnt,ErrFlag ;
  
  TCB_TYPE_MEMORY = HAL_DMA_INIT_MEM_TYPE(&samples);//ОПРЕДЕЛЕНИЕ ТИПА ИСПОЛЬЗУЕМОЙ ПАМЯТИ
  if(TCB_TYPE_MEMORY == -1){ puts("ERROR TYPE MEMORY - RX"); }
  TCB_TYPE_MEMORY = HAL_DMA_INIT_MEM_TYPE(&samples);//ОПРЕДЕЛЕНИЕ ТИПА ИСПОЛЬЗУЕМОЙ ПАМЯТИ
  if(TCB_TYPE_MEMORY == -1){ puts("ERROR TYPE MEMORY - TX") ; }

  itcb[0] = ((void *)samples);       					          //DI
  itcb[1] = ((N/2) << 16) | 4;         					          //DX
  itcb[2] = 0;                         					          //DY
  itcb[3] = TCB_TYPE_MEMORY | DP_DMA_LENGTH_DATA_128 ;//DP

  otcb[0] = (void *)samples ;       	       //DI
  otcb[1] = 0xFFFF0004 ;        					          //DX
  otcb[2] = 0;                         					          //DY
  otcb[3] = TCB_TYPE_MEMORY | DP_DMA_LENGTH_DATA_128 ;//DP

  HAL_DMA_RqstClr(DMA_CH_OUTPUT);//ОЧИСТКА ИСТОЧНИКА ЗАПРОСА DMA
  HAL_DMA_RqstSet(DMA_CH_OUTPUT, dmaUPDOWN0);//УСТАНОВКА НОВОГО ИСТОЧНИКА ОПРОСА ДЛЯ КАНАЛА DMA_CH_OUTPUT
  HAL_DMA_RqstClr(DMA_CH_INPUT);//ОЧИСТКА ИСТОЧНИКА ЗАПРОСА DMA
  HAL_DMA_RqstSet(DMA_CH_INPUT, dmaUPDOWN0);//УСТАНОВКА НОВОГО ИСТОЧНИКА ОПРОСА ДЛЯ КАНАЛА DMA_CH_INPUT

  DDUC_STEP = 23593 ;            //REG_STEP
  DDUC_CR   = SR_DDUC_ENABLE    |
              SR_FIR_TAP*15     |
              SR_SRC_DR         |
              SR_ROUND_OFF      |
              SR_SATURATION_OFF |
              SR_ROUNDM_OFF     |
              SR_FIR_ORDER3     |
              SR_INPUT_BE       |
              SR_OUTPUT_LE      |
              SR_MODE_DDC       |
              SR_SHIFT*15 ; 

  ifid = fopen("iddc.bin","rb") ; if(ifid == 0){ puts("Unable open iddc.bin.") ; return -1; }
  ofid = fopen("oddc.bin","wb") ; if(ofid == 0){ puts("Unable open oddc.bin.") ; fclose(ifid); return -1;}

/*
  fread((void *)samples,sizeof(uint32_t),2,ifid) ;
  DDUC_STEP = samples[0] ;
  DDUC_CR   = samples[1] ;
*/
  while(fread((void *)samples,sizeof(uint16_t),N,ifid))
  {
    HAL_DMA_Stop( DMA_CH_INPUT ) ;
    HAL_DMA_Stop( DMA_CH_OUTPUT ) ;
    
    HAL_DMA_GetChannelStatusClear( DMA_CH_INPUT ) ;
    HAL_DMA_GetChannelStatusClear( DMA_CH_OUTPUT ) ;
    
    HAL_DMA_WriteDC(DMA_CH_OUTPUT, &otcb) ;
    HAL_DMA_WriteDC(DMA_CH_INPUT,  &itcb) ;
    
    ErrFlag = HAL_DMA_WaitForChannel(DMA_CH_OUTPUT) ;
    if(ErrFlag == 0)	 { printf("OK TX DMA - %i\n",DMA_CNT++); }
    else if(ErrFlag == 1){ puts("ERROR TX DMA") ; return(-1) ;     }
    else if(ErrFlag == 2){ puts("TX DMA OFF")   ; return(-1) ;     }

    iqcnt = HAL_DMA_GetDcCountX(DMA_CH_OUTPUT) ; 
    fwrite((void *)samples,sizeof(uint32_t),(0xFFFF-iqcnt),ofid) ;
  }
  fclose(ifid) ;
  fclose(ofid) ;
  return(0) ;
}
