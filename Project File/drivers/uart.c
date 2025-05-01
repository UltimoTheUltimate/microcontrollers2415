#include <platform.h>
#include <uart.h>


//LCR
#define UART_LCR_DLAB_EN        ((uint8_t)(1<<7))
#define UART_LCR_WLS(n)         ((uint8_t)(n-5))  //n=5,6,7,8

/** Line status register: Receive data ready*/
#define UART_LSR_RDR        ((uint8_t)(1<<0))
/** Line status register: Overrun error*/
#define UART_LSR_OE         ((uint8_t)(1<<1))
/** Line status register: Parity error*/
#define UART_LSR_PE         ((uint8_t)(1<<2))
/** Line status register: Framing error*/
#define UART_LSR_FE         ((uint8_t)(1<<3))
/** Line status register: Break interrupt*/
#define UART_LSR_BI         ((uint8_t)(1<<4))
/** Line status register: Transmit holding register empty*/
#define UART_LSR_THRE       ((uint8_t)(1<<5))
/** Line status register: Transmitter empty*/
#define UART_LSR_TEMT       ((uint8_t)(1<<6))
/** Error in RX FIFO*/
#define UART_LSR_RXFE       ((uint8_t)(1<<7))


// UART FIFO 
#define UART_FCR_FIFO_EN        (1<<0)
#define UART_FCR_RX_RS          (1<<1)
#define UART_FCR_TX_RS          (1<<2)

// Transmit enable bit 
#define UART_TER_TXEN           ((uint8_t)(1<<7))
#define UART_TER_BITMASK        ((uint8_t)(0x80))

// Interrupt Enable Register
#define UART_IER_RBRIE          ((uint8_t)(1<<0))
#define UART_IER_THREIE         ((uint8_t)(1<<1))
#define UART_IER_RXIE           ((uint8_t)(1<<2))

#define PCUART2                 ((uint8_t )(1<<24))
#define PCUART0                 ((uint8_t )(1<<3))

float Fractional_Divider_Array [72][3];

static void (*UART_callback)(uint8_t);

void FR_TABLE_Value(float input_array[3], float FR, float Div, float Mul);
void FR_TABLE_Construct(void);
void uart_set_baudrate(uint32_t baud);

void uart_init(uint32_t baud) {
	
	//Use UART 3
	//Set IOCON
	uint32_t* _UART_TX =GET_IOCON(P_TX);
	uint32_t* _UART_RX =GET_IOCON(P_RX);
	*_UART_TX &= ~7;
	*_UART_RX &= ~7;
	*_UART_TX |= 4;
	*_UART_RX |= 4;
	
	LPC_SC->PCONP |= PCUART0;   //Enable power output for UART
	
	//Reset UART
	LPC_UART0->IER = 0;// Disable interrupt
	LPC_UART0->FCR = 0;// Disable FIFO
  LPC_UART0->LCR = 0;// Reset LCR 
	LPC_UART0->SCR = 0;// Reset SCR
  LPC_UART0->ACR = 0;// Reset ACR
  LPC_UART0->TER = 0;// Disable Tx	
	
	//Initialize FIFO
	LPC_UART0->FCR = UART_FCR_RX_RS | UART_FCR_TX_RS;
	//Initialize UART
	LPC_UART0->LCR = UART_LCR_WLS(8) | UART_LCR_DLAB_EN;//Set LCR
	uart_set_baudrate(baud);//Set Baud Rate
	LPC_UART0->LCR &= ~UART_LCR_DLAB_EN;
	LPC_UART0->FCR = UART_FCR_FIFO_EN;

}

