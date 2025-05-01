#include <platform.h>
#include <i2c.h>

#define I2C_CONCLR_AAC(n)        (n<<2)
#define I2C_CONCLR_SIC(n)        (n<<3)
#define I2C_CONCLR_STOC(n)       (n<<4)
#define I2C_CONCLR_STAC(n)       (n<<5)
#define I2C_CONCLR_I2ENC(n)      (n<<6)

#define I2C_CONSET_AA(n)         (n<<2)
#define I2C_CONSET_SI(n)         (n<<3)
#define I2C_CONSET_STO(n)        (n<<4)
#define I2C_CONSET_STA(n)        (n<<5)
#define I2C_CONSET_I2EN(n)       (n<<6)

#define X          (0)
#define Y          (1)

//Master transmit mode
//A start condition has been transmitted
#define I2C_I2STAT_M_TX_START                   ((0x08))
//A repeat start condition has been transmitted 
#define I2C_I2STAT_M_TX_RESTART                 ((0x10))
//SLA+W has been transmitted, ACK has been received 
#define I2C_I2STAT_M_TX_SLAW_ACK                ((0x18))
//SLA+W has been transmitted, NACK has been received 
#define I2C_I2STAT_M_TX_SLAW_NACK               ((0x20))
//Data has been transmitted, ACK has been received 
#define I2C_I2STAT_M_TX_DAT_ACK                 ((0x28))
//Data has been transmitted, NACK has been received 
#define I2C_I2STAT_M_TX_DAT_NACK                ((0x30))
//Arbitration lost in SLA+R/W or Data bytes 
#define I2C_I2STAT_M_TX_ARB_LOST                ((0x38))

//Master receive mode
//A start condition has been transmitted 
#define I2C_I2STAT_M_RX_START                   ((0x08))
//A repeat start condition has been transmitted
#define I2C_I2STAT_M_RX_RESTART                 ((0x10))
//Arbitration lost 
#define I2C_I2STAT_M_RX_ARB_LOST                ((0x38))
//SLA+R has been transmitted, ACK has been received
#define I2C_I2STAT_M_RX_SLAR_ACK                ((0x40))
//SLA+R has been transmitted, NACK has been received
#define I2C_I2STAT_M_RX_SLAR_NACK               ((0x48))
//Data has been received, ACK has been returned
#define I2C_I2STAT_M_RX_DAT_ACK                 ((0x50))
//Data has been received, NACK has been return
#define I2C_I2STAT_M_RX_DAT_NACK                ((0x58))

#define I2C_I2STAT_M_BUS_ERROR                  ((0x00))

#define READ 0x1
#define WRITE 0x0

void I2C_Mode_Set(uint8_t STA, uint8_t STO, uint8_t SI, uint8_t AA);
void I2C_Mode_Clr(uint8_t STA, uint8_t STO, uint8_t SI, uint8_t AA);

void i2c_init() {
	
	uint32_t I2C_CLK;
	uint32_t* I2C_SDA_PIN = GET_IOCON(P_SDA);
	uint32_t* I2C_SCL_PIN = GET_IOCON(P_SCL);
	
	LPC_SC -> PCONP |= (1<<7); //EnableI2C0
	*I2C_SDA_PIN |= 1;
	*I2C_SCL_PIN |= 1;
	
	//Set up I2C rate
	I2C_CLK = CLK_FREQ / 400000; //100kHz
	LPC_I2C0->SCLH = I2C_CLK / 2;
	LPC_I2C0->SCLL = I2C_CLK - LPC_I2C0->SCLH ;
	
	LPC_I2C0->CONCLR = 0; //Reset I2C0
	
	LPC_I2C0->CONSET = 0x40;// Enable the i2c peripheral
	// Set up the GPIO pins, the clock, the i2c peripheral
	// and enables.
}

