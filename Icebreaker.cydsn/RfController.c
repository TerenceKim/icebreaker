#include <RfController.h>
#include <Application.h>
#include <cc85xx.h>
#include <utils.h>
#include <SystemManager.h>

#define IS_MASTER()                 (rfControllerCb.role == PROTOCOL_ROLE_master)
#define IS_SLAVE()                  (rfControllerCb.role == PROTOCOL_ROLE_slave)

#define RF_SCAN_INTERVAL_MS         (1000)
#define RF_JOIN_INTERVAL_MS         (1000)

#define RF_MANF_ID                  (0x00000001)
#define RF_PRODUCT_ID_SLAVE         (0xB3F1233D)

#define RF_NVS_ID_AUTO_CONN_DEV_ID  (0)

volatile uint32_t rfEvents;

typedef enum
{
  RUN_MODE_unknown,
  RUN_MODE_bootloader,
  RUN_MODE_app,
  RUN_MODE_factorytest
} rf_controller_run_mode_e;

typedef struct
{
  rf_controller_nwk_state_e nwkState;
} rf_controller_master_cb_s;

typedef struct
{
  rf_controller_nwk_state_e nwkState;
  uint32_t                  lastConnectedDeviceId;
  uint32_t                  foundDeviceId;
  uint32_t                  joinDeviceId;
  bool                      printScanResults;
} rf_controller_slave_cb_s;

typedef struct
{
  rf_controller_run_mode_e        runMode;
  rf_controller_protocol_role_e   role;
  union role_cb_u
  {
    rf_controller_master_cb_s     master;
    rf_controller_slave_cb_s      slave;
  } roleCb;
  uint32_t                        startTime;
  tmrFuncType                     tmr;
  uint32_t                        tmrEvent;
} rf_controller_cb_s;

static const char *nwkStateStr[] =
{
  "idle",
  "scanning",
  "auto_joining",
  "joining",
  "connected",
  "master_discoverable",
};

static rf_controller_cb_s rfControllerCb;

static void rfControllerNetworkJoin(void);
static void rfControllerAutoConnect(void);
static void rfControllerNetworkScan(void);

static uint16_t rfControllerSendEvent(void)
{
  rfSetEvents(rfControllerCb.tmrEvent);

  return 0;
}

static void rfClearLaterEvents(void)
{
  tmrFuncDelete(&rfControllerCb.tmr);
}

static void rfSetEventsLater(uint32_t e, uint16_t mS)
{
  rfControllerCb.tmr.mS = mS;
  rfControllerCb.tmr.func = rfControllerSendEvent;
  rfControllerCb.tmrEvent = e;
  tmrFuncAdd(&rfControllerCb.tmr);
}

static bool rfControllerCheckNetworkScanResults(void)
{
  uint16_t rxLen = sizeof(cc85xx_nwm_do_scan_rsp_s);
  cc85xx_nwm_do_scan_rsp_s rsp;
  bool ret = false;
  uint32_t i;
  int rssi;

  memset(&rsp, 0, sizeof(cc85xx_nwm_do_scan_rsp_s));
  
  ret = cc85xx_nwm_get_scan_results(&rsp, &rxLen);
  if (ret)
  {
    rfControllerCb.roleCb.slave.foundDeviceId = BE32(rsp.device_id);
    
    if (rfControllerCb.roleCb.slave.printScanResults)
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
        rfControllerCb.roleCb.slave.printScanResults = false;   
      }
      else
      {
        return false;
      }
    }
  }

  return ret;
}

static bool rfControllerNetworkGetStatus(void)
{
  bool ret = false;
  uint16_t rxLen = 0;
  uint8_t rsp[256];
  cc85xx_nwm_get_status_rsp_slave_s *pStatus = (cc85xx_nwm_get_status_rsp_slave_s *)rsp;

  if (IS_SLAVE())
  {
    rxLen = sizeof(cc85xx_nwm_get_status_rsp_slave_s);

    ret = cc85xx_nwm_get_status((uint8_t *)pStatus, &rxLen);
    if (!ret)
    {
      return ret;
    }
    
    D_PRINTF(INFO, "RXLEN = %d\n", rxLen);
    printArray(rsp, rxLen);

    return (rxLen != 0);
  }
  else if (IS_MASTER())
  {
    // TODO: Add master logic
  }

  return ret;
}

