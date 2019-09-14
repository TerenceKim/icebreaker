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

typedef struct __attribute__((packed))
{
  uint32_t nImageBytes;
  uint16_t statusWord; 
} cc85xx_control_block_s;

static cc85xx_control_block_s cc85xxCb;

static bool cc85xx_wait_for_status(uint16_t expectedStatus, uint32_t timeout)
{
  uint32_t start = tmrGetCounter_ms();
  bool ret = false;
  
  do
  {
    (void)cc85xx_get_status();
  } while ((cc85xx_get_cached_status() != expectedStatus) && (tmrGetElapsedMs(start) < timeout));
  
  ret = (cc85xx_get_cached_status() == expectedStatus);

  if (!ret)
  {
    D_PRINTF(ERROR, "ERROR: expected vs. actual status (0x%04X, 0x%04X)\n", expectedStatus, cc85xx_get_cached_status());
  }

  return ret;
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
  uint32_t start = tmrGetCounter_ms();
  while((SPI_SpiUartGetRxBufferSize() == 0) && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS));
  return (uint8_t)SPI_SpiUartReadRxData();
}

static bool cc85xx_basic_transaction(uint8_t *pTxData, uint32_t txLen, uint8_t *pRxData, uint16_t rxLen, uint16_t *pRxLen)
{
  uint32_t start = tmrGetCounter_ms();
  uint32_t i;

  /* Wait for previous operation to complete */
  while (SPI_miso_m_Read() == 0 && tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS);
  
  if (tmrGetElapsedMs(start) >= CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS)
  {
    D_PRINTF(ERROR, "ERROR: Timed out waiting for previous SPI operation to complete\n");
    return false;
  }
  
  D_PRINTF(DEBUG, "TX: ");
  printArray(pTxData, txLen);
  
  /* Clear dummy bytes from RX buffer */
  SPI_SpiUartClearRxBuffer();
  
  /* Start transfer */
  SPI_SpiUartPutArray(pTxData, txLen);

  /* Wait for the end of the transfer. The number of transmitted data
  * elements has to be equal to the number of received data elements.
  */
  while((SPI_SpiUartGetRxBufferSize() < 2) && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS));
  
  D_PRINTF(DEBUG, "RX: ");
  cc85xxCb.statusWord = cc85xx_spi_read() << 8;
  cc85xxCb.statusWord |= cc85xx_spi_read();
  
  D_PRINTF(DEBUG, "%02X %02X ", (cc85xxCb.statusWord >> 8) & 0xFF, (cc85xxCb.statusWord & 0xFF));
  
  if (pRxLen)
  {
    while((SPI_SpiUartGetRxBufferSize() < 2) && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS));
  
    *pRxLen = cc85xx_spi_read() << 8;
    *pRxLen |= cc85xx_spi_read();
    
    D_PRINTF(DEBUG, "%02X %02X ", (*pRxLen >> 8) & 0xFF, (*pRxLen & 0xFF));
    
    rxLen = (*pRxLen < rxLen) ? *pRxLen : rxLen;
    
    if (rxLen == 0)
    {
      /* Wait for operation to finish. */
      while((SPI_SpiUartGetTxBufferSize() > 0) && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS));
  
      /* Clear dummy bytes from RX buffer */
      SPI_SpiUartClearRxBuffer();
      
      return true;
    }
  }
  
  if (pRxData)
  {
    while((SPI_SpiUartGetRxBufferSize() < rxLen) && (tmrGetElapsedMs(start) < CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS));
  
    for (i = 0; i < rxLen; i++)
    {
      pRxData[i] = cc85xx_spi_read();
    }
    printArray(pRxData, rxLen);
  }
  else
  {
    D_PRINTF(DEBUG, "\n");
  }

  /* Clear dummy bytes from RX buffer */
  SPI_SpiUartClearRxBuffer();
  
  return true;
}

