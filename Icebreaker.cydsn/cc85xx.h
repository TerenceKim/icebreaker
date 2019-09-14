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

#define CC85XX_FAMILY_ID                      (0x2505)

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
#define CC85XX_OP_READBC                      (0b1010)
  
#define CC85XX_CMD_TYPE_BL_UNLOCK_SPI         (0x00)
#define CC85XX_CMD_TYPE_BL_MASS_ERASE         (0x03)
#define CC85XX_CMD_TYPE_BL_FLASH_PAGE_PROG    (0x07)
#define CC85XX_CMD_TYPE_NWM_DO_SCAN           (0x08)
#define CC85XX_CMD_TYPE_NWM_DO_JOIN           (0x09)
#define CC85XX_CMD_TYPE_NWM_GET_STATUS        (0x0A)
#define CC85XX_CMD_TYPE_BL_FLASH_VERIFY       (0x0F)
#define CC85XX_CMD_TYPE_PS_RF_STATS           (0x10)
#define CC85XX_CMD_TYPE_EHC_EVT_CLR           (0x19)
#define CC85XX_CMD_TYPE_EHC_EVT_MASK          (0x1A)
#define CC85XX_CMD_TYPE_DI_GET_DEVICE_INFO    (0x1E)
#define CC85XX_CMD_TYPE_DI_GET_CHIP_INFO      (0x1F)
#define CC85XX_CMD_TYPE_NVS_GET_DATA          (0x2B)
#define CC85XX_CMD_TYPE_NVS_SET_DATA          (0x2C)
  
#define CC85XX_BL_UNLOCK_SPI_KEY              (0x2505B007)
#define CC85XX_BL_MASS_ERASE_KEY              (0x25051337)

typedef struct __attribute__((packed))
{
  unsigned sr_chg:1;
  unsigned nwk_chg:1;
  unsigned ps_chg:1;
  unsigned vol_chg:1;
  unsigned spi_error:1;
  unsigned dsc_reset:1;
  unsigned dsc_tx_avail:1;
  unsigned dsc_rx_avail:1;
} cc85xx_evt_mask_s;
  
/**
 * Basic SPI operations.
 */
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
  unsigned :4;
  unsigned opcode:4;
  
  unsigned :8;
} cc85xx_readbc_cmd_s;

typedef struct __attribute__((packed))
{
  uint16_t data_len;
  uint8_t data[1];
} cc85xx_readbc_rsp_s;

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
  cc85xx_evt_mask_s evt;
  
  unsigned wasp_conn:1;
  unsigned pwr_state:3;
  unsigned :3;
  unsigned cmdreq_rdy:1;
} cc85xx_get_status_s;

/**
 * Bootloader SPI Command Set.
 */
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

/**
 * Device Information.
 */
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
  unsigned fw_min_rev:4;
  unsigned fw_maj_rev:4;
  unsigned fw_patch_rev:8;
  
  unsigned :32;
  
  unsigned fw_image_size:32;
  
  unsigned chip_caps:16;
  unsigned chip_id:16;
} cc85xx_di_get_chip_info_rsp_s;

typedef struct __attribute__((packed))
{
  uint32_t device_id;
  uint32_t manf_id;
  uint32_t prod_id;
} cc85xx_di_get_device_info_rsp_s;

/**
 * EHIF Control Commands.
 */
typedef struct __attribute__((packed))
{
  unsigned ehif_irq_pol:1;
  unsigned rsvd:7;
  
  cc85xx_evt_mask_s evt;
} cc85xx_ehc_evt_mask_cmd_s;

typedef struct __attribute__((packed))
{
  cc85xx_evt_mask_s evt;
} cc85xx_ehc_evt_clr_cmd_s;

/**
 * Audio Network Control and Status Commands.
 */
typedef struct __attribute__((packed))
{
  uint16_t scan;
  
  uint32_t manf_id;
  uint32_t prod_id_mask;
  uint32_t prod_id_ref;
  
  unsigned req_wpm_pair_signal:1;
  unsigned rsvd:7;
  
  uint8_t req_rssi;
} cc85xx_nwm_do_scan_cmd_s;

typedef struct __attribute__((packed))
{
  unsigned format1:3;
  unsigned active1:1;
  unsigned format0:3;
  unsigned active0:1;
} cc85xx_audio_channel_s;