static void rfControllerSlaveSetState(rf_controller_nwk_state_e newState)
{
  if (newState == rfControllerCb.roleCb.slave.nwkState)
  {
    return;
  }
  else
  {
    rfClearLaterEvents();
  }

  switch (newState)
  {
    case NWK_STATE_idle:
    {
      if (RfControllerGetState() == NWK_STATE_connected)
      {
        // Disconnect
        RfControllerNetworkDisconnect();
      }
    } break;

    case NWK_STATE_auto_joining:
    {
      // Start auto-connect
      rfControllerAutoConnect();
    } break;

    case NWK_STATE_scanning:
    {
      // Start searching
      rfControllerNetworkScan();
    } break;

    case NWK_STATE_joining:
    {
      // Start join
      rfControllerNetworkJoin();
    } break;

    case NWK_STATE_connected:
    {
      cc85xx_nwm_ach_set_usage_cmd_s cmd;
      
      // Disable all other audio channels
      memset((void *)&cmd, 0xFF, sizeof(cc85xx_nwm_ach_set_usage_cmd_s));
      
      cmd.ach[CC85XX_ACH_front_primary_left] = 0x00;
      cmd.ach[CC85XX_ACH_front_primary_right] = 0x01;

      // Enable audio channels
      cc85xx_nwm_ach_set_usage(&cmd);
      
      if (rfControllerCb.roleCb.slave.lastConnectedDeviceId != rfControllerCb.roleCb.slave.joinDeviceId)
      {
        rfControllerCb.roleCb.slave.lastConnectedDeviceId = rfControllerCb.roleCb.slave.joinDeviceId;
        
        // Store away for auto-conn on next boot-up
        cc85xx_nvs_set_data(RF_NVS_ID_AUTO_CONN_DEV_ID, rfControllerCb.roleCb.slave.lastConnectedDeviceId);
      }
    } break;

    default:
      break;
  }

  D_PRINTF(INFO, "RF Slave state: %s -> %s\n", nwkStateStr[rfControllerCb.roleCb.slave.nwkState], nwkStateStr[newState]);

  rfControllerCb.roleCb.slave.nwkState = newState;
}

static void rfControllerNetworkJoin(void)
{
  cc85xx_nwm_do_join_cmd_s cmd = 
  {
    .join_to = BE16(RF_JOIN_INTERVAL_MS / 10),
    .device_id = BE32(rfControllerCb.roleCb.slave.joinDeviceId),
    .manf_id = 0,
    .prod_id_mask = 0,
    .prod_id_ref = 0
  };
  cc85xx_nwm_do_join(&cmd);

  if (rfControllerCb.roleCb.slave.joinDeviceId)
  {
    // Check for connection later
    rfSetEventsLater(RF_EVENTS_JOIN_CHECK, RF_JOIN_INTERVAL_MS);
  }
}

static void rfControllerAutoConnect(void)
{
  RfControllerNetworkJoinById(rfControllerCb.roleCb.slave.lastConnectedDeviceId);
}

static void rfControllerNetworkScan(void)
{
  uint32_t i;
  uint8_t *pData;
  cc85xx_nwm_do_scan_cmd_s cmd;

  memset(&cmd, 0, sizeof(cc85xx_nwm_do_scan_cmd_s));
  cmd.scan = RF_SCAN_INTERVAL_MS / 10; // in 10's of ms
  cmd.scan |= (1 & 0xF) << 12;
  cmd.scan = BE16(cmd.scan);
  cmd.req_rssi = 0x80; // -128dB to disable filtering
  
  pData = (uint8_t *)&cmd;
  
  D_PRINTF(DEBUG, "Scan command: ");
  for (i = 0; i < sizeof(cc85xx_nwm_do_scan_cmd_s); i++)
  {
    D_PRINTF(DEBUG, "%02X ", pData[i]);
  }
  D_PRINTF(DEBUG, "\n");

  cc85xx_nwm_do_scan(&cmd);
  
  D_PRINTF(DEBUG, "SW: 0x%04X\n", cc85xx_get_cached_status());

  // Check for scan results later
  rfSetEventsLater(RF_EVENTS_SCAN_CHECK, RF_SCAN_INTERVAL_MS);
}