void uart_set_baudrate(uint32_t baudrate){
	
	uint32_t BAUD_DIVADDVAL, BAUD_MULVAL, BAUD_DLEST;
	float DLEST, FREST,upper_bound,lower_bound, UART_CLK;
	int int_DLEST;
	int table_item = 35;
	
	upper_bound = 1.9;
	lower_bound = 1.1;
	
	UART_CLK = CLK_FREQ;
	DLEST = UART_CLK/(16 * baudrate);
	int_DLEST = DLEST;
	
	if(!(DLEST - int_DLEST)){
		
		BAUD_DLEST = int_DLEST;
		BAUD_DIVADDVAL = 0;
		BAUD_MULVAL = 1;
		LPC_UART0-> FDR |= (BAUD_DIVADDVAL <<0) | ( BAUD_MULVAL <<4);
		LPC_UART0-> DLL |= (BAUD_DLEST & 0xFF);
		LPC_UART0-> DLM |= ((BAUD_DLEST & 0xFF00)>>8);

	}
  else {
		
	  FR_TABLE_Construct();
		FREST = 1.5;
		int_DLEST = DLEST / FREST;
		FREST = UART_CLK/(16 * baudrate * int_DLEST);
		
		while(FREST < lower_bound) {
			
			table_item ++;
      FREST	= Fractional_Divider_Array[table_item][0];
			int_DLEST = UART_CLK/(16 * baudrate * FREST);
			FREST = UART_CLK/(16 * baudrate * int_DLEST);
			
			if(table_item > 71) while(1); //error
		}
		
		while(FREST > upper_bound) {
			
			table_item --;
      FREST	= Fractional_Divider_Array[table_item][0];
			int_DLEST = UART_CLK/(16 * baudrate * FREST);
			FREST = UART_CLK/(16 * baudrate * int_DLEST);
			
			if(table_item < 0) while(1); //error
		}
		
		BAUD_DIVADDVAL = Fractional_Divider_Array[table_item][1];
		BAUD_MULVAL = Fractional_Divider_Array[table_item][2];
		BAUD_DLEST = int_DLEST;
		
		LPC_UART0-> FDR |= (BAUD_DIVADDVAL <<0) | ( BAUD_MULVAL <<4);
		LPC_UART0-> DLM = ((BAUD_DLEST & 0xFF00)>>8);
		LPC_UART0-> DLL = (BAUD_DLEST & 0x00FF);
	
	}
	
}

void uart_enable(void) {
	// Enable the peripheral.
	LPC_UART0 -> TER |= UART_TER_TXEN;
}

void uart_print(char *string) {
	while(*string) {
		uart_tx(*string++);
	}
}

void uart_set_rx_callback(void (*callback)(uint8_t)) {
	
	LPC_UART0-> IER  |= UART_IER_RBRIE | UART_IER_THREIE | UART_IER_RXIE; //Enable RBRIR and RXIE
	UART_callback = callback;
	
	NVIC_SetPriority(UART0_IRQn, 3);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	__enable_irq();
}

void UART0_IRQHandler(void){
	
	switch(LPC_UART0->IIR>>1 & 0x7){
		case 0x3:
			while(1)
		;//error
			
		case 0x2:
		case 0x6:
			LPC_UART0-> IER  &= ~(UART_IER_RBRIE); //Temporarily disable interrupters RBR
	    UART_callback(uart_rx());
			LPC_UART0-> IER  |= (UART_IER_RBRIE);
	  break;//Receive data
		
		default:
	  break;		
    }
  
}

void uart_tx(uint8_t c) {
	
	while( !((LPC_UART0-> LSR >>5) & 0x1) ) //Check UnTHR is empty
			;
	LPC_UART0->THR = c & 0xFF;
	// Blocks until the peripheral is ready to receive data,
	// and then sends a new byte, c, to the UART peripheral.
}

uint8_t uart_rx(void) {
	
	// Blocks until the peripheral has a received character
	while( !(LPC_UART0-> LSR & 0x1) ) 
			;
	
  return (LPC_UART0->RBR & 0xFF);
}

void FR_TABLE_Construct(void){
	
	uint32_t i =0;
	
	FR_TABLE_Value(Fractional_Divider_Array[i],     1.000,0,1);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.067,1,15);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.071,1,14);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.077,1,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.083,1,12);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.091,1,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.100,1,10);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.111,1,9);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.125,1,8);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.133,2,15);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.143,1,7);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.154,2,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.167,1,6);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.182,2,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.200,1,5);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.214,3,14);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.222,2,9);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.231,3,13);
	
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.250,1,4);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.267,4,15);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.273,3,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.286,2,7);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.300,3,10);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.308,4,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.333,1,3);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.357,5,14);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.364,4,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.375,3,8);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.385,5,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.400,2,5);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.417,5,12);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.429,3,7);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.444,4,9);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.455,5,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.462,6,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.467,7,15);
	
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.500,1,2);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.533,8,15);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.538,7,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.545,6,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.556,5,9);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.571,4,7);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.583,7,12);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.600,3,5);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.615,8,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.625,5,8);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.636,7,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.643,9,14);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.667,2,3);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.692,9,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.700,7,10);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.714,5,7);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.727,8,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.733,11,15);
	
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.750,3,4);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.769,10,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.778,7,9);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.786,11,14);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.800,4,5);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.818,9,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.833,5,6);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.846,11,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.857,6,7);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.867,13,15);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.875,7,8);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.889,8,9);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.900,9,10);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.909,10,11);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.917,11,12);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.923,12,13);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.929,13,14);
	FR_TABLE_Value(Fractional_Divider_Array[i++],   1.933,14,15);




}

void FR_TABLE_Value(float input_array[3], float FR, float Div, float Mul){

	input_array[0] = FR;
	input_array[1] = Div;
	input_array[2] = Mul;

}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