void i2c_write(uint8_t address, uint8_t *buffer, int buff_len) {
	uint32_t i = 0;
	LPC_I2C0->CONSET |= 0x20;
	start:	switch(LPC_I2C0->STAT)
	{
		case I2C_I2STAT_M_TX_START:
		case I2C_I2STAT_M_TX_RESTART:
			LPC_I2C0->DAT = address | WRITE;
			I2C_Mode_Clr(Y,0,0,Y);
			goto start;
		
		case I2C_I2STAT_M_TX_SLAW_ACK:
			LPC_I2C0->DAT = buffer[i];
			I2C_Mode_Clr(0,0,0,Y);
				i++;
			goto start;
		
		case I2C_I2STAT_M_TX_SLAW_NACK:;
			I2C_Mode_Set(1,1,X,X);
			I2C_Mode_Clr(Y,Y,0,Y);
			goto start;
			
		case I2C_I2STAT_M_TX_DAT_ACK:
			if(i <= buff_len -1){
		  LPC_I2C0->DAT = buffer[i];
			I2C_Mode_Clr(0,0,0,Y);
				i++;
			goto start;
			}
			else{
			I2C_Mode_Set(1,X,X,X);
			I2C_Mode_Clr(Y,0,0,Y);
			break;
			}
		
		case I2C_I2STAT_M_TX_DAT_NACK:
			I2C_Mode_Set(1,X,X,X);
			I2C_Mode_Clr(Y,Y,0,Y);
		break;
		
		case I2C_I2STAT_M_TX_ARB_LOST:
			I2C_Mode_Set(1,X,X,X);
			I2C_Mode_Clr(Y,0,0,Y);
		  goto start;
		
		case I2C_I2STAT_M_BUS_ERROR:
			I2C_Mode_Set(X,1,X,X);
			I2C_Mode_Clr(0,Y,0,Y);
			LPC_I2C0->CONSET |= 0x20;
      goto start;
		
		default:
			goto start;;		
	}
	
	// Send the following sequence:
	//  - Start bit
	//  - Contents of buffer, from 0..buff_len
	//  - Stop bit
}

void i2c_read(uint8_t address, uint8_t *buffer, int buff_len) {
	
	uint32_t i = 0;
	LPC_I2C0->CONSET |= 0x20;
	start:	switch(LPC_I2C0->STAT)
	{
		case I2C_I2STAT_M_RX_START:
		case I2C_I2STAT_M_RX_RESTART:
			LPC_I2C0->DAT = address | READ;
			I2C_Mode_Clr(Y,0,0,Y);
			goto start;
		
		case I2C_I2STAT_M_RX_SLAR_ACK:
		  if(buff_len>1) {
				I2C_Mode_Set(X,X,X,1);
				I2C_Mode_Clr(0,0,0,Y);}
			else{
				I2C_Mode_Clr(0,0,0,0);
			}
		goto start;
		
		case I2C_I2STAT_M_RX_SLAR_NACK:
				I2C_Mode_Set(1,1,X,X);
				I2C_Mode_Clr(Y,Y,0,Y);
	  goto start;
		
		case I2C_I2STAT_M_RX_DAT_ACK:
			buffer[i] = LPC_I2C0->DAT;
		  i++;
		  if(i == buff_len-1){
				I2C_Mode_Clr(0,0,0,0);
			}
			else{
				I2C_Mode_Set(X,X,X,1);
				I2C_Mode_Clr(0,0,0,Y);
			}
		goto start;		
		
		case I2C_I2STAT_M_RX_DAT_NACK:
			buffer[i] = LPC_I2C0->DAT;
				I2C_Mode_Set(1,X,X,X);
				I2C_Mode_Clr(Y,0,0,Y);
		break;
		
		case I2C_I2STAT_M_BUS_ERROR:
			I2C_Mode_Set(X,1,X,X);
			I2C_Mode_Clr(0,Y,0,Y);
			LPC_I2C0->CONSET |= 0x20;
    goto start;
		
		default:
		goto start;		
	}
	// Read with the following sequence:
	//  - Start bit
	//  - Contents of buffer, from 0..buff_len, sending a NACK
	//    for the last item and an ACK otherwise.
	//  - Stop bit
}

void I2C_Mode_Set(uint8_t STA, uint8_t STO, uint8_t SI, uint8_t AA){
	
		LPC_I2C0->CONSET = I2C_CONSET_STA(STA) | I2C_CONSET_STO(STO) | I2C_CONSET_SI(SI)| I2C_CONSET_AA(AA);
}

void I2C_Mode_Clr(uint8_t STA, uint8_t STO, uint8_t SI, uint8_t AA){
	  LPC_I2C0->CONCLR = I2C_CONCLR_STAC(!STA) | I2C_CONCLR_STOC(!STO) | I2C_CONCLR_SIC(!SI)| I2C_CONCLR_AAC(!AA);
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