void RfControllerInit(void)
{
  uint8_t rsp[256];
  cc85xx_di_get_chip_info_rsp_s *pChipInfo = (cc85xx_di_get_chip_info_rsp_s *)rsp;
  cc85xx_di_get_device_info_rsp_s *pDevInfo = (cc85xx_di_get_device_info_rsp_s *)rsp;

  memset((void *)&rfControllerCb, 0, sizeof(rf_controller_cb_s));

  cc85xx_init();

  cc85xx_di_get_chip_info(pChipInfo);

  // Sanity check
  if (BE16(pChipInfo->family_id) != CC85XX_FAMILY_ID)
  {
    D_PRINTF(ERROR, "ERROR: Wrong family ID 0x%04X\n", BE16(pChipInfo->family_id));
    return;
  }

  cc85xx_di_get_device_info(pDevInfo);

  switch (BE16(pChipInfo->chip_id))
  {
    case 0:
    {
      D_PRINTF(INFO, "cc85xx in bootloader mode\n");
      rfControllerCb.runMode = RUN_MODE_bootloader;
    } break;

    case 0xFFFF:
    {
      rfControllerCb.runMode = RUN_MODE_app;
      D_PRINTF(INFO, "Device ID: 0x%08X\n", BE32(pDevInfo->device_id));

      if (BE32(pDevInfo->prod_id) == RF_PRODUCT_ID_SLAVE)
      {
        D_PRINTF(INFO, "Protocol Role: slave\n");
        rfControllerCb.role = PROTOCOL_ROLE_slave;
      }
      else
      {
        D_PRINTF(INFO, "Protocol Role: master\n");
        rfControllerCb.role = PROTOCOL_ROLE_master;
      }
    } break;

    default:
      break;
  }

  if (rfControllerCb.role == PROTOCOL_ROLE_slave)
  {
    cc85xx_nvs_get_data(RF_NVS_ID_AUTO_CONN_DEV_ID, &rfControllerCb.roleCb.slave.lastConnectedDeviceId);
  }
}

void RfControllerDeinit(void)
{
  cc85xx_pm_set_state(CC85XX_PM_STATE_off);
}

