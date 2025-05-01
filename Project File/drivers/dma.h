/*!
 * \file      dma.h
 * \brief     Controller for DMA pheriperal 
 *            
 * \copyright ARM University Program &copy; ARM Ltd 2014.
 */
#ifndef DMA_H
#define DMA_H


#define PING 0x00
#define PONG 0x01
#define DMA_BUFFER_SIZE 128  


/*! \brief Initialises the DMA pheriperal module 
 */
void dma_init(void);

/*! \brief Setup DMA channel peripheral according to the specified parameters 
 */

void dma_setup(char ChannelNum, 
							 unsigned int SrcMemAddr,
							 unsigned int DstMemAddr,
							 unsigned int SrcPeriph,
							 unsigned int DstPeriph,
							 unsigned int TransferSize,
							 unsigned int BurstSize,
							 unsigned int TransferWidth,
							 unsigned int TransferType,
							 unsigned int Dmalli  );

/*! \brief Enables the DMA chanel. */
void dma_enable(unsigned char ChannelNum);							 

/*! \brief Disables the DMA chanel. */							 
void dma_disable(unsigned char ChannelNum);					 

/*! \brief Reads the status of the interrupts
 *  \return 1=DMA channel interrupt request	is active */							 
unsigned int dma_state(unsigned char ChannelNum);								 

/*! \brief Clean the interrupt requests. */							 
void dma_clean(unsigned char ChannelNum);		

/*! \brief Rewrite the memory source address */							 
void dma_src_memory(unsigned char ChannelNum, unsigned int address);		

/*! \brief Rewrite the memory destination address */							 
void dma_dest_memory(unsigned char ChannelNum, unsigned int address);			

/*! \brief Write the data transfer size. */							 
void dma_transfersize(unsigned char ChannelNum, unsigned int size);		

/*! \brief Pass a callback to the API, which is executed during the
 *         interrupt handler.
 *  \param callback  Callback function.
 */
void dma_set_callback(void (*callback)(void));

#endif //DMA_H