static bool cc85xx_cmd_req(uint8_t cmdType, uint8_t paramLen, uint8_t *pData)
{
  bool ret = false;

  cc85xx_cmd_req_s *pTxPkt = (cc85xx_cmd_req_s *)malloc(sizeof(cc85xx_cmd_req_hdr_s) + paramLen);

  pTxPkt->hdr.opcode = CC85XX_OP_CMD_REQ;
  pTxPkt->hdr.cmd_type = cmdType;
  pTxPkt->hdr.param_len = paramLen;
  memcpy(pTxPkt->param, pData, paramLen);
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_cmd_req_hdr_s) + paramLen, NULL, 0, NULL);
  
  free(pTxPkt);

  if (!ret)
  {
    return ret;
  }
  
  return true;
}

static bool cc85xx_read(uint16_t dataLen, uint8_t *pData)
{
  bool ret = false;
  
  cc85xx_read_s *pTxPkt = (cc85xx_read_s *)malloc(sizeof(cc85xx_read_s) + dataLen);
  
  memset((void *)pTxPkt, 0x80, sizeof(cc85xx_read_s) + dataLen);
  memset((void *)pTxPkt, 0, sizeof(cc85xx_read_s));

  pTxPkt->opcode = CC85XX_OP_READ;
  pTxPkt->data_len = dataLen;
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_read_s) + dataLen, pData, dataLen, NULL);

  free(pTxPkt);

  if (!ret)
  {
    D_PRINTF(ERROR, "ERROR: READ failed: sw=0x%04X\n", cc85xx_get_cached_status());
    return ret;
  }
  
  return true;
}

static bool cc85xx_readbc(uint8_t *pRxData, uint16_t *pRxLen)
{
  bool ret = false;
  uint8_t *pTxBuf = (uint8_t *)malloc(sizeof(cc85xx_readbc_cmd_s) + sizeof(uint16_t) + *pRxLen);
  
  memset(pTxBuf, 0x80, sizeof(cc85xx_readbc_cmd_s) + sizeof(uint16_t) + *pRxLen);
  
  cc85xx_readbc_cmd_s *pCmd = (cc85xx_readbc_cmd_s *)pTxBuf;

  memset(pCmd, 0, sizeof(cc85xx_readbc_cmd_s));
  pCmd->opcode = CC85XX_OP_READBC;

  ret = cc85xx_basic_transaction(pTxBuf, sizeof(cc85xx_readbc_cmd_s) + sizeof(uint16_t) + *pRxLen, pRxData, *pRxLen, pRxLen);
  
  free(pTxBuf);
  
  return ret;
}

static bool cc85xx_set_addr(uint16_t addr)
{
  bool ret = false;
  
  cc85xx_set_addr_s *pTxPkt = (cc85xx_set_addr_s *)malloc(sizeof(cc85xx_set_addr_s));
  
  pTxPkt->opcode = CC85XX_OP_SET_ADDR;
  pTxPkt->addr_hi = (addr >> 8) & 0xFF;
  pTxPkt->addr_lo = (addr & 0xFF);
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_set_addr_s), NULL, 0, NULL);
  
  free(pTxPkt);
  
  if (!ret)
  {
    return ret;
  }
  
  return true;
}

static bool cc85xx_write(uint16_t dataLen, uint8_t *pData)
{
  bool ret = false;
  
  cc85xx_write_s *pTxPkt = (cc85xx_write_s *)malloc(sizeof(cc85xx_write_s) + dataLen);
  
  pTxPkt->opcode = CC85XX_OP_WRITE;
  pTxPkt->len_hi = (dataLen >> 8) & 0xFF;
  pTxPkt->len_lo = (dataLen & 0xFF);
  memcpy(pTxPkt->param, pData, dataLen);
  
  ret = cc85xx_basic_transaction((uint8_t *)pTxPkt, sizeof(cc85xx_write_s) + dataLen, NULL, 0, NULL);
  
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
  cc85xx_bl_flash_page_prog_s *pTxPkt = (cc85xx_bl_flash_page_prog_s *)malloc(sizeof(cc85xx_bl_flash_page_prog_s));

  pTxPkt->ram_addr = BE16(ramAddr);
  pTxPkt->flash_addr = BE16(flashAddr);
  pTxPkt->dword_count = BE16(dwordCount);
  pTxPkt->key = BE32(CC85XX_BL_MASS_ERASE_KEY);

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_FLASH_PAGE_PROG, sizeof(cc85xx_bl_flash_page_prog_s), (uint8_t *)pTxPkt);
  
  free(pTxPkt);

  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_FLASH_PAGE_PROG_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS))
  {
    
    return false;
  }
  
  return true;
}