void RfControllerService(void)
{
  bool ret = false;

  if (rfCheckEvents(RF_EVENTS_POWER_ON))
  {
    rfClearEvents(RF_EVENTS_POWER_ON);

    if (IS_MASTER())
    {
      // TODO: NWM_CONTROL_ENABLE - to go "discoverable"

      // TODO: NWM_CONTROL_SIGNAL - to notify slaves of connectability
    }
    else if (IS_SLAVE())
    {
      if (rfControllerCb.roleCb.slave.lastConnectedDeviceId)
      {
        rfControllerSlaveSetState(NWK_STATE_auto_joining);
      }
      else
      {
        rfControllerSlaveSetState(NWK_STATE_scanning);
      }
    }
  }

  if (rfCheckEvents(RF_EVENTS_POWER_OFF))
  {
    rfClearEvents(RF_EVENTS_POWER_OFF);

    if (IS_SLAVE())
    {
      rfControllerSlaveSetState(NWK_STATE_idle);
    }
  }
  
  if (rfCheckEvents(RF_EVENTS_SCAN_START))
  {
    rfClearEvents(RF_EVENTS_SCAN_START);
    
    rfControllerSlaveSetState(NWK_STATE_scanning);
  }

  if (rfCheckEvents(RF_EVENTS_SCAN_CHECK))
  {
    // Save flag since it may be modified later by rfControllerCheckNetworkScanResults() 
    ret = rfControllerCb.roleCb.slave.printScanResults;
        
    rfClearEvents(RF_EVENTS_SCAN_CHECK);

    if (rfControllerCheckNetworkScanResults())
    {
      if (ret == false)
      {
        // Command not triggered by shell
        // Attempt to connect to device that was found
        rfControllerCb.roleCb.slave.joinDeviceId = rfControllerCb.roleCb.slave.foundDeviceId;
        rfControllerSlaveSetState(NWK_STATE_joining);
      }
      else
      {
        // We are done here
        rfControllerSlaveSetState(NWK_STATE_idle);
      }
    }
    else
    {
      // Keep searching
      rfControllerNetworkScan();
    }
  }
  
  if (rfCheckEvents(RF_EVENTS_SCAN_STOP))
  {
    rfClearEvents(RF_EVENTS_SCAN_STOP);

    rfControllerCb.roleCb.slave.printScanResults = false;

    rfControllerSlaveSetState(NWK_STATE_idle);
  }
  
  if (rfCheckEvents(RF_EVENTS_SCAN_START))
  {
    rfClearEvents(RF_EVENTS_SCAN_START);

    rfControllerSlaveSetState(NWK_STATE_scanning);
  }

  if (rfCheckEvents(RF_EVENTS_JOIN_CHECK))
  {
    rfClearEvents(RF_EVENTS_JOIN_CHECK);

    if (rfControllerNetworkGetStatus())
    {
      rfControllerSlaveSetState(NWK_STATE_connected);
      
      sysSetEvents(SYS_EVENTS_NWK_JOINED);
    }
    else
    {
      // Keep attempting to connect
      rfControllerNetworkJoin();
    }
  }
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

void RfControllerNetworkJoinById(uint32_t deviceId)
{
  rfControllerCb.roleCb.slave.joinDeviceId = deviceId;
  
  rfControllerSlaveSetState(NWK_STATE_joining);
}

void RfControllerNetworkDisconnect(void)
{
  rfControllerCb.roleCb.slave.joinDeviceId = 0;
  
  rfControllerNetworkJoin();
}

void RfControllerPrintScanResults(void)
{
  rfControllerCb.roleCb.slave.printScanResults = true;
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

bool RfControllerPrintNetworkStats(void)
{
  bool ret = false;
  uint16_t rxLen;
  uint32_t i;
  int rssi;
  
  if (IS_SLAVE())
  {
    cc85xx_nwm_get_status_rsp_slave_s rsp;
    rxLen = sizeof(cc85xx_nwm_get_status_rsp_slave_s);
    ret = cc85xx_nwm_get_status((uint8_t *)&rsp, &rxLen);
    if (ret && rxLen > 0)
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
      PRINTF("\tnwk_status\t= %d\n", rsp.nwk_status);
      PRINTF("\taudio_latency\t= %d samples\n", ((rsp.latency_hi << 8 | rsp.latency_lo)));
      PRINTF("\tach_used\t= 0x%04X\n", BE16(rsp.ach_used));
    }
    else
    {
      return false;
    }
  }
  
  return ret;
}

bool RfControllerPrintPmData(void)
{
  cc85xx_pm_get_data_s rsp;
  uint16_t rxLen = sizeof(cc85xx_pm_get_data_s);
  bool ret = cc85xx_pm_get_data(&rsp, &rxLen);
  
  if (ret && rxLen > 0)
  {
    PRINTF("RXLEN = %d\n", rxLen);
    PRINTF("\tin_silence_time\t= %d ms\n", BE32(rsp.in_silence_time) * 10);
    PRINTF("\tout_silence_time\t= %d ms\n", BE32(rsp.out_silence_time) * 10);
    PRINTF("\tnwk_inactivity_time\t= %d ms\n", BE32(rsp.nwk_inactivity_time) * 10);
    PRINTF("\tvbat_voltage\t= %d mV\n", BE16(rsp.vbat_voltage));
  }
  else
  {
    return false;
  }
  
  return ret;
}

rf_controller_nwk_state_e RfControllerGetState(void)
{
  if (IS_SLAVE())
  {
    return rfControllerCb.roleCb.slave.nwkState;
  }
  else
  {
    return rfControllerCb.roleCb.master.nwkState;
  }
}

rf_controller_protocol_role_e RfControllerGetRole(void)
{
  return rfControllerCb.role;
}
