/*
 * SPI.c
 *
 *  Created on: Nov 4, 2021
 *      Author: 0
 */

#include "stm32l4xx_hal.h"
#include "stdint.h"
#include "SPI.h"

extern SPI_HandleTypeDef hspi1;
/**
 * @brief    SPI Send data of specified length
 * @param    buf  ——  Send data buffer first address
 * @param    size ——  The number of bytes to send data
 * @retval    Successfully returns HAL_OK
 */
HAL_StatusTypeDef SPI_Transmit(uint8_t* send_buf, uint16_t size)
{
    return HAL_SPI_Transmit(&hspi1, send_buf, size, 100);
}

/**
 * @brief   SPI Receive data of specified length
 * @param   buf  ——  Receive data buffer first address
 * @param   size ——  The number of bytes to receive data
 * @retval   Successfully returns HAL_OK\
 */
HAL_StatusTypeDef SPI_Receive(uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_Receive(&hspi1, recv_buf, size, 100);
}

/**
 * @brief   SPI Receive data of specified length while sending data
 * @param   send_buf  ——  Receive data buffer first address
 * @param   recv_buf  ——  Receive data buffer first address
 * @param   size ——  To send / The number of bytes of data received
 * @retval   Successfully returns HAL_OK
 */
HAL_StatusTypeDef SPI_TransmitReceive(uint8_t* send_buf, uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_TransmitReceive(&hspi1, send_buf, recv_buf, size, 1000);
}