bool cc85xx_bl_unlock_spi(void)
{
  bool ret = false;
  uint32_t key = BE32(CC85XX_BL_UNLOCK_SPI_KEY);
  cc85xxCb.statusWord = 0xCCCC;

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_UNLOCK_SPI, sizeof(key), (uint8_t *)&key);
  
  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_UNLOCK_SPI_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS))
  {
    
    return false;
  }

  return true;
}

bool cc85xx_bl_mass_erase(void)
{
  bool ret = false;
  uint32_t key = BE32(CC85XX_BL_MASS_ERASE_KEY);
  cc85xxCb.statusWord = 0xCCCC;

  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_MASS_ERASE, sizeof(key), (uint8_t *)&key);
  
  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_MASS_ERASE_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS))
  {
    
    return false;
  }

  return true;
}

bool cc85xx_bl_flash_verify(uint32_t *pCrc32)
{
  bool ret = false;
  bool rv = true;

  cc85xx_bl_flash_verify_s *pTxPkt = (cc85xx_bl_flash_verify_s *)malloc(sizeof(cc85xx_bl_flash_verify_s));

  pTxPkt->byte_count = BE32(cc85xxCb.nImageBytes);
  pTxPkt->data_addr = BE32(0x8000UL);
  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_BL_FLASH_VERIFY, sizeof(cc85xx_bl_flash_verify_s), (uint8_t *)pTxPkt);

  free(pTxPkt);

  if (!ret || !cc85xx_wait_for_status(CC85XX_SW_BL_FLASH_VERIFY_SUCCESS, CC85XX_WAIT_FOR_OPERATION_TIMEOUT_MS))
  {
    
    rv = false;
  }
  
  if (pCrc32)
  {
    ret = cc85xx_read(sizeof(uint32_t), (uint8_t *)pCrc32);
    if (ret)
    {
      *pCrc32 = BE32(*pCrc32);
      D_PRINTF(ERROR, "nImageBytes = %u (0x%08x)\nCRC_VAL = 0x%08X\nsw = 0x%04X\n", cc85xxCb.nImageBytes, cc85xxCb.nImageBytes, *pCrc32, cc85xxCb.statusWord);
    }
  }
  
  return rv;
}

bool cc85xx_di_get_chip_info(cc85xx_di_get_chip_info_rsp_s *pInfo)
{
  const uint16_t rsvd = 0x00B0;
  bool ret = false;
  
  ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_DI_GET_CHIP_INFO, sizeof(rsvd), (uint8_t *)&rsvd);
  if (!ret)
  {
    return ret;
  }

  ret = cc85xx_read(sizeof(cc85xx_di_get_chip_info_rsp_s), (uint8_t *)pInfo);
  if (!ret)
  {
    return ret;
  }
  
  return ret;
}

bool cc85xx_di_get_device_info(cc85xx_di_get_device_info_rsp_s *pInfo)
{
  bool ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_DI_GET_DEVICE_INFO, 0, NULL);
  if (!ret)
  {
    return ret;
  }
  
  ret = cc85xx_read(sizeof(cc85xx_di_get_device_info_rsp_s), (uint8_t *)pInfo);
  if (!ret)
  {
    return ret;
  }
  
  return ret;
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
  uint8_t txBuffer[] = { 0x80, 0x00 };

  (void)cc85xx_basic_transaction(txBuffer, sizeof(txBuffer), NULL, 0, NULL);
  
  return cc85xxCb.statusWord;
}

uint16_t cc85xx_get_cached_status(void)
{
  return cc85xxCb.statusWord;
}

void cc85xx_init(void)
{
  cc85xx_sys_reset();
}

