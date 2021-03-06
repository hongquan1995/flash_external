/*
 * SPI.h
 *
 *  Created on: Nov 4, 2021
 *      Author: 0
 */

#ifndef FLASH_EX_SPI_H_
#define FLASH_EX_SPI_H_


HAL_StatusTypeDef SPI_Transmit(uint8_t* send_buf, uint16_t size);
HAL_StatusTypeDef SPI_Receive(uint8_t* recv_buf, uint16_t size);
HAL_StatusTypeDef SPI_TransmitReceive(uint8_t* send_buf, uint8_t* recv_buf, uint16_t size);

#endif /* FLASH_EX_SPI_H_ */
