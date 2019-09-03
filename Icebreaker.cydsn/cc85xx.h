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
#ifndef CC85XX_H
#define CC85XX_H

#include <stdint.h>
#include <stdbool.h>
  
#define CC85XX_CMD_REQ_HDR_SIZE             (2)
  
#define CC85XX_SW_BL_UNLOCK_SPI_SUCCESS     (0x8020)
#define CC85XX_SW_BL_UNLOCK_SPI_WAITING     (0x8021)
#define CC85XX_SW_BL_UNLOCK_SPI_FAILURE     (0x0022)
  
#define CC85XX_OP_CMD_REQ                   (0b11)
#define CC85XX_OP_READ                      (0b1001)
  
#define CC85XX_CMD_TYPE_BL_UNLOCK_SPI       (0x00)
#define CC85XX_CMD_TYPE_DI_GET_CHIP_INFO    (0x1F)
  
#define CC85XX_BOOTLOADER_KEY           /*(0x07B00525)*/(0x2505B007)

typedef struct __attribute__((packed))
{
  unsigned cmd_type:6;
  unsigned opcode:2;

  unsigned param_len:8;
} cc85xx_cmd_req_hdr_s;

typedef struct __attribute__((packed))
{
  cc85xx_cmd_req_hdr_s hdr;
  uint8_t param[1];
} cc85xx_cmd_req_s;

typedef struct __attribute__((packed))
{
  unsigned :4;
  unsigned opcode:4;
  
  unsigned data_len:8;
} cc85xx_read_s;

//cc85xx_di_get_chip_info_rsp_s;
  
void cc85xx_init(void);
bool cc85xx_bl_unlock_spi(void);
uint16_t cc85xx_get_status(void);
bool cc85xx_print_info(void);

#endif /* CC85XX_H */
/* [] END OF FILE */
