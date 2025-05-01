#include <dac.h>
#include <platform.h>
#include "LPC407x_8x_177x_8x.h"

//DAC power setting
//IOCON Register
#define IOCON_DAC_PIN_FUNC             ((uint32_t)(1<<1))
#define IOCON_DIGITAL_MODE             ((uint32_t)(1<<7))
#define IOCON_DAC_ENABLE               ((uint32_t)(1<<16))

//DAC control 
//CR Register
//Enable bias: maximum current is 700 uA and maximum frquency is 1MHz
//Disenable bias: maximum current is 350 uA and maximum frquency is 400kHz 
#define DAC_BIAS_EN         ((uint32_t)(1<<16))

//Received value of output at DAC
#define DAC_VALUE(n)        ((uint32_t)((n&0x3FF)<<6))

void dac_init(void) {
	
  //DAC Pin initialisation 
  uint32_t* DAC_Pin = GET_IOCON(P_DAC);
	*DAC_Pin |= IOCON_DAC_PIN_FUNC | IOCON_DIGITAL_MODE | IOCON_DAC_ENABLE; 
	
}

void dac_set(int value) {
	  
  LPC_DAC->CR = DAC_VALUE(value);
	
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
