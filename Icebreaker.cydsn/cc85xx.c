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
#include <utils.h>

#define CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS  (1000)
#define CC85XX_PAGE_SIZE                      (1024) // 1KB

static uint32_t nImageBytes;


static bool cc85xx_wait_for_status(uint16_t expectedStatus, uint32_t timeout, uint16_t *pStatus)
{
  uint32_t start = tmrGetCounter_ms();
  
  do
  {
    *pStatus = cc85xx_get_status();
  } while ((*pStatus != expectedStatus) && (tmrGetElapsedMs(start) < timeout));
  
  return (*pStatus == expectedStatus);
}

static bool cc85xx_reset(bool boot)
{
  uint32_t start;
  uint8_t before;
  bool ret = false;

  RSTn_SetDriveMode(RSTn_DM_STRONG);
  RSTn_Write(0);
  
  SPI_ss0_m_SetDriveMode(SPI_ss0_m_DM_STRONG);
  SPI_ss0_m_Write(0);
  
  /* Set output pin state after block is disabled */
  //SPI_mosi_m_Write(SPI_GET_SPI_MOSI_INACTIVE);

  /* Set GPIO to drive output pin */
  SPI_SET_HSIOM_SEL(SPI_MOSI_M_HSIOM_REG, SPI_MOSI_M_HSIOM_MASK,
                                 SPI_MOSI_M_HSIOM_POS, SPI_MOSI_M_HSIOM_SEL_GPIO);

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
  CyDelayUs(2);
  SPI_ss0_m_Write(0);
  
  /* Set GPIO to drive output pin */
  //SPI_SET_HSIOM_SEL(SPI_MISO_M_HSIOM_REG, SPI_MISO_M_HSIOM_MASK,
  //                               SPI_MISO_M_HSIOM_POS, SPI_MISO_M_HSIOM_SEL_GPIO);
  
  //SPI_miso_m_SetDriveMode(SPI_miso_m_DM_RES_UPDWN);
  
  before = SPI_miso_m_Read();

  start = tmrGetCounter_ms();
  while (SPI_miso_m_Read() == 0 && tmrGetElapsedMs(start) < 1000);
  
  if (SPI_miso_m_Read() && (before != SPI_miso_m_Read()) && (tmrGetElapsedMs(start) < 1000))
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  
  /* Set GPIO to back to SPI */
  SPI_SET_HSIOM_SEL(SPI_MOSI_M_HSIOM_REG, SPI_MOSI_M_HSIOM_MASK,
                               SPI_MOSI_M_HSIOM_POS, SPI_MOSI_M_HSIOM_SEL_SPI);
  
  return ret;
}

static uint8_t cc85xx_spi_read(void)
{
  //while (SPI_SpiUartGetRxBufferSize() == 0);
  
  return (uint8_t)SPI_SpiUartReadRxData();
}

bool cc85xx_sys_reset(void)
{
  bool ret;

  SPI_Stop();
  ret = cc85xx_reset(false);
  SPI_Start();

  return ret;
}

bool cc85xx_boot_reset(void)
{
  bool ret;
  
  SPI_Stop();
  ret = cc85xx_reset(true);
  SPI_Start();
  
  return ret;
}

uint16_t cc85xx_get_status(void)
{
  uint16_t sw = 0;
  uint8_t txBuffer[] = { 0x80, 0x00 };
  uint32_t start;

  SPI_SpiUartPutArray(txBuffer, sizeof(txBuffer));
  
  /* Wait for the end of the transfer. The number of transmitted data
  * elements has to be equal to the number of received data elements.
  */
  start = tmrGetCounter_ms();
  
  while((sizeof(txBuffer) != SPI_SpiUartGetRxBufferSize()) && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS))
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
  start = tmrGetCounter_ms();

  while(txLen != SPI_SpiUartGetRxBufferSize() && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS))
  {
  }
  
  if (pStatus)
  {
    *pStatus = cc85xx_spi_read() << 8;
    *pStatus |= cc85xx_spi_read();
  }
  
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