typedef struct __attribute__((packed))
{
  uint32_t device_id;
  uint32_t manf_id;
  uint32_t prod_id;
  
  unsigned :1;
  unsigned wpm_allows_join:1;
  unsigned wpm_pair_signal:1;
  unsigned wpm_mfct_filt:1;
  unsigned wpm_dsc_en:1;
  unsigned wpm_pm_state:3;
  
  uint16_t ach_support;

  cc85xx_audio_channel_s ach[8];
  
  uint8_t rssi;

  unsigned sample_rate_hi:4;
  unsigned :4;
  
  unsigned sample_rate_lo:8;
  
  unsigned audio_latency_hi:4;
  unsigned :4;
  
  unsigned audio_latency_lo:8;
} cc85xx_nwm_do_scan_rsp_s;

typedef struct __attribute__((packed))
{
  uint16_t join_to;

  uint32_t device_id;

  uint32_t manf_id;

  uint32_t prod_id_mask;
  uint32_t prod_id_ref;
} cc85xx_nwm_do_join_cmd_s;

typedef struct __attribute__((packed))
{
  uint32_t device_id;
  uint32_t man_id;
  uint32_t prod_id;

  unsigned :1;
  unsigned wpm_allows_join:1;
  unsigned wpm_pair_signal:1;
  unsigned wpm_mfc_filt:1;
  unsigned wpm_dsc_en:1;
  unsigned wpm_pm_state:3;

  cc85xx_audio_channel_s ach[8];

  uint8_t  rssi;

  unsigned sample_rate_hi:4;
  unsigned :4;

  unsigned sample_rate_lo:8;

  unsigned latency_hi:4;
  unsigned nwk_status:4;

  unsigned latency_lo:8;

  uint16_t ach_used;
} cc85xx_nwm_get_status_rsp_slave_s;

typedef struct __attribute__((packed))
{
  uint32_t device_id;
  uint32_t manf_id;
  uint32_t prod_id;
  uint16_t ach_used;
} cc85xx_nwm_get_status_slave_info_s;

typedef struct __attribute__((packed))
{
  unsigned nwk_status:4;
  unsigned :4;

  unsigned sample_rate_hi:4;
  unsigned :4;

  unsigned sample_rate_lo:8;

  uint16_t ach_used;

  unsigned :8;

  unsigned wps_dsc_en:1;
  unsigned slave_short_id:3;
  unsigned :4;
  cc85xx_nwm_get_status_slave_info_s slave[1];
} cc85xx_nwm_get_status_rsp_master_s;

/**
 * RF and Audio Statistics Commands.
 */
typedef struct __attribute__((packed))
{
  uint32_t timeslot_count;
  uint32_t rx_pkt_count;
  
  uint32_t rx_pkt_fail_count;
  uint32_t rx_slice_count;
  
  uint32_t rx_slice_err_count;

  uint8_t  nwk_join_count;
  uint8_t  nwk_drop_count;
  uint16_t afh_swap_count;
  
  uint16_t afh_ch_usage_count[20];
} cc85xx_ps_rf_stats_s;

void cc85xx_init(void);
bool cc85xx_sys_reset(void);
bool cc85xx_boot_reset(void);
bool cc85xx_bl_unlock_spi(void);
bool cc85xx_bl_mass_erase(void);
bool cc85xx_bl_flash_verify(uint32_t *pCrc32);
uint16_t cc85xx_get_status(void);
uint16_t cc85xx_get_cached_status(void);
bool cc85xx_flash_bytes(uint16_t addr, uint8_t *pData, uint16_t dataLen);
bool cc85xx_di_get_chip_info(cc85xx_di_get_chip_info_rsp_s *pInfo);
bool cc85xx_di_get_device_info(cc85xx_di_get_device_info_rsp_s *pInfo);
bool cc85xx_ehc_evt_mask(cc85xx_ehc_evt_mask_cmd_s *pCmd);
bool cc85xx_ehc_evt_clr(cc85xx_ehc_evt_clr_cmd_s *pCmd);
bool cc85xx_nwm_do_scan(cc85xx_nwm_do_scan_cmd_s *pCmd);
bool cc85xx_nwm_get_scan_results(cc85xx_nwm_do_scan_rsp_s *pRsp, uint16_t *pRxLen);
bool cc85xx_nwm_do_join(cc85xx_nwm_do_join_cmd_s *pCmd);
bool cc85xx_nwm_get_status(uint8_t *pRsp, uint16_t *pRxLen);
bool cc85xx_nvs_set_data(uint8_t slotIdx, uint32_t data);
bool cc85xx_nvs_get_data(uint8_t slotIdx, uint32_t *pData);
bool cc85xx_ps_rf_stats(cc85xx_ps_rf_stats_s *pRsp, uint16_t *pRxLen);


#endif /* CC85XX_H */
/* [] END OF FILE */
