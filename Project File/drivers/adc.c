#include <platform.h>
#include <adc.h>

//ADC power control
//PCONP
#define ADC_POWER_EN                   ((uint32_t)((1)<<12)) //Enable PCADC

//IOCON Register
#define IOCON_DIGITAL_MODE             ((uint32_t)(1<<7))
#define IOCON_ADC_PIN_FUNC_1           ((uint32_t)(1))       //For ADC0[0-3]   

//CR
#define ADC_PDN                  ((uint32_t)((1)<<21)) 
#define ADC_START                ((uint32_t)((1)<<24)) 
#define ADC_PORT_SELECT(n)        ((uint32_t)((1)<<n))

#define ADC_SAMPLING_FREQUENCY       (400000)                 //400kHz
#define ADC_VREF                     (3.3)

uint8_t GET_ADC0_Port(Pin pin){
	
	uint8_t ADC0_Pin_num;
	if(GET_PORT_INDEX(pin)) {ADC0_Pin_num = GET_PIN_INDEX(pin) - 26;}
	else {ADC0_Pin_num = GET_PIN_INDEX(pin) - 23;}
	
	return ADC0_Pin_num;

}

void adc_init(void) {
	
  uint32_t temp;
	uint32_t* ADC0_Port =GET_IOCON(P_ADC);
	
  LPC_SC -> PCONP |= ADC_POWER_EN; //Enable power output to ADC
	
	//Enable ADC 
	*ADC0_Port |= IOCON_ADC_PIN_FUNC_1; 
	*ADC0_Port &= ~IOCON_DIGITAL_MODE;
	
	LPC_ADC -> CR = 0;
	
	//Define APB clock
	temp = ADC_SAMPLING_FREQUENCY * 31;
	temp = (PeripheralClock * 2 + temp) / (2 * temp) - 1;
	LPC_ADC -> CR |=  (temp<<8);
	
	LPC_ADC -> CR |= ADC_PORT_SELECT(GET_ADC0_Port(P_ADC)) | ADC_PDN; // ADC pre-setting

}

int adc_read(void) {
	
	uint32_t data;
	
	LPC_ADC -> CR |= ADC_START; //Start conversion
	
	while( !(LPC_ADC->DR[GET_ADC0_Port(P_ADC)] & (1UL<<31)) );//wait until the conversion completes
	LPC_ADC -> CR &= ~ADC_START;
	
	data = ((LPC_ADC->DR[GET_ADC0_Port(P_ADC)] >> 4) ) & 0xFFF;
	return data;

}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
