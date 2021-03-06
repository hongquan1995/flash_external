/*
 * EN25QH.c
 *
 *  Created on: Nov 4, 2021
 *      Author: 0
 */

/**
 * @brief    Read Flash Inside ID
 * @param   none
 * @retval   Successfully returns device_id
 */

#include "stm32l4xx_hal.h"
#include "EN25QH.h"
#include "SPI.h"
#include "time.h"

extern 	uint32_t RT_Count_Systick_u32;
/***************************************************
Chip Select
*/
void Flash_EN25QH_ChipSelect(uint8_t State)
{
	uint16_t	i = 0;
	/* Set High or low the chip select line on PA.4 pin */
	for(i = 0; i<500; i++); // ??? có nên dùng
	if (State == LOW)
		HAL_GPIO_WritePin(EN25QH_SC_PORT, EN25QH_SC_PIN, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(EN25QH_SC_PORT, EN25QH_SC_PIN, GPIO_PIN_SET);
	// delay
	for(i = 0; i<500; i++);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t Flash_EN25QH_Send_Byte(uint8_t byte)
{
	uint8_t retVal=0;
	SPI_TransmitReceive(&byte, &retVal, 1);
	return retVal;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t Flash_EN25QH_Connect(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	/* Select the FLASH: Chip Select low */
	Flash_EN25QH_ChipSelect(LOW);
	/* Send "RDID " instruction */
	Flash_EN25QH_Send_Byte(EN25QH_JEDEC);
	/* Read a byte from the FLASH */
	Temp0 = Flash_EN25QH_Send_Byte(DUMMY_BYTE);
	/* Read a byte from the FLASH */
	Temp1 = Flash_EN25QH_Send_Byte(DUMMY_BYTE);
	/* Read a byte from the FLASH */
	Temp2 = Flash_EN25QH_Send_Byte(DUMMY_BYTE);
	/* Deselect the FLASH: Chip Select high */
	Flash_EN25QH_ChipSelect(HIGH);
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/* Chua co check trang thai khi ghi thanh cong */
void Flash_EN25QH_Enable(void)
{
  /* Select the FLASH: Chip Select low */
	Flash_EN25QH_ChipSelect(LOW);

  /* Send "Write Enable" instruction */
	Flash_EN25QH_Send_Byte(EN25QH_WREN);

  /* Deselect the FLASH: Chip Select high */
	Flash_EN25QH_ChipSelect(HIGH);
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void Flash_EN25QH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  Flash_EN25QH_ChipSelect(LOW);

  /*!< Send "Read Status Register" instruction */
  Flash_EN25QH_Send_Byte(EN25QH_READ_STATUS);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = Flash_EN25QH_Send_Byte(DUMMY_BYTE);

  }
  while ((flashstatus & WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  Flash_EN25QH_ChipSelect(HIGH);
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void Flash_EN25QH_EraseSector(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
	Flash_EN25QH_Enable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
	Flash_EN25QH_ChipSelect(LOW);
  /*!< Send Sector Erase instruction */
	Flash_EN25QH_Send_Byte(EN25QH_SECTOR_ERASE);
  /*!< Send SectorAddr high nibble address byte */
	Flash_EN25QH_Send_Byte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
	Flash_EN25QH_Send_Byte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
	Flash_EN25QH_Send_Byte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
	Flash_EN25QH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
	Flash_EN25QH_WaitForWriteEnd();
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void Flash_EN25QH_EraseBulk(void)
{
  /*!< Send write enable instruction */
	Flash_EN25QH_Enable();

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  Flash_EN25QH_ChipSelect(LOW);
  /*!< Send Bulk Erase instruction  */
  Flash_EN25QH_Send_Byte(EN25QH_CHIP_ERASE);
  /*!< Deselect the FLASH: Chip Select high */
  Flash_EN25QH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
  Flash_EN25QH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "Flash_EN25QH_PAGESIZE" value.
  * @retval None
  */
void Flash_EN25QH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
	Flash_EN25QH_Enable();

  /*!< Select the FLASH: Chip Select low */
	Flash_EN25QH_ChipSelect(LOW);
  /*!< Send "Write to Memory " instruction */
	Flash_EN25QH_Send_Byte(EN25QH_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
	Flash_EN25QH_Send_Byte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
	Flash_EN25QH_Send_Byte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
	Flash_EN25QH_Send_Byte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
	  Flash_EN25QH_Send_Byte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  Flash_EN25QH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
  Flash_EN25QH_WaitForWriteEnd();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void Flash_EN25QH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % Flash_EN25QH_PAGESIZE; /* Kiem tra truong hop dia chi bat dau ko phai la boi so cua PAGE_SIZE */
  count = Flash_EN25QH_PAGESIZE - Addr;		/* tinh khoang trong con lai cua 1 page de ghi du lieu */
  NumOfPage = NumByteToWrite / Flash_EN25QH_PAGESIZE;
  NumOfSingle = NumByteToWrite % Flash_EN25QH_PAGESIZE;

  if (Addr == 0) /*!< WriteAddr is Flash_EN25QH_PAGESIZE aligned - Dia chi la boi so cua PAGE_SIZE */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < Flash_EN25QH_PAGESIZE */
    {
    	Flash_EN25QH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > Flash_EN25QH_PAGESIZE */
    {
      while (NumOfPage--)
      {
    	Flash_EN25QH_WritePage(pBuffer, WriteAddr, Flash_EN25QH_PAGESIZE);
        WriteAddr += Flash_EN25QH_PAGESIZE;
        pBuffer += Flash_EN25QH_PAGESIZE;
      }

      Flash_EN25QH_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not Flash_EN25QH_PAGESIZE aligned  - Dia chi ko phai la boi so cua PAGE_SIZE */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < Flash_EN25QH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > Flash_EN25QH_PAGESIZE - so khoang trong ko du, phai can page moi */
      {
        temp = NumOfSingle - count;		// tinh khoang data can luu o page moi

        Flash_EN25QH_WritePage(pBuffer, WriteAddr, count);
        WriteAddr += count;
        pBuffer += count;

        Flash_EN25QH_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
    	Flash_EN25QH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / Flash_EN25QH_PAGESIZE;
      NumOfSingle = NumByteToWrite % Flash_EN25QH_PAGESIZE;

      Flash_EN25QH_WritePage(pBuffer, WriteAddr, count);
      WriteAddr += count;
      pBuffer += count;

      while (NumOfPage--)
      {
    	Flash_EN25QH_WritePage(pBuffer, WriteAddr, Flash_EN25QH_PAGESIZE);
        WriteAddr += Flash_EN25QH_PAGESIZE;
        pBuffer += Flash_EN25QH_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
    	Flash_EN25QH_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void Flash_EN25QH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
	Flash_EN25QH_ChipSelect(LOW);

  /*!< Send "Read from Memory " instruction */
	Flash_EN25QH_Send_Byte(EN25QH_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
	Flash_EN25QH_Send_Byte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
	Flash_EN25QH_Send_Byte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
	Flash_EN25QH_Send_Byte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = Flash_EN25QH_Send_Byte(DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  Flash_EN25QH_ChipSelect(HIGH);
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void Flash_EN25QH_StartReadSequence(uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
	Flash_EN25QH_ChipSelect(LOW);

  /*!< Send "Read from Memory " instruction */
	Flash_EN25QH_Send_Byte(EN25QH_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
	Flash_EN25QH_Send_Byte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
	Flash_EN25QH_Send_Byte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
	Flash_EN25QH_Send_Byte(ReadAddr & 0xFF);
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t Flash_EN25QH_ReadByte(void)
{
  return (Flash_EN25QH_Send_Byte(DUMMY_BYTE));
}

