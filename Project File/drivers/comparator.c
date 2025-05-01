#include <platform.h>
#include <comparator.h>

#define TRIG_MODE_MASK   (0x7 << 16 )

static void (*CMP_callback)(int status);

void comparator_init(void) {
	
	uint32_t i;
	
	uint32_t* Pin_VP =GET_IOCON(P_CMP_PLUS);
	uint32_t* Pin_VM =GET_IOCON(P_CMP_NEG);
	
	LPC_SC->PCONP1 |= (1 << 3);       //power on the comparator
	
	LPC_SC->RSTCON1 |= (1 << 3);      // reset the comparator
  LPC_SC->RSTCON1 &= ~(1 << 3);
	
	*Pin_VP   &= ~0x9F;
	*Pin_VM   &= ~0x9F;
	*Pin_VP   |= 5;
	*Pin_VM   |= 5;
	
	LPC_COMPARATOR->CTRL &= ~(0x3 << 0);
	LPC_COMPARATOR->CTRL |= (0x3 << 0); //Enable current source
	LPC_COMPARATOR->CTRL &= ~(0x3 << 2);
	LPC_COMPARATOR->CTRL |= (0x3 << 2); //Enable bandgap control
	
	for (i = 0; i < 0x80; i++); //wait to stablize
	
	//Set external inputs
	LPC_COMPARATOR->CTRL1 &= ~(0x3 << 0);
	LPC_COMPARATOR->CTRL1 |= (0x3 << 0);
	
	//VP  CMP1_IN[2]
	LPC_COMPARATOR->CTRL1 &= ~(7 << 8);
  LPC_COMPARATOR->CTRL1 |= (0x3 << 8);   
	
	//VM  CMP1_IN[3]
	LPC_COMPARATOR->CTRL1 &= ~(7 << 4);
  LPC_COMPARATOR->CTRL1 |= (0x4 << 4);
	
	LPC_COMPARATOR->CTRL1 &= ~(0x4 << 13);//No hysteresis

	LPC_COMPARATOR->CTRL1 &= ~(1 << 12);

  for (i = 0; i < 0x80; i++); //wait to stablize
	
	LPC_COMPARATOR->CTRL1 |= (1<<19) | (1<<2);
	
}

void comparator_set_callback(void (*callback)(int state)) {
	
	CMP_callback = callback; 
	
	NVIC_SetPriority(CMP1_IRQn, 3);
	NVIC_ClearPendingIRQ(CMP1_IRQn);
	NVIC_EnableIRQ(CMP1_IRQn);
	__enable_irq();
}

int comparator_read(void) {
	
	uint32_t i;
	uint32_t CMP_status = 0;
	for (i = 0; i < 0x80; i++); //wait to stablize
	CMP_status = ( LPC_COMPARATOR->CTRL1 & (1 << 3) ) >> 3;
	
	return CMP_status;
}

void comparator_set_trigger(ComparatorTriggerMode trig) {

	LPC_COMPARATOR->CTRL1 &= ~TRIG_MODE_MASK;
	
	switch(trig){
		case CompNone:
      NVIC_DisableIRQ(CMP1_IRQn);			
			break;
		case CompRising:
			LPC_COMPARATOR->CTRL1 |= (0x1 << 17);
			break;
		case CompFalling:
			LPC_COMPARATOR->CTRL1 &= ~(0x3 << 17);
			break;
		case CompBoth:
			LPC_COMPARATOR->CTRL1 |= (0x2 << 17);
			break;
		default:
			break;
	}
	
}

void CMP1_IRQHandler(void){
	int CMP_IRQ_status = (((LPC_COMPARATOR->CTRL1) & (0x1 << 19)) >> 19);
	
	//Clear interrupt pending
	LPC_COMPARATOR->CTRL1 |= (0x1 << 19);
	
	if(CMP_IRQ_status) CMP_callback(comparator_read());
	
}
