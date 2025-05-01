/*!
 * \file      i2s.h
 * \brief     Controller for pheriperal I2S module, configured
 *            as a slave.
 * \copyright ARM University Program &copy; ARM Ltd 2014.
 */
#ifndef I2S_H
#define I2S_H

/*! \brief Initialises the pheriperal I2S module in slave mode, and any
 *         relevant pins.
 */
void i2s_init(void);

/*! \brief //Enable I2S Receive Interrupt. */
void i2s_rx_irq_enable(void);

/*! \brief //Enable I2S Receive Interrupt. */
void i2s_tx_irq_enable(void);

/*! \brief FIFO level on wich a RX irq request is created
 *  \param d depth
 */
void i2s_rx_depth_irq(int d);

/*! \brief FIFO level on wich a TX irq request is created
 *  \param d depth
 */
void i2s_tx_depth_irq(int d);

/*! \brief Enable I2S receive DMA request. */
void i2s_rx_dma1_enable(void);

/*! \brief Enable I2S receive DMA request. */
void i2s_tx_dma1_enable(void);

/*! \brief FIFO level on wich irq request is created.
 *  \param d depth
 */
void i2s_rx_depth_dma1(int d);  

/*! \brief FIFO level on wich irq request is created.
 *  \param d depth
 */
void i2s_tx_depth_dma1(int d);  

/*! \brief Enable I2S receive DMA request. */
void i2s_rx_dma2_enable(void);

/*! \brief Enable I2S receive DMA request. */
void i2s_tx_dma2_enable(void);

/*! \brief FIFO level on wich irq request is created.
 *  \param d depth
 */
void i2s_rx_depth_dma2(int d);  

/*! \brief FIFO level on wich irq request is created.
 *  \param d depth
 */
void i2s_tx_depth_dma2(int d);  

/*! \brief Transmits 32 bits to i2s TX FIFO.
 *  \param c Character to send.
 */
void i2s_tx(unsigned int c);

/*! \brief Receives 32 bits data from i2s RX FIFO.
 *  \return Received data.
 */
unsigned int i2s_rx(void);

/*! \brief Clear STOP, RESET and MUTE bit */
void i2s_start(void);

/*! \brief STOP RESET and MUTE I2S */
void i2s_stop(void);

/*! \brief Pass a callback to the API, which is executed during the
 *         interrupt handler.
 *  \param callback  Callback function.
 */
void i2s_set_callback(void (*callback)(void));

#endif //I2S_H
