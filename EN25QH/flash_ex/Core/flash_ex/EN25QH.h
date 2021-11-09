/*
 * EN25QH.h
 *
 *  Created on: Nov 4, 2021
 *      Author: 0
 */

#ifndef FLASH_EX_EN25QH_H_
#define FLASH_EX_EN25QH_H_

#define LOW    	 			0x00  /* Chip Select line low */
#define HIGH    			0x01  /* Chip Select line high */

/* ID command EN25QH */
#define EN25QH_W_STATUS      0x01
#define EN25QH_WRITE         0x02
#define EN25QH_READ          0x03
#define EN25QH_READ_STATUS   0x05
#define EN25QH_WREN          0x06
#define EN25QH_SECTOR_ERASE  0x20
#define EN25QH_BLOCK_ERASE   0xD8
#define EN25QH_CHIP_ERASE    0xC7
#define EN25QH_JEDEC         0x9F  // 0x90
#define EN25QH_WRITE_DISABLE 0x04

#define WIP_FLAG           				0x01  /* Write In Progress (WIP) flag */
#define WEL_Flag   		    			0x02							/* Chua co check Flag WEL */
#define DUMMY_BYTE 		    			0xA5  /* Byte temp */
#define Flash_EN25QH_PAGESIZE       		0x100

#define EN25QH_SC_PORT 						GPIOC   			// PORT
#define EN25QH_SC_PIN 						GPIO_PIN_4   		// PIN Select chip

#define FLASH_EN25QH_ID         			0x1C7018

void Flash_EN25QH_ChipSelect(uint8_t State);
uint8_t Flash_EN25QH_Send_Byte(uint8_t byte);
uint32_t Flash_EN25QH_Connect(void);
void Flash_EN25QH_Enable(void);
void Flash_EN25QH_WaitForWriteEnd(void);
void Flash_EN25QH_EraseSector(uint32_t SectorAddr);
void Flash_EN25QH_EraseBulk(void);
void Flash_EN25QH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void Flash_EN25QH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void Flash_EN25QH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void Flash_EN25QH_StartReadSequence(uint32_t ReadAddr);
uint8_t Flash_EN25QH_ReadByte(void);

#endif /* FLASH_EX_EN25QH_H_ */
