#include "header.h"

/*****************************************************/
//��������� �������� � ������ �� SPI
/*****************************************************/
//void start_exchange_via_spi(int index_chip, unsigned int number_bytes_transfer)
//{
//  //Գ����� ������ ���� �� ����� ���������� (�� ������� �� ������� ���������� ������� - ��� ����� ���� �������� ��������� �� ��������)
//  number_bytes_transfer_spi_df = number_bytes_transfer;
//  
//  //��������� ���� DMA �� ������ ����� ���� �� ���������
//  if ((DMA_StreamSPI_DF_Rx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamSPI_DF_Rx->CR &= ~(uint32_t)DMA_SxCR_EN;
//  DMA_StreamSPI_DF_Rx->NDTR = number_bytes_transfer;
//  //��������� ���� DMA �� �������� ����� ���� �� ���������
//  if ((DMA_StreamSPI_DF_Tx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamSPI_DF_Tx->CR &= ~(uint32_t)DMA_SxCR_EN;
//  DMA_StreamSPI_DF_Tx->NDTR = number_bytes_transfer;
//  
//  //������� ���������, �� �������� ��� ���������� ������/�������� ����� ��� DMA1 �� ������ SPI_DF_Rx � SPI_DF_Tx
//  DMA_ClearFlag(DMA_StreamSPI_DF_Rx, DMA_FLAG_TCSPI_DF_Rx | DMA_FLAG_HTSPI_DF_Rx | DMA_FLAG_TEISPI_DF_Rx | DMA_FLAG_DMEISPI_DF_Rx | DMA_FLAG_FEISPI_DF_Rx);
//  DMA_ClearFlag(DMA_StreamSPI_DF_Tx, DMA_FLAG_TCSPI_DF_Tx | DMA_FLAG_HTSPI_DF_Tx | DMA_FLAG_TEISPI_DF_Tx | DMA_FLAG_DMEISPI_DF_Tx | DMA_FLAG_FEISPI_DF_Tx);
//  
//  //���������� �������� ����� DMA
//  if ((SPI_DF->CR2 & SPI_I2S_DMAReq_Tx) == 0) SPI_DF->CR2 |= SPI_I2S_DMAReq_Tx;
//  //���������� ������ ����� DMA
//  if ((SPI_DF->CR2 & SPI_I2S_DMAReq_Rx) == 0) SPI_DF->CR2 |= SPI_I2S_DMAReq_Rx;
//
//  //����������� chip_select  � ������������� � ������� �����������, �� ��� ����
//  if (index_chip == INDEX_DATAFLASH_1) GPIO_SPI_DF_TOGGLE->BSRRH = GPIO_SPI_DF_TOGGLE_Pin;//DF_TOGGLE - �� ���������� � 0 
//  else if (index_chip == INDEX_DATAFLASH_2)GPIO_SPI_DF_TOGGLE->BSRRL = GPIO_SPI_DF_TOGGLE_Pin;//DF_TOGGLE - �� ���������� � 1
//  else
//  {
//    //³������� ����������� �������, ���� ����� ��� �� ����������������
//    total_error_sw_fixed(6);
//  }
//  state_execution_spi_df[index_chip] = TRANSACTION_EXECUTING;
//  GPIO_SPI_DF->BSRRH = GPIO_NSSPin_DF; //����������� Chip_select ��������� NSS  � 0
//
//  //���������� ��������� ���������� �� ������ DMA_StreamSPI_DF_Rx
//  DMA_StreamSPI_DF_Rx->CR |= DMA_IT_TC;
//  
//  //������� ������ ��������
//  SPI_DF->DR;
//  SPI_DF->SR;
//
//  //��������� ������-�������� 
//  DMA_StreamSPI_DF_Rx->CR |= (uint32_t)DMA_SxCR_EN;
//  DMA_StreamSPI_DF_Tx->CR |= (uint32_t)DMA_SxCR_EN;
//}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