bool cc85xx_flash_bytes(uint16_t addr, uint8_t *pData, uint16_t dataLen)
{
  bool ret = true;
  uint16_t ram_addr = 0x6000 + (addr % CC85XX_PAGE_SIZE);
  
  // SET_ADDR
  cc85xx_set_addr(ram_addr);
  
  // WRITE
  cc85xx_write(dataLen, pData);
  
  if ((dataLen + (addr % CC85XX_PAGE_SIZE)) >= CC85XX_PAGE_SIZE)
  {
    // Program the page
    ret = cc85xx_bl_flash_page_prog(0x6000, 0x8000 + ((addr - 0x8000) / CC85XX_PAGE_SIZE) * CC85XX_PAGE_SIZE, 0x100);
  }
  
  if (addr <= 0x801C && (addr + dataLen) >= 0x801C)
  {
    addr = 0x801C - addr;
    cc85xxCb.nImageBytes = BE32(*((uint32_t *)&pData[addr]));
  }

  return ret;
}

bool cc85xx_ehc_evt_mask(cc85xx_ehc_evt_mask_cmd_s *pCmd)
{
  return cc85xx_cmd_req(CC85XX_CMD_TYPE_EHC_EVT_MASK, sizeof(cc85xx_ehc_evt_mask_cmd_s), (uint8_t *)pCmd);
}

bool cc85xx_ehc_evt_clr(cc85xx_ehc_evt_clr_cmd_s *pCmd)
{
  return cc85xx_cmd_req(CC85XX_CMD_TYPE_EHC_EVT_CLR, sizeof(cc85xx_ehc_evt_clr_cmd_s), (uint8_t *)pCmd);
}

bool cc85xx_nvs_get_data(uint8_t slotIdx, uint32_t *pData)
{
  uint8_t param = slotIdx;
  bool ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_NVS_GET_DATA, sizeof(slotIdx), &param);
  if (!ret)
  {
    return ret;
  }

  return cc85xx_read(sizeof(uint32_t), (uint8_t *)pData);
}

bool cc85xx_nvs_set_data(uint8_t slotIdx, uint32_t data)
{
  uint8_t pars[] = { slotIdx, 0, 0, 0, 0 };

  memcpy((void *)&pars[1], (void *)&data, sizeof(uint32_t));

  return cc85xx_cmd_req(CC85XX_CMD_TYPE_NVS_SET_DATA, sizeof(pars), pars);
}

bool cc85xx_nwm_do_scan(cc85xx_nwm_do_scan_cmd_s *pCmd)
{
  return cc85xx_cmd_req(CC85XX_CMD_TYPE_NWM_DO_SCAN, sizeof(cc85xx_nwm_do_scan_cmd_s), (uint8_t *)pCmd);
}

bool cc85xx_nwm_get_scan_results(cc85xx_nwm_do_scan_rsp_s *pRsp, uint16_t *pRxLen)
{
  return cc85xx_readbc((uint8_t *)pRsp, pRxLen);
}

bool cc85xx_nwm_do_join(cc85xx_nwm_do_join_cmd_s *pCmd)
{
  uint16_t join_to = (pCmd->join_to_hi << 8) | pCmd->join_to_lo;

  if (pCmd->device_id != 0)
  {
    // Not a request to leave a network, check join_to parameter
    if (join_to < 50)
    {
      // JOIN_TO must be 500ms or higher to account for one-time RF channel scan
      return false;
    }
  }
  return cc85xx_cmd_req(CC85XX_CMD_TYPE_NWM_DO_JOIN, sizeof(cc85xx_nwm_do_join_cmd_s), (uint8_t *)pCmd);
}

bool cc85xx_nwm_get_status(uint8_t *pRsp, uint16_t *pRxLen)
{
  bool ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_NWM_GET_STATUS, 0, NULL);
  if (!ret)
  {
    return ret;
  }

  return cc85xx_readbc(pRsp, pRxLen);
}

bool cc85xx_ps_rf_stats(cc85xx_ps_rf_stats_s *pRsp, uint16_t *pRxLen)
{
  bool ret = cc85xx_cmd_req(CC85XX_CMD_TYPE_PS_RF_STATS, 0, NULL);
  if (!ret)
  {
    return ret;
  }
  
  return cc85xx_readbc((uint8_t *)pRsp, pRxLen);
}


/* [] END OF FILE */
