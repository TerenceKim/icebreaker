#include <RfController.h>
#include <Application.h>
#include <cc85xx.h>

volatile uint32_t rfEvents;



void RfControllerInit(void)
{
  cc85xx_init();
}

void RfControllerService(void)
{

}

bool RfControllerPrintInfo(void)
{
  uint8_t rsp[sizeof(cc85xx_di_get_chip_info_rsp_s)];
  cc85xx_di_get_chip_info_rsp_s *pRsp = (cc85xx_di_get_chip_info_rsp_s *)rsp;
  cc85xx_di_get_device_info_rsp_s *pInfo = (cc85xx_di_get_device_info_rsp_s *)rsp;
  bool ret = cc85xx_di_get_chip_info(pRsp);
  if (!ret)
  {
    PRINTF("ERROR: Failed to fetch chip info\n");
    return ret;
  }

#if 0
  uint32_t i;
  for (i = 0; i < 24; i++)
  {
    PRINTF("0x%02X: %02X\n", i, rsp[i]);
  }
#endif
  
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
  
  ret = cc85xx_di_get_device_info(pInfo);
  if (!ret)
  {
    PRINTF("ERROR: Failed to get device info\n");
    return ret;
  }
  
  PRINTF("\tdevice_id: 0x%08X\n", BE32(pInfo->device_id));
  PRINTF("\tmanf_id: 0x%08X\n", BE32(pInfo->manf_id));
  PRINTF("\tprod_id: 0x%08X\n", BE32(pInfo->prod_id));

  return ret;
}

void RfControllerStartScan(void)
{
  uint32_t i;
  uint8_t *pData;
  cc85xx_nwm_do_scan_cmd_s cmd;

  memset(&cmd, 0, sizeof(cc85xx_nwm_do_scan_cmd_s));
  cmd.scan = RF_SCAN_TIME_SECONDS * 100; // in 10's of ms
  cmd.scan |= (1 & 0xF) << 12;
  cmd.scan = BE16(cmd.scan);
  cmd.req_rssi = 0x80; // -128dB to disable filtering
  
  pData = (uint8_t *)&cmd;
  
  PRINTF("Scan command: ");
  for (i = 0; i < sizeof(cc85xx_nwm_do_scan_cmd_s); i++)
  {
    PRINTF("%02X ", pData[i]);
  }
  PRINTF("\n");

  cc85xx_nwm_do_scan(&cmd);
  
  PRINTF("SW: 0x%04X\n", cc85xx_get_cached_status());
}

bool RfControllerPrintScanResults(void)
{
  int rssi;
  uint32_t i;
  uint16_t rxLen = sizeof(cc85xx_nwm_do_scan_rsp_s);
  cc85xx_nwm_do_scan_rsp_s rsp;
  bool ret = false;

  memset(&rsp, 0, sizeof(cc85xx_nwm_do_scan_rsp_s));
  
  ret = cc85xx_nwm_get_scan_results(&rsp, &rxLen);
  
  PRINTF("SW: 0x%04X\n", cc85xx_get_cached_status());

  if (ret)
  {
    if (rxLen > 0)
    {
      PRINTF("RXLEN = %d\n", rxLen);
      PRINTF("\tdevice_id\t= 0x%08X\n", BE32(rsp.device_id));
      PRINTF("\tmanf_id\t= 0x%08X\n", BE32(rsp.manf_id));
      PRINTF("\tprod_id\t= 0x%08X\n", BE32(rsp.prod_id));
      
      PRINTF("\twpm_allows_join\t= %d\n", rsp.wpm_allows_join);
      PRINTF("\twpm_pair_signal\t= %d\n", rsp.wpm_pair_signal);
      PRINTF("\twpm_mfct_filt\t= %d\n", rsp.wpm_mfct_filt);
      PRINTF("\twpm_dsc_en\t= %d\n", rsp.wpm_dsc_en);
      PRINTF("\twpm_pm_state\t= %d\n", rsp.wpm_pm_state);
      
      PRINTF("\tach_support\t= 0x%04X\n", BE16(rsp.ach_support));

      for (i = 0; i < 8; i++)
      {
        PRINTF("\tach[%d][1].format = %d\n", i, rsp.ach[i].format1);
        PRINTF("\tach[%d][1].active = %d\n", i, rsp.ach[i].active1);
        PRINTF("\tach[%d][0].format = %d\n", i, rsp.ach[i].format0);
        PRINTF("\tach[%d][0].active = %d\n", i, rsp.ach[i].active0);
      }
      
      if (rsp.rssi & 0x80)
      {
        rssi = ((rsp.rssi ^ 0xFF) + 0x1) * -1;
      }
      else
      {
        rssi = rsp.rssi;
      }

      PRINTF("\trssi\t= %d dBm\n", rssi);

      PRINTF("\tsample_rate\t= %d Hz\n", ((rsp.sample_rate_hi << 8 | rsp.sample_rate_lo) * 25));
      PRINTF("\taudio_latency\t= %d samples\n", ((rsp.audio_latency_hi << 8 | rsp.audio_latency_lo)));
      return true;
    }
  }

  return false;
}

void RfControllerPrintStats(void)
{
  uint32_t i;
  uint16_t rxLen = sizeof(cc85xx_ps_rf_stats_s);
  cc85xx_ps_rf_stats_s rsp;
  bool ret = cc85xx_ps_rf_stats(&rsp, &rxLen);
  
  if (ret)
  {
    PRINTF("RXLEN = %d\n", rxLen);
    
    PRINTF("\ttimeslot_count\t= %d\n", BE32(rsp.timeslot_count));
    PRINTF("\trx_pkt_count\t= %d\n", BE32(rsp.rx_pkt_count));
  
    PRINTF("\trx_pkt_fail_count\t= %d\n", BE32(rsp.rx_pkt_fail_count));
    PRINTF("\trx_slice_count\t= %d\n", BE32(rsp.rx_slice_count));
    
    PRINTF("\trx_slice_err_count\t= %d\n", BE32(rsp.rx_slice_err_count));

    PRINTF("\tnwk_join_count\t= %d\n", rsp.nwk_join_count);
    PRINTF("\tnwk_drop_count\t= %d\n", rsp.nwk_drop_count);
    PRINTF("\tafh_swap_count\t= %d\n", BE16(rsp.afh_swap_count));
    
    for (i = 0; i < 20; i++)
    {
      PRINTF("\tafh_ch_usage_count[%d]\t= %d\n", i, BE16(rsp.afh_ch_usage_count[i]));
    }
  }
}