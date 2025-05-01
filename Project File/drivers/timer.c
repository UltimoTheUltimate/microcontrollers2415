#include <platform.h>
#include <timer.h>

//PCONP power control register
#define PCTIM0 (1UL << 1)
#define PCTIM1 (1UL << 2)
#define PCTIM2 (1UL << 22)
#define PCTIM3 (1UL << 23)

//Increment timer 0 every 100uS
#define PRESCALEVALUE                   (100)
//Interrupt triggered every n second
#define MATCHVALUE(n)                   (10000*n)
//Set Match Register n
#define TIM_MCR_CHANNEL_SET(n)      ((uint32_t)(3<<(n*3)))


static void (*timer_callback)(void) = 0;

//Using timer 0
void timer_init(uint32_t period) {
	
	// Enable power
	LPC_SC -> PCONP |= PCTIM0;
	
	//Set to timer default mode
	LPC_TIM0 -> CTCR = 0;
  LPC_TIM0 -> TC =0;
  LPC_TIM0 -> PC =0;
  LPC_TIM0 -> PR =0;		
  LPC_TIM0->TCR |= (1<<1);  //Reset Counter
  LPC_TIM0->TCR &= ~(1<<1); //release reset
	
  // Initialize timer 0
	LPC_TIM0 -> PR = period/4;  //By default PCLK/4
	// Clear interrupt pending
	LPC_TIM0 -> IR = 0xFFFFFFFF;
			
}

void timer_enable(void) {
	
	LPC_TIM0 -> TCR |= 1;
  LPC_TIM0 -> PC =0;   //the offest of timer is 12 - 13
	
}

void timer_disable(void) {
	
  LPC_TIM0 -> TCR = 0;
	
}

void timer_set_callback(void (*callback)(void)) {
	
	timer_callback = callback;
	
	//Set Match 0 register
  LPC_TIM0 -> MR0 = 1;
	LPC_TIM0 -> MCR |= TIM_MCR_CHANNEL_SET(0);
	
	//Enable interrupt for timer 0
  NVIC_SetPriority(TIMER0_IRQn, 2);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
  NVIC_EnableIRQ(TIMER0_IRQn);
	__enable_irq();

}

void TIMER0_IRQHandler(void){
	
	if ( ((LPC_TIM0 -> IR) & (0x1)) != 0 )
    {
			timer_callback();
			// Clear interrupt pending
			LPC_TIM0 -> IR = 0x1;
		}
	
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