static bool cc85xx_set_addr(uint16_t addr, uint16_t *pStatus)
{
  bool ret = false;
  
  cc85xx_set_addr_s *pTxPkt = (cc85xx_set_addr_s *)malloc(sizeof(cc85xx_set_addr_s));
  
  pTxPkt->opcode = CC85XX_OP_SET_ADDR;
  pTxPkt->addr_hi = (addr >> 8) & 0xFF;
  pTxPkt->addr_lo = (addr & 0xFF);
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_set_addr_s), pStatus, NULL, 0);
  
  free(pTxPkt);
  
  if (!ret)
  {
    return ret;
  }
  
  return true;
}

static bool cc85xx_write(uint16_t dataLen, uint8_t *pData, uint16_t *pStatus)
{
  bool ret = false;
  
  cc85xx_write_s *pTxPkt = (cc85xx_write_s *)malloc(sizeof(cc85xx_write_s) + dataLen);
  
  pTxPkt->opcode = CC85XX_OP_WRITE;
  pTxPkt->len_hi = (dataLen >> 8) & 0xFF;
  pTxPkt->len_lo = (dataLen & 0xFF);
  memcpy(pTxPkt->param, pData, dataLen);
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_write_s) + dataLen, pStatus, NULL, 0);
  
  free(pTxPkt);
  
  if (!ret)
  {
    return ret;
  }
  
  return true;
}

static bool cc85xx_bl_flash_page_prog(uint16_t ramAddr, uint16_t flashAddr, uint16_t dwordCount)
{
  bool ret = false;
  uint16_t sw;
  cc85xx_bl_flash_page_prog_s *pTxPkt = (cc85xx_bl_flash_page_prog_s *)malloc(sizeof(cc85xx_bl_flash_page_prog_s));

  pTxPkt->ram_addr = BE16(ramAddr);
  pTxPkt->flash_addr = BE16(flashAddr);
  pTxPkt->dword_count = BE16(dwordCount);
  pTxPkt->key = BE32(CC85XX_BL_MASS_ERASE_KEY);

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_FLASH_PAGE_PROG, sizeof(cc85xx_bl_flash_page_prog_s), (uint8_t *)pTxPkt, &sw);
  
  free(pTxPkt);

  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_FLASH_PAGE_PROG_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS, &sw))
  {
    PRINTF("ERROR: %s: ret=%d, status=0x%04X\n", __FUNCTION__, ret, sw);
    return false;
  }
  
  return true;
}

bool cc85xx_bl_unlock_spi(void)
{
  bool ret = false;
  uint32_t key = BE32(CC85XX_BL_UNLOCK_SPI_KEY);
  uint16_t sw = 0xCCCC;

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_UNLOCK_SPI, sizeof(key), (uint8_t *)&key, &sw);
  
  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_UNLOCK_SPI_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS, &sw))
  {
    PRINTF("ERROR: %s: ret=%d, status=0x%04X\n", __FUNCTION__, ret, sw);
    return false;
  }

  return true;
}

bool cc85xx_bl_mass_erase(void)
{
  bool ret = false;
  uint32_t key = BE32(CC85XX_BL_MASS_ERASE_KEY);
  uint16_t sw = 0xCCCC;

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_MASS_ERASE, sizeof(key), (uint8_t *)&key, &sw);
  
  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_MASS_ERASE_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS, &sw))
  {
    PRINTF("ERROR: %s: ret=%d, status=0x%04X\n", __FUNCTION__, ret, sw);
    return false;
  }

  return true;
}

