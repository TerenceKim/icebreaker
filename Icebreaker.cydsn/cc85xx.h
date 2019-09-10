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
#include <utils.h>
  
#define CC85XX_CMD_REQ_HDR_SIZE               (2)
    
#define CC85XX_SW_BL_MASS_ERASE_SUCCESS       (0x8003)
#define CC85XX_SW_BL_FLASH_PAGE_PROG_SUCCESS  (0x800B)
#define CC85XX_SW_BL_FLASH_VERIFY_SUCCESS     (0x800E)
#define CC85XX_SW_BL_UNLOCK_SPI_SUCCESS       (0x8020)
#define CC85XX_SW_BL_UNLOCK_SPI_WAITING       (0x8021)
#define CC85XX_SW_BL_UNLOCK_SPI_FAILURE       (0x0022)

#define CC85XX_OP_SET_ADDR                    (0b0)
#define CC85XX_OP_CMD_REQ                     (0b11)
#define CC85XX_OP_WRITE                       (0b1000)
#define CC85XX_OP_READ                        (0b1001)
  
#define CC85XX_CMD_TYPE_BL_UNLOCK_SPI         (0x00)
#define CC85XX_CMD_TYPE_BL_MASS_ERASE         (0x03)
#define CC85XX_CMD_TYPE_BL_FLASH_PAGE_PROG    (0x07)
#define CC85XX_CMD_TYPE_BL_FLASH_VERIFY       (0x0F)
#define CC85XX_CMD_TYPE_DI_GET_CHIP_INFO      (0x1F)
  
#define CC85XX_BL_UNLOCK_SPI_KEY              (0x2505B007)
#define CC85XX_BL_MASS_ERASE_KEY              (0x25051337)

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

typedef struct __attribute__((packed))
{
  unsigned len_hi:4;
  unsigned opcode:4;
  
  unsigned len_lo:8;
  
  uint8_t param[1];
} cc85xx_write_s;

typedef struct __attribute__((packed))
{
  unsigned addr_hi:7;
  unsigned opcode:1;
  
  unsigned addr_lo:8;
} cc85xx_set_addr_s;

typedef struct __attribute__((packed))
{
  uint16_t ram_addr;
  uint16_t flash_addr;
  uint16_t dword_count;
  uint32_t key;
} cc85xx_bl_flash_page_prog_s;

typedef struct __attribute__((packed))
{
  uint32_t data_addr;
  uint32_t byte_count;
} cc85xx_bl_flash_verify_s;

typedef struct __attribute__((packed))
{
  unsigned family_id:16;
  
  unsigned sil_min_rev:4;
  unsigned sil_maj_rev:4;

  unsigned :8;
  
  unsigned rom_type:16;
  unsigned rom_maj_rev:8;
  unsigned rom_min_rev:8;
  
  unsigned fw_type:16;
  unsigned fw_maj_rev:4;
  unsigned fw_min_rev:4;
  unsigned fw_patch_rev:8;
  
  unsigned :32;
  
  unsigned fw_image_size:32;
  
  unsigned chip_caps:16;
  unsigned chip_id:16;
} cc85xx_di_get_chip_info_rsp_s;
  
void cc85xx_init(void);
bool cc85xx_sys_reset(void);
bool cc85xx_boot_reset(void);
bool cc85xx_bl_unlock_spi(void);
bool cc85xx_bl_mass_erase(void);
bool cc85xx_bl_flash_verify(uint32_t *pCrc32);
uint16_t cc85xx_get_status(void);
bool cc85xx_print_info(void);
bool cc85xx_flash_bytes(uint16_t addr, uint8_t *pData, uint16_t dataLen);

#endif /* CC85XX_H */
/* [] END OF FILE */
