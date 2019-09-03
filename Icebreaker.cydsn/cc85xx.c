/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <cc85xx.h>
#include <SPI.h>
#include <SPI_SPI_UART.h>
#include <RSTn.h>
#include <timers.h>
#include <Application.h>

#define CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS  (1000)

static bool cc85xx_reset(bool boot)
{
  uint32_t start;
  uint8_t before;

  RSTn_Write(0);
  SPI_ss0_m_Write(0);
  
  SPI_mosi_m_SetDriveMode(SPI_mosi_m_DM_STRONG);

  if (boot)
  {
    SPI_mosi_m_Write(0);
  }
  else
  {
    SPI_mosi_m_Write(1);
  }
  
  CyDelay(3);
  
  RSTn_Write(1);
  
  CyDelayUs(6);
  
  SPI_ss0_m_Write(1);
  CyDelayUs(1);
  SPI_ss0_m_Write(0);
  
  before = SPI_miso_m_Read();

  start = tmrGetCounter_ms();
  while (SPI_miso_m_Read() == 0 && tmrGetElapsedMs(start) < 1000);
  
  if (SPI_miso_m_Read() && (before != SPI_miso_m_Read()) && (tmrGetElapsedMs(start) < 1000))
  {
    return true;
  }
  else
  {
    return false;
  }
}

static uint8_t cc85xx_spi_read(void)
{
  //while (SPI_SpiUartGetRxBufferSize() == 0);
  
  return (uint8_t)SPI_SpiUartReadRxData();
}

static bool cc85xx_sys_reset(void)
{
  return cc85xx_reset(false);
}

static bool cc85xx_boot_reset(void)
{
  return cc85xx_reset(true);
}

uint16_t cc85xx_get_status(void)
{
  uint16_t sw = 0;
  uint8_t txBuffer[] = { 0x80, 0x00 };

  SPI_SpiUartPutArray(txBuffer, sizeof(txBuffer));
  
  /* Wait for the end of the transfer. The number of transmitted data
  * elements has to be equal to the number of received data elements.
  */
  while(sizeof(txBuffer) != SPI_SpiUartGetRxBufferSize())
  {
  }
  
  while(SPI_SpiUartGetRxBufferSize() > 0)
  {
    sw <<= 8;
    sw |= (SPI_SpiUartReadRxData() & 0xFF);
  }

  /* Clear dummy bytes from RX buffer */
  SPI_SpiUartClearRxBuffer();
  
  return sw;
}

static bool cc85xx_basic_transaction(uint8_t *pTxData, uint32_t txLen, uint16_t *pStatus, uint8_t *pRxData, uint16_t rxLen)
{
  uint32_t start = tmrGetCounter_ms();
  uint32_t i;

  /* Wait for previous operation to complete */
  while (SPI_miso_m_Read() == 0 && tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS);
  
  if (tmrGetElapsedMs(start) >= CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS)
  {
    PRINTF("ERROR: Timed out waiting for previous SPI operation to complete\n");
    return false;
  }
  
  /* Start transfer */
  SPI_SpiUartPutArray(pTxData, txLen);

  /* Wait for the end of the transfer. The number of transmitted data
  * elements has to be equal to the number of received data elements.
  */
  while(txLen != SPI_SpiUartGetRxBufferSize())
  {
  }
  
  *pStatus = cc85xx_spi_read() << 8;
  *pStatus |= cc85xx_spi_read();
  
  if (pRxData != NULL)
  {
    for (i = 0; i < rxLen; i++)
    {
      pRxData[i] = cc85xx_spi_read();
    }
  }

  /* Clear dummy bytes from RX buffer */
  SPI_SpiUartClearRxBuffer();
  
  return true;
}

static bool cc85xx_cmd_req(uint8_t cmdType, uint8_t paramLen, uint8_t *pData, uint16_t *pStatus)
{
  bool ret = false;

  cc85xx_cmd_req_s *pTxPkt = (cc85xx_cmd_req_s *)malloc(sizeof(cc85xx_cmd_req_hdr_s) + paramLen);

  pTxPkt->hdr.opcode = CC85XX_OP_CMD_REQ;
  pTxPkt->hdr.cmd_type = cmdType;
  pTxPkt->hdr.param_len = paramLen;
  memcpy(pTxPkt->param, pData, paramLen);
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_cmd_req_hdr_s) + paramLen, pStatus, NULL, 0);
  
  free(pTxPkt);

  if (!ret)
  {
    return ret;
  }
  
  return true;
}

static bool cc85xx_read(uint16_t dataLen, uint16_t *pStatus, uint8_t *pData)
{
  bool ret = false;
  
  cc85xx_read_s *pTxPkt = (cc85xx_read_s *)malloc(sizeof(cc85xx_read_s) + dataLen);
  
  memset((void *)pTxPkt, 0xCC, sizeof(cc85xx_read_s) + dataLen);
  
  pTxPkt->opcode = CC85XX_OP_READ;
  pTxPkt->data_len = dataLen;
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_read_s) + dataLen, pStatus, pData, dataLen);

  free(pTxPkt);

  if (!ret)
  {
    return ret;
  }
  
  return true;
}

bool cc85xx_bl_unlock_spi(void)
{
  bool ret = false;
  uint32_t key = CC85XX_BOOTLOADER_KEY;
  uint16_t sw = 0xCCCC;

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_UNLOCK_SPI, sizeof(key), (uint8_t *)&key, &sw);
  
  if (!ret || (sw != CC85XX_SW_BL_UNLOCK_SPI_SUCCESS))
  {
    PRINTF("ERROR: %s: ret=%d, status=0x%04X\n", __FUNCTION__, ret, sw);
    return false;
  }
  
  return true;
}

bool cc85xx_print_info(void)
{
  uint32_t i;
  uint8_t rsp[24];
  const uint16_t rsvd = 0x00B0;
  uint16_t sw = 0;
  bool ret = false;
  
  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_DI_GET_CHIP_INFO, sizeof(rsvd), (uint8_t *)&rsvd, &sw);
  if (!ret)
  {
    PRINTF("ERROR: CMD_REQ 0x%02X failed: sw=0x%04X\n", CC85XX_CMD_TYPE_DI_GET_CHIP_INFO, sw);
    return ret;
  }

  ret = cc85xx_read(24, &sw, rsp);
  if (!ret)
  {
    PRINTF("ERROR: READ failed: sw=0x%04X\n", sw);
    return ret;
  }
  
  for (i = 0; i < 24; i++)
  {
    PRINTF("0x%02X: %02X\n", i, rsp[i]);
  }

  return ret;
}

void cc85xx_init(void)
{
  SPI_Stop();

  if (cc85xx_boot_reset() == false)
  {
    PRINTF("Failed to boot cc8520 into bootloader\n");
    return;
  }

  SPI_Start();
}

/* [] END OF FILE */