bool cc85xx_bl_flash_verify(uint32_t *pCrc32)
{
  bool ret = false;
  bool rv = true;
  uint16_t sw;

  cc85xx_bl_flash_verify_s *pTxPkt = (cc85xx_bl_flash_verify_s *)malloc(sizeof(cc85xx_bl_flash_verify_s));

  pTxPkt->byte_count = BE32(nImageBytes);
  pTxPkt->data_addr = BE32(0x8000UL);
  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_FLASH_VERIFY, sizeof(cc85xx_bl_flash_verify_s), (uint8_t *)pTxPkt, &sw);

  free(pTxPkt);

  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_FLASH_VERIFY_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS, &sw))
  {
    PRINTF("ERROR: %s: ret=%d, status=0x%04X\n", __FUNCTION__, ret, sw);
    rv = false;
  }
  
  if (pCrc32)
  {
    ret = cc85xx_read(sizeof(uint32_t), &sw, (uint8_t *)pCrc32);
    if (ret)
    {
      PRINTF("nImageBytes = %u (0x%08x)\nCRC_VAL = 0x%08X\nsw = 0x%04X\n", nImageBytes, nImageBytes, BE32(*pCrc32), sw);
    }
  }
  
  return rv;
}

bool cc85xx_print_info(void)
{
  uint32_t i;
  uint8_t rsp[24];
  const uint16_t rsvd = 0x00B0;
  uint16_t sw = 0;
  bool ret = false;
  cc85xx_di_get_chip_info_rsp_s *pRsp;
  
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
  
#if 0
  for (i = 0; i < 24; i++)
  {
    PRINTF("0x%02X: %02X\n", i, rsp[i]);
  }
#endif
  
  pRsp = (cc85xx_di_get_chip_info_rsp_s *)rsp;
  
  PRINTF("\tfamily_id: 0x%04X\n", BE16(pRsp->family_id));
  
  PRINTF("\tsil_min_rev: %d\n", pRsp->sil_min_rev);
  PRINTF("\tsil_maj_rev: %d\n", pRsp->sil_maj_rev);
  
  PRINTF("\trom_min_rev: %d\n", pRsp->rom_min_rev);
  PRINTF("\trom_maj_rev: %d\n", pRsp->rom_maj_rev);
  PRINTF("\trom_type: 0x%04X\n", BE16(pRsp->rom_type));
  
  PRINTF("\tfw_patch_rev: %d\n", pRsp->fw_patch_rev);
  PRINTF("\tfw_min_rev: %d\n", pRsp->fw_min_rev);
  PRINTF("\tfw_maj_rev: %d\n", pRsp->fw_maj_rev);
  PRINTF("\tfw_type: 0x%04X\n", BE16(pRsp->fw_type));
  
  PRINTF("\tfw_image_size: %d\n", BE32(pRsp->fw_image_size));
  
  PRINTF("\tchip_id: 0x%04X\n", BE16(pRsp->chip_id));
  PRINTF("\tchip_caps: 0x%04X\n", BE16(pRsp->chip_caps));

  return ret;
}

void cc85xx_init(void)
{
  /*
  SPI_Stop();

  if (cc85xx_boot_reset() == false)
  {
    PRINTF("Failed to initialize cc85xx\n");
    return;
  }
  */

  SPI_Start();
}

bool cc85xx_flash_bytes(uint16_t addr, uint8_t *pData, uint16_t dataLen)
{
  bool ret = true;
  uint16_t sw;
  uint16_t ram_addr = 0x6000 + (addr % CC85XX_PAGE_SIZE);
  
  // SET_ADDR
  cc85xx_set_addr(ram_addr, &sw);
  
  // WRITE
  cc85xx_write(dataLen, pData, &sw);
  
  if ((dataLen + (addr % CC85XX_PAGE_SIZE)) >= CC85XX_PAGE_SIZE)
  {
    // Program the page
    ret = cc85xx_bl_flash_page_prog(0x6000, 0x8000 + ((addr - 0x8000) / CC85XX_PAGE_SIZE) * CC85XX_PAGE_SIZE, 0x100);
  }
  
  if (addr <= 0x801C && (addr + dataLen) >= 0x801C)
  {
    addr = 0x801C - addr;
    nImageBytes = BE32(*((uint32_t *)&pData[addr]));
  }

  return ret;
}

/* [] END OF FILE */
